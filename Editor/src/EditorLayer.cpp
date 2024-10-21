#include "jnypch.h"
#include "EditorLayer.h"
#include "Panels/SceneHierarchy.h"
#include "Panels/EntityProperties.h"

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

	m_panels.push_back(new SceneHierarchy(m_context));
	m_panels.push_back(new EntityProperties(m_context));

	//-- Create frambuffer to draw in it instead of directly drawing on screen
	m_framebuffer = Framebuffer::create({});
	//-- Zoom level allows us to not be too close to object

	//-- Let's test creating entity and components
	SpriteComponent sampleSpriteComponent;
	sampleSpriteComponent.m_color = { 0.2f, 0.8f, 0.0f, 0.7f };
	sampleSpriteComponent.m_texture = Texture2D::create(vfs.virtualToNativePath("assets/textures/bomb.png").string());

	//-- In scene all entities are living
	m_context->m_currentScene = Ref<Scene>::create();
	m_sampleE = m_context->m_currentScene->createEntity();
	m_sampleE.addComponent<SpriteComponent>(std::move(sampleSpriteComponent));
	m_sampleE.component<TransformComponent>().m_position = { 0.0f, 0.0f, 0.0f };
	m_sampleE.component<TransformComponent>().m_scale = { 1.0f, 1.0f, 0.0f };
	m_sampleE.component<EntityNameComponent>().m_name = "Sample Quad";

	m_cameraE = m_context->m_currentScene->createEntity();
	m_cameraE.addComponent<CameraComponent>().m_primer = true;
	m_cameraE.component<EntityNameComponent>().m_name = "Camera";

	class CameraController : public Script
	{
	public:
		CameraController(Entity entity) : Script(entity) {}

		void attach() override {}

		void update(f32 dt) override
		{
			auto& tc = component<TransformComponent>();
			float cameraSpeedWithDeltaTime = m_cameraMoveSpeed * dt;
			auto& cameraPos = tc.m_position;

			auto& inputPoll = Application::subsystems().st<jny::InputPoll>();
			if (inputPoll.mouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
			{
				if (inputPoll.keyPressed(GLFW_KEY_D))
				{
					cameraPos.x += cameraSpeedWithDeltaTime;
				}
				else if (inputPoll.keyPressed(GLFW_KEY_A))
				{
					cameraPos.x -= cameraSpeedWithDeltaTime;
				}

				if (inputPoll.keyPressed(GLFW_KEY_W))
				{
					cameraPos.y += cameraSpeedWithDeltaTime;
				}
				else if (inputPoll.keyPressed(GLFW_KEY_S))
				{
					cameraPos.y -= cameraSpeedWithDeltaTime;
				}
			}
		}

		void detach() override {}

	private:
		f32	m_cameraMoveSpeed = 5.0f;
	};

	m_cameraE.addComponent<NativeScriptComponent>().bind<CameraController>(m_cameraE);
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

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::MenuItem("TstOpt", NULL, &m_tstOpt);
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

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

	for (auto* panel : m_panels)
	{
		panel->updateUI();
	}
	
	if (ImGui::Begin("Statistics info"))
	{
		ImGui::Text("FPS: %d", static_cast<int>(m_FPS));
		const auto& stat = Application::subsystems().st<Renderer2D>().stats();
		ImGui::Text("Draw calls: %d", stat.m_drawCalls);
		ImGui::Text("Quads count: %d", stat.m_quadCount);
		ImGui::End();
	}
}

}