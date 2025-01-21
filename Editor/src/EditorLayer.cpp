#include "jnypch.h"
#include "EditorLayer.h"
#include "Panels/SceneHierarchy.h"
#include "Panels/EntityProperties.h"
#include "Panels/AssetBrowser.h"
#include "Journey/ResourceManagers/TextureManager.h"
#include "Journey/ResourceManagers/ScenesManager.h"
#include "Journey/ImGui/Controls/DropDownList.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

namespace jny
{

EditorLayer::EditorLayer(Application* app) :
	Layer("EditorLayers"),
	m_context(new EditorContext),
	m_app(app)
{
	JNY_ASSERT(app != nullptr, "Use valid app");
}

void EditorLayer::attach()
{
	const auto& vfs = Application::subsystems().st<VFS>();
	auto& textureManager = Application::subsystems().st<TextureManager>();

	m_panels.push_back(new SceneHierarchy(m_context));
	m_panels.push_back(new EntityProperties(m_context));
	m_panels.push_back(new AssetBrowser(m_context));

	//-- Create frambuffer to draw in it instead of directly drawing on screen
	FramebufferSpecs specs;
	specs.m_width = 1200;
	specs.m_height = 800;
	specs.m_textureDescription = {
		FrambufferTextureFormat::DEPTH24STENCIL8,
		FrambufferTextureFormat::RGBA8,
		FrambufferTextureFormat::RGBA8,
		FrambufferTextureFormat::RED_INTEGER
	};
	//-- FrambufferTextureFormat::RED_INTEGER
	m_frambufferPickingIndex = 2;
	m_framebuffer = Framebuffer::create(std::move(specs));
	m_context->m_currentScene = Ref<Scene>::create();
}

void EditorLayer::detach()
{
	for (auto* panel : m_panels)
	{
		delete panel;
	}
	m_panels.clear();
}

void EditorLayer::update(f32 dt)
{
	PROFILE_FUNC;

	if (m_context->m_sceneChanged)
	{
		m_sceneMode = SceneMode::Editor;
		m_context->m_sceneChanged = false;
		m_context->m_currentScene->onViewportResize(static_cast<u32>(m_viewportSize.x), static_cast<u32>(m_viewportSize.y));
	}

	//-- Resizing viewport
	const auto& specs = m_framebuffer->specs();
	u32 wCasted = std::max<u32>(1, static_cast<u32>(m_viewportSize.x));
	u32 hCasted = std::max<u32>(1, static_cast<u32>(m_viewportSize.y));
	if (wCasted != specs.m_width || hCasted != specs.m_height)
	{
		m_framebuffer->resize({ std::max(m_viewportSize.x, 1.0f), std::max(m_viewportSize.y, 1.0f) });
		m_context->m_currentScene->onViewportResize(wCasted, hCasted);
		m_context->m_editorCamera.onViewportResize(wCasted, hCasted);
	}

	auto& rc = Application::subsystems().st<RenderCommand>();
	auto& renderer2D = Application::subsystems().st<Renderer2D>();

	renderer2D.resetStatistics();
	m_framebuffer->bind();

	rc.setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	rc.clear();
	rc.clearRedIntTexture(m_framebuffer->colorAttachment(m_frambufferPickingIndex), -1);

	//-- Updating our scene
	if (m_sceneMode == SceneMode::Editor)
	{
		if (m_viewportActive)
		{
			m_context->m_editorCamera.update(dt);
		}
		m_context->m_currentScene->editorModeUpdate(dt, m_context->m_editorCamera);
	}
	else
	{
		m_context->m_currentScene->update(dt);
	}

	ImVec2 mousePos = ImGui::GetMousePos();
	mousePos.x -= m_viewportBounds.first.x;
	mousePos.y -= m_viewportBounds.first.y;
	//-- Texture UV coordinates are going from left down corner
	//-- so we need to flip our y axe
	mousePos.y = m_viewportSize.y - mousePos.y;

	int mouseFrameX = static_cast<int>(mousePos.x);
	int mouseFrameY = static_cast<int>(mousePos.y);
	if (mouseFrameX >= 0 &&
		mouseFrameY >= 0 &&
		mouseFrameX <= static_cast<int>(m_viewportSize.x) &&
		mouseFrameY <= static_cast<int>(m_viewportSize.y) &&
		ImGui::IsMouseClicked(ImGuiMouseButton_Left) &&
		!ImGuizmo::IsOver() && !ImGuizmo::IsUsing())
	{
		int entityId = m_framebuffer->readbackPixel(m_frambufferPickingIndex, mouseFrameX, mouseFrameY);
		if (entityId == -1)
		{
			m_context->m_selectedEntity = {};
		}
		m_context->m_selectedEntity = Entity(entt::entity(entityId), m_context->m_currentScene.raw());
	}

	m_framebuffer->unbind();

	m_FPS = 1.0f / dt;
	m_dt = dt;
}

void EditorLayer::onEvent(Event& event)
{
	PROFILE_FUNC;

	EventDispatcher ed(event);
	auto& inputPollSystem = Application::subsystems().st<jny::InputPoll>();
	ed.dispatch<KeyPressedEvent>([&](KeyPressedEvent& e)
		{
			//-- Shortcuts
			if (inputPollSystem.keyPressed(GLFW_KEY_LEFT_CONTROL))
			{
				if (inputPollSystem.keyPressed(GLFW_KEY_LEFT_SHIFT))
				{
					//-- Save As
					if (e.keyCode() == GLFW_KEY_S)
					{
						saveSceneAs();
						return true;
					}
				}
				//-- Open
				if (e.keyCode() == GLFW_KEY_O)
				{
					openSceneUI();
					return true;
				}
				//-- New scene
				if (e.keyCode() == GLFW_KEY_N)
				{
					newScene();
					return true;
				}
				//-- Translate Gizmo
				if (e.keyCode() == GLFW_KEY_W)
				{
					setTranslateGizmo();
				}
				//-- Rotate Gizmo
				if (e.keyCode() == GLFW_KEY_E)
				{
					setRotateGizmo();
				}
				//-- Scale Gizmo
				if (e.keyCode() == GLFW_KEY_R)
				{
					setScaleGizmo();
				}
				//-- Selection mode
				if (e.keyCode() == GLFW_KEY_Q)
				{
					setSelectMode();
				}

				//-- Framebuffer test
				if (e.keyCode() == GLFW_KEY_1)
				{
					m_frambufferIndex = 0;
				}
				if (e.keyCode() == GLFW_KEY_2)
				{
					m_frambufferIndex = 1;
				}
			}
			return false;
		});
	if (m_viewportActive)
	{
		m_context->m_editorCamera.onEvent(event);
	}
}

void EditorLayer::imGuiRender()
{
	PROFILE_FUNC;

	ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

	if (m_saveScene)
	{
		saveSceneUI();
	}
	if (m_loadSceneUI)
	{
		loadSceneUI();
	}

	drawMenuBar();
	
	if (m_openDemo)
	{
		ImGui::ShowDemoWindow(&m_openDemo);
	}

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
	if (ImGui::Begin("Viewport", 0, ImGuiWindowFlags_NoScrollbar))
	{
		ImVec2 offset = ImGui::GetCursorPos();

		m_viewportActive = ImGui::IsWindowHovered() || ImGui::IsWindowFocused();
		m_app->imGuiLayer()->setUpMarkEventsProcessed(!m_viewportActive);

		m_viewportSize = ImGui::GetContentRegionAvail();

		u64 frameId = static_cast<u64>(m_framebuffer->colorAttachment(m_frambufferIndex));
		ImGui::Image
		(
			reinterpret_cast<void*>(frameId),
			m_viewportSize,
			ImVec2{ 0.0f, 1.0f },
			ImVec2{ 1.0f, 0.0f }
		);
		const ImVec2 windowSize = ImGui::GetWindowSize();
		ImVec2 windowLeftCorner = ImGui::GetWindowPos() + offset;
		ImVec2 windowRightCorner = windowLeftCorner + windowSize;
		m_viewportBounds.first = std::move(windowLeftCorner);
		m_viewportBounds.second = std::move(windowRightCorner);

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_BROWSER_ITEM"))
			{
				fs_path path(std::string(static_cast<const char*>(payload->Data), payload->DataSize));
				if (path.extension() == ".yaml")
				{
					openScene(path.string());
				}
			}
			ImGui::EndDragDropTarget();
		}
		
		drawViewportToolbar();
		if (m_showGizmo)
		{
			drawGizmos();
		}
		

		ImGui::End();
	}
	ImGui::PopStyleVar();

	drawStats();

	for (auto* panel : m_panels)
	{
		panel->updateUI();
	}
}

void EditorLayer::openScene(std::string scenePath)
{
	auto& scenesManager = Application::subsystems().st<ScenesManager>();
	m_context->m_selectedEntity = {};
	m_context->m_currentScene = scenesManager.create(scenePath);
	m_context->m_currentScene->onViewportResize(static_cast<u32>(m_viewportSize.x), static_cast<u32>(m_viewportSize.y));
	m_sceneMode = SceneMode::Editor;
}

void EditorLayer::openSceneUI()
{
	m_loadSceneUI = true;
	m_sceneFilename = m_context->m_currentScene->name();
}

void EditorLayer::saveSceneAs()
{
	m_saveScene = true;
	m_sceneFilename = m_context->m_currentScene->name();
}

void EditorLayer::newScene()
{
	m_context->m_currentScene = Ref<Scene>::create();
	m_context->m_selectedEntity = {};
}

void EditorLayer::saveSceneUI()
{
	constexpr ImVec2 winSize = { 250.0f, 150.0f };
	ImVec2 winSizeWithDpi = { winSize.x * ImGui::GetWindowDpiScale()
		, winSize.y * ImGui::GetWindowDpiScale() };

	ImGui::SetNextWindowSize(winSizeWithDpi);

	if (ImGui::Begin("Save Scene", &m_saveScene, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))
	{
		const i32 C_BUF_LENGTH = 1024;

		char buff[C_BUF_LENGTH];
		memset(buff, 0, C_BUF_LENGTH);
		//-- No use strcpy since copiler considers in unsafe
		for (u32 i = 0; i < m_sceneFilename.size(); ++i)
		{
			buff[i] = m_sceneFilename[i];
		}
		if (ImGui::InputText("##sceneSaving", buff, C_BUF_LENGTH))
		{
			m_sceneFilename = buff;
		}

		constexpr ImVec2 buttonSize = { 60.0f, 30.0f };
		ImVec2 btnSizeWithDpi = { buttonSize.x * ImGui::GetWindowDpiScale()
			, buttonSize.y * ImGui::GetWindowDpiScale() };

		if (ImGui::Button("Save", btnSizeWithDpi))
		{
			m_context->m_currentScene->serialize(m_sceneFilename);
			m_saveScene = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel", btnSizeWithDpi))
		{
			m_saveScene = false;
		}
		ImGui::End();
	}
}

void EditorLayer::loadSceneUI()
{
	constexpr ImVec2 winSize = { 250.0f, 150.0f };
	ImVec2 winSizeWithDpi = { winSize.x * ImGui::GetWindowDpiScale()
		, winSize.y * ImGui::GetWindowDpiScale() };

	ImGui::SetNextWindowSize(winSizeWithDpi);

	if (ImGui::Begin("Load Scene", &m_loadSceneUI, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))
	{
		u32 currSelectedScene = DropDownList::C_INVALID_INDEX;

		const auto& scenesAssetsPaths = Application::subsystems().st<ScenesManager>().allScenesOnDisk();

		std::vector<std::string> pathsAsStrs;
		pathsAsStrs.reserve(scenesAssetsPaths.size());
		for (const auto& path : scenesAssetsPaths)
		{
			pathsAsStrs.emplace_back(path.generic_string());
		}

		std::string label = fmt::format("##scenesSelector");
		if (DropDownList(pathsAsStrs, label, currSelectedScene).draw())
		{
			openScene(scenesAssetsPaths[currSelectedScene].string());
			m_loadSceneUI = false;
		}

		ImGui::End();
	}
}

void EditorLayer::drawMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New", "Ctrl+N"))
			{
				newScene();
			}
			if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
			{
				saveSceneAs();
			}
			if (ImGui::MenuItem("Open...", "Ctrl+O"))
			{
				openSceneUI();
			}
			ImGui::MenuItem("Show Demo Window", NULL, &m_openDemo);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Fonts Regular"))
			{
				m_app->imGuiLayer()->setFont(ImGuiLayer::Font::Regular);
			}
			if (ImGui::MenuItem("Fonts Pixel"))
			{
				m_app->imGuiLayer()->setFont(ImGuiLayer::Font::Pixel);
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void EditorLayer::drawStats()
{
	if (ImGui::Begin("Statistics info"))
	{
		ImGui::Text("Current Scene: %s", m_context->m_currentScene->name().c_str());
		if (m_context->m_selectedEntity)
		{
			std::string name = m_context->m_selectedEntity.component<EntityNameComponent>().m_name;
			ImGui::Text("Selected entity: %s", name.c_str());
		}
		else
		{
			ImGui::Text("Selected entity: <none>");
		}
		ImGui::Text("FPS: %d", static_cast<int>(m_FPS));
		ImGui::Text("Time spent on a call: %.1f ms", m_dt * 1000.0f);
		const auto& stat = Application::subsystems().st<Renderer2D>().stats();
		ImGui::Text("Draw calls: %d", stat.m_drawCalls);
		ImGui::Text("Quads count: %d", stat.m_quadCount);
		ImGui::End();
	}
}

void EditorLayer::drawGizmos()
{
	//-- Gizmos
	if (m_context->m_selectedEntity)
	{
		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist();

		float width = ImGui::GetWindowWidth();
		float height = ImGui::GetWindowHeight();
		float posX = ImGui::GetWindowPos().x;
		float posY = ImGui::GetWindowPos().y;
		ImGuizmo::SetRect(posX, posY, width, height);

		//-- camera
		glm::mat4 viewMat = m_context->m_editorCamera.viewMatrix();
		glm::mat4 projMat = m_context->m_editorCamera.projMatrix();

		//-- selecte entity data
		auto& entityTc = m_context->m_selectedEntity.component<TransformComponent>();
		glm::vec3& origRotation = entityTc.m_rotation;
		glm::mat4 modelMat = entityTc.transform();

		//-- snapping
		//-- TODO: add possibility to set snapping val
		bool snapped = false;
		float snapVal = 0.0f;
		if (Application::subsystems().st<InputPoll>().keyPressed(GLFW_KEY_LEFT_CONTROL))
		{
			snapped = true;
			if (m_gizmoData.m_gizmoType == ImGuizmo::OPERATION::TRANSLATE)
			{
				snapVal = 0.5f;
			}
			else if (m_gizmoData.m_gizmoType == ImGuizmo::OPERATION::ROTATE)
			{
				snapVal = 45.0f;
			}
			else if (m_gizmoData.m_gizmoType == ImGuizmo::OPERATION::SCALE)
			{
				snapVal = 0.25f;
			}
		}
		glm::vec3 computedSnap = { snapVal, snapVal, snapVal };

		if (ImGuizmo::Manipulate
		(
			glm::value_ptr(viewMat),
			glm::value_ptr(projMat),
			m_gizmoData.m_gizmoType,
			m_gizmoData.m_coordinateType,
			glm::value_ptr(modelMat),
			nullptr,
			glm::value_ptr(computedSnap)
		))
		{
			glm::vec3 position, scale, skew;
			glm::quat rotation;
			glm::vec4 perspective;

			glm::decompose(
				modelMat,
				scale,
				rotation,
				position,
				skew,
				perspective
			);

			glm::vec3 eulerRotation = glm::degrees(glm::eulerAngles(rotation));
			glm::vec3 deltaRotation = origRotation - eulerRotation;

			entityTc.m_position = position;
			entityTc.m_rotation -= deltaRotation;
			entityTc.m_scale = scale;
		}
	}
}

void EditorLayer::setSelectMode()
{
	m_showGizmo = false;
}

void EditorLayer::setTranslateGizmo()
{
	m_gizmoData.m_gizmoType = ImGuizmo::OPERATION::TRANSLATE;
	m_showGizmo = true;
}

void EditorLayer::setRotateGizmo()
{
	m_gizmoData.m_gizmoType = ImGuizmo::OPERATION::ROTATE;
	m_showGizmo = true;
}

void EditorLayer::setScaleGizmo()
{
	m_gizmoData.m_gizmoType = ImGuizmo::OPERATION::SCALE;
	m_showGizmo = true;
}

void EditorLayer::switchToGameMode()
{
	if (m_context->m_currentScene)
	{
		m_context->m_currentScene->switchToGameMode();
		m_sceneMode = SceneMode::Game;
	}
}

void EditorLayer::switchToEditorMode()
{
	if (m_context->m_currentScene)
	{
		m_context->m_currentScene->switchToEditorMode();
		m_sceneMode = SceneMode::Editor;
	}
}

void EditorLayer::drawViewportToolbar()
{
	const float toolbarPosX = 5.0f * ImGui::GetWindowDpiScale();
	const float toolbarPosY = 30.0f * ImGui::GetWindowDpiScale();

	const ImVec2 cursorToRestore = ImGui::GetCursorPos();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.35f, 0.35f, 0.35f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));

	{
		ImGui::SetCursorPos( { toolbarPosX, toolbarPosY } );
		if (ImGui::Button("Select"))
		{
			setSelectMode();
		}
		ImGui::SameLine();
		if (ImGui::Button("T"))
		{
			setTranslateGizmo();
		}
		ImGui::SameLine();
		if (ImGui::Button("R"))
		{
			setRotateGizmo();
		}
		ImGui::SameLine();
		if (ImGui::Button("S"))
		{
			setScaleGizmo();
		}
	}

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4f, 0.4f, 0.4f, 0.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.35f, 0.35f, 0.35f, 0.4f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.5f, 0.5f, 0.4f));

		constexpr std::string_view	C_PLAY_BUTTON_PATH = "editor/icons/play_button.png";
		constexpr std::string_view	C_STOP_BUTTON_PATH = "editor/icons/stop_button.png";

		constexpr float				C_BUTTON_SIZE = 48.0f;

		const float availableSpaceX = ImGui::GetContentRegionAvail().x;
		ImGui::SetCursorPos( { (availableSpaceX / 2.0f) - (C_BUTTON_SIZE / 2.0f) , toolbarPosY } );

		auto& tm = jny::Application::subsystems().st<jny::TextureManager>();
		auto& vfs = jny::Application::subsystems().st<jny::VFS>();
		Ref<Texture2D> icon = nullptr;

		switch (m_sceneMode)
		{
			case SceneMode::Game:
				icon = tm.create(vfs.virtualToNativePath(C_STOP_BUTTON_PATH).string());
				break;
			case SceneMode::Editor:
				icon = tm.create(vfs.virtualToNativePath(C_PLAY_BUTTON_PATH).string());
				break;
			default:
				break;
		}

		const ImVec2 buttonSizeWithDpi = ImVec2{ C_BUTTON_SIZE, C_BUTTON_SIZE } * ImGui::GetWindowDpiScale();
		if (ImGui::ImageButton(reinterpret_cast<void*>(static_cast<u64>(icon->rendererId())),
			buttonSizeWithDpi,
			ImVec2{ 0.0f, 1.0f },
			ImVec2{ 1.0f, 0.0f },
			0))
		{
			if (m_sceneMode == SceneMode::Editor)
			{
				switchToGameMode();
			}
			else
			{
				switchToEditorMode();
			}
		}

		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
	}

	ImGui::SetCursorPos(cursorToRestore);
}

}