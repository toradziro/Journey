#include "jnypch.h"
#include "EditorLayer.h"
#include "Panels/SceneHierarchy.h"
#include "Panels/EntityProperties.h"
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

	//-- Create frambuffer to draw in it instead of directly drawing on screen
	m_framebuffer = Framebuffer::create({});
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

	//-- Resizing viewport
	const auto& specs = m_framebuffer->specs();
	if (static_cast<u32>(m_viewportSize.x) != specs.m_width
		|| static_cast<u32>(m_viewportSize.y) != specs.m_height)
	{
		m_framebuffer->resize({ std::max(m_viewportSize.x, 1.0f), std::max(m_viewportSize.y, 1.0f) });
		m_context->m_currentScene->onViewportResize
		(
			static_cast<u32>(m_viewportSize.x),
			static_cast<u32>(m_viewportSize.y)
		);
	}

	//m_sampleE2.component<TransformComponent>().m_rotation.x += 15.0f * dt;
	//m_sampleE.component<TransformComponent>().m_rotation.y += 15.0f * dt;

	auto& rc = Application::subsystems().st<RenderCommand>();
	auto& renderer2D = Application::subsystems().st<Renderer2D>();

	renderer2D.resetStatistics();
	m_framebuffer->bind();

	rc.setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	rc.clear();

	//-- Updating our scene
	m_context->m_currentScene->update(dt);

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
					openScene();
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
			}
			return false;
		});
}

void EditorLayer::imGuiRender()
{
	PROFILE_FUNC;

	ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

	if (m_saveScene)
	{
		saveSceneUI();
	}
	if (m_loadScene)
	{
		loadSceneUI();
	}

	drawMenuBar();
	
	if (m_openDemo)
	{
		ImGui::ShowDemoWindow(&m_openDemo);
	}

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
	if (ImGui::Begin("Viewport"))
	{
		m_viewportActive = ImGui::IsWindowHovered() || ImGui::IsWindowFocused();
		m_app->imGuiLayer()->setUpMarkEventsProcessed(!m_viewportActive);

		ImVec2 regionSize = ImGui::GetContentRegionAvail();
		m_viewportSize = regionSize;

		u64 frameId = static_cast<u64>(m_framebuffer->colorAttachment());
		ImGui::Image
		(
			reinterpret_cast<void*>(frameId),
			m_viewportSize,
			ImVec2{ 0.0f, 1.0f },
			ImVec2{ 1.0f, 0.0f }
		);
		
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

void EditorLayer::openScene()
{
	m_loadScene = true;
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

	if (ImGui::Begin("Load Scene", &m_loadScene, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))
	{
		u32 currSelectedScene = DropDownList::C_INVALID_INDEX;

		auto& scenesManager = Application::subsystems().st<ScenesManager>();
		const auto& scenesAssetsPaths = scenesManager.allScenesOnDisk();

		std::vector<std::string> pathsAsStrs;
		pathsAsStrs.reserve(scenesAssetsPaths.size());
		for (const auto& path : scenesAssetsPaths)
		{
			pathsAsStrs.emplace_back(path.generic_string());
		}

		std::string label = fmt::format("##scenesSelector");
		if (DropDownList(pathsAsStrs, label, currSelectedScene).draw())
		{
			m_context->m_currentScene = scenesManager.create(scenesAssetsPaths[currSelectedScene].string());
			m_context->m_currentScene->onViewportResize(static_cast<u32>(m_viewportSize.x), static_cast<u32>(m_viewportSize.y));
			m_context->m_selectedEntity = {};
			m_loadScene = false;
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
				openScene();
			}
			ImGui::MenuItem("Show Demo Window", NULL, &m_openDemo);
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
		auto e = m_context->m_currentScene->activeCameraEntity();
		auto& cc = e.component<CameraComponent>();
		auto& tc = e.component<TransformComponent>();
		glm::mat4 viewMat = glm::inverse(tc.transform());
		glm::mat4 projMat = cc.m_perspectiveCamera.projection();

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

		if (ImGuizmo::Manipulate
		(
			glm::value_ptr(viewMat),
			glm::value_ptr(projMat),
			m_gizmoData.m_gizmoType,
			m_gizmoData.m_coordinateType,
			glm::value_ptr(modelMat),
			nullptr,
			&snapVal
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

	ImGui::SetCursorPos(cursorToRestore);
}

}