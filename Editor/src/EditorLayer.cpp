#include "jnypch.h"
#include "EditorLayer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace jny
{

EditorLayer::EditorLayer(Application* app) :
	Layer("EditorLayers"),
	m_app(app)
{
	JNY_ASSERT(app != nullptr, "Use valid app");
}

void EditorLayer::attach()
{
	const auto& vfs = Application::subsystems().st<VFS>();

	//-- Create frambuffer to draw in it instead of directly drawing on screen
	m_framebuffer = Framebuffer::create({});
	//-- Zoom level allows us to not be too close to object

	//-- Let's test creating entity and components
	SpriteComponent sampleSpriteComponent;
	sampleSpriteComponent.m_position = { -1.0f, -1.0f, 0.5f };
	sampleSpriteComponent.m_size = { 1.0f, 1.0f, 0.0f };
	sampleSpriteComponent.m_color = { 0.2f, 0.8f, 0.0f, 0.7f };
	sampleSpriteComponent.m_texture = Texture2D::create(vfs.virtualToNativePath("assets/textures/bomb.png").string());

	TransformComponent sampleTransformComponent;
	sampleTransformComponent.m_transform = glm::translate(sampleTransformComponent.m_transform, sampleSpriteComponent.m_position);
	sampleTransformComponent.m_transform = glm::scale(sampleTransformComponent.m_transform,
		{ sampleSpriteComponent.m_size.x, sampleSpriteComponent.m_size.y, 0.0f });

	//-- In scene all entities are living
	m_scene = Ref<Scene>::create();
	m_sampleE = m_scene->createEntity();
	m_sampleE.addComponent<SpriteComponent>(std::move(sampleSpriteComponent));
	m_sampleE.component<TransformComponent>() = std::move(sampleTransformComponent);
	m_sampleE.component<EntityNameComponent>().m_name = "Sample Quad";

	m_cameraE = m_scene->createEntity();
	m_cameraE.addComponent<CameraComponent>().m_primer = true;
	m_cameraE.component<EntityNameComponent>().m_name = "Camera";
}

void EditorLayer::detach() { }

void EditorLayer::update(f32 dt)
{
	PROFILE_FUNC;

	//-- Resizing viewport
	const auto& specs = m_framebuffer->specs();
	if (static_cast<u32>(m_viewportSize.x) != specs.m_width
		|| static_cast<u32>(m_viewportSize.y) != specs.m_height)
	{
		m_framebuffer->resize({ std::max(m_viewportSize.x, 1.0f), std::max(m_viewportSize.y, 1.0f) });
		m_scene->onViewportResize(static_cast<u32>(m_viewportSize.x), static_cast<u32>(m_viewportSize.y));
	}

	//-- Update camera
	if (m_viewportActive)
	{
		//-- update camera props? maybe make static method in OCC?
	}

	auto& rc = Application::subsystems().st<RenderCommand>();
	auto& renderer2D = Application::subsystems().st<Renderer2D>();

	renderer2D.resetStatistics();
	m_framebuffer->bind();

	rc.setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	rc.clear();

	//-- Updating our scene
	m_scene->update(dt);

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

	ImGui::Begin("TstWindow");

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

	if (m_sampleE)
	{
		auto& sc = m_sampleE.component<SpriteComponent>();
		//ImGui::DragFloat2("Position", glm::value_ptr(sc.m_position), 0.01f);
		ImGui::ColorEdit4("Color", glm::value_ptr(sc.m_color));
	}
	
	ImGui::Text("FPS: %d", static_cast<int>(m_FPS));

	const auto& stat = Application::subsystems().st<Renderer2D>().stats();
	ImGui::Text("Draw calls: %d", stat.m_drawCalls);
	ImGui::Text("Quads count: %d", stat.m_quadCount);

	ImGui::End();
}

}