#include "jnypch.h"
#include "Scene.h"
#include "Components.h"
#include "Journey/Core/Application.h"
#include "Journey/Renderer/Renderer2D.h"

#include <glm/glm.hpp>

/*
==============================ENTT USAGE HINT==============================
struct TransfomComponent
{
	glm::mat4 m_transform = { 1.0f };
};

struct MeshComponent { };

struct MovableComponent { };

//-- Creates an entity
entt::entity sampleEntity = m_registry.create();
//-- Add a component to a selected entity
m_registry.emplace<TransfomComponent>(sampleEntity, TransfomComponent());

//-- If entity has all passed components?
if (m_registry.all_of<TransfomComponent>(sampleEntity))
{
	//-- We want this particular component from this entity
	auto& tComp = m_registry.get<TransfomComponent>(sampleEntity);
	//-- And we want to remove component from en entity
	m_registry.remove<TransfomComponent>(sampleEntity);
}
//-- Or it has at least one of passed components types?
if (m_registry.any_of<TransfomComponent>(sampleEntity)){}

//-- Containers of tupeles with entt to data
//-- group will collect etities with filtering
//-- here we will have all entities that have AND TransfomComponent AND MeshComponent but WITHOUT MovableComponent
auto group = m_registry.group<TransfomComponent, MeshComponent>(entt::get<>, entt::exclude<MovableComponent>);
//-- view will collect all entities that have this particular component
auto view = m_registry.view<TransfomComponent>();
===========================================================================
*/

namespace jny
{

Scene::Scene() {}

Scene::~Scene() {}

void Scene::update(f32 dt)
{
	auto& renderer2D = Application::subsystems().st<Renderer2D>();

	auto group = m_registry.group<TransformComponent, SpriteComponent>();
	for (auto& e : group)
	{
		auto& transform = group.get<TransformComponent>(e);
		auto& sprite = group.get<SpriteComponent>(e);

		QuadCfg quad;
		quad.m_color = sprite.m_color;
		quad.m_position = sprite.m_position;
		quad.m_size = sprite.m_size;
		if (sprite.m_texture)
		{
			quad.m_textureOpt = TextureOpt::Textured;
			quad.m_texture = sprite.m_texture;
		}
		quad.m_transform = transform.m_transform;
		quad.m_transformCalculated = true;

		renderer2D.drawQuad(quad);
	}
}

} //-- jny