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
		m_context->m_currentScene->onViewportResize(static_cast<u32>(m_viewportSize.x), static_cast<u32>(m_viewportSize.y));
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

	//-- now process events in entities?
	//m_orthoCameraCtrl.onEvent(event);
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

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::MenuItem("Show Demo Window", NULL, &m_openDemo);
			if (ImGui::MenuItem("Save Scene"))
			{
				m_saveScene = true;
				m_sceneFilename = m_context->m_currentScene->name();
			}
			if (ImGui::MenuItem("Load Scene"))
			{
				m_loadScene = true;
				m_sceneFilename = m_context->m_currentScene->name();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
	
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
		ImGui::Image(reinterpret_cast<void*>(frameId), m_viewportSize, ImVec2{ 0.0f, 1.0f }, ImVec2{ 1.0f, 0.0f });
		ImGui::End();
	}
	ImGui::PopStyleVar();

	for (auto* panel : m_panels)
	{
		panel->updateUI();
	}

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

void EditorLayer::saveSceneUI()
{
	constexpr ImVec2 winSize = { 250.0f, 150.0f };
	ImVec2 winSizeWithDpi = { winSize.x * ImGui::GetWindowDpiScale()
		, winSize.y * ImGui::GetWindowDpiScale() };

	ImGui::SetNextWindowSize(winSizeWithDpi);

	ImGui::Begin("Save Scene", &m_saveScene);
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

void EditorLayer::loadSceneUI()
{
	constexpr ImVec2 winSize = { 250.0f, 150.0f };
	ImVec2 winSizeWithDpi = { winSize.x * ImGui::GetWindowDpiScale()
		, winSize.y * ImGui::GetWindowDpiScale() };

	ImGui::SetNextWindowSize(winSizeWithDpi);

	ImGui::Begin("Load Scene", &m_loadScene);

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