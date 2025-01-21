#include "jnypch.h"
#include "Scene.h"
#include "Journey/Core/Application.h"
#include "Journey/Renderer/Renderer2D.h"
#include "Journey/Systems/MainHeroSystem.h"
#include "Journey/Systems/PhysicSystem.h"
#include "Components.h"

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

//-- Containers of tuples with entt to data
//-- group will collect entities with filtering
//-- here we will have all entities that have AND TransfomComponent AND MeshComponent but WITHOUT MovableComponent
auto group = m_registry.group<TransfomComponent, MeshComponent>(entt::get<>, entt::exclude<MovableComponent>);
//-- view will collect all entities that have this particular component
auto view = m_registry.view<TransfomComponent>();
===========================================================================
*/

namespace jny
{

Scene::Scene()
{
	m_gameSystems.push_back(new MainHeroSystem(m_registry));
	m_gameSystems.push_back(new PhysicSystem(m_registry));
}

Scene::~Scene()
{
	auto view = m_registry.view<EntityNameComponent>();
	m_registry.destroy(view.begin(), view.end());
}

void Scene::update(f32 dt)
{
	CameraComponent* mainCamera = nullptr;
	glm::mat4 mainCameraTransform = {};

	std::ranges::for_each(m_gameSystems, [dt](auto& s)
		{
			s->update(dt);
		});

	//-- get in group allows to avoid owning component
	for (auto& e : m_registry.group<CameraComponent>(entt::get<TransformComponent>))
	{
		auto& cam = m_registry.get<CameraComponent>(e);
		
		if (cam.m_viewportH != m_viewportHeight || cam.m_viewportW != m_viewportWidth)
		{
			cam.onViewportResize(m_viewportWidth, m_viewportHeight);
		}

		if (cam.m_primer)
		{
			mainCamera = &cam;
			mainCameraTransform = m_registry.get<TransformComponent>(e).transform();
		}
	}
	
	if (mainCamera)
	{
		auto& renderer2D = Application::subsystems().st<Renderer2D>();
		renderer2D.beginScene(*mainCamera, mainCameraTransform);

		auto group = m_registry.group<SpriteComponent>(entt::get<TransformComponent>);
		std::vector<QuadCfg> drawList;
		drawList.reserve(group.size());
		for (auto& e : group)
		{
			auto& transform = group.get<TransformComponent>(e);
			auto& sprite = group.get<SpriteComponent>(e);

			QuadCfg quad;
			quad.m_color = sprite.m_color;
			if (sprite.m_texture)
			{
				quad.m_textureOpt = TextureOpt::Textured;
				quad.m_texture = sprite.m_texture;
			}
			quad.m_transform = transform.transform();
			quad.m_zDepth = transform.m_position.z;

			drawList.push_back(quad);
		}

		std::ranges::sort(drawList, [](const auto& quad1, const auto& quad2)
			{
				return quad1.m_zDepth < quad2.m_zDepth;
			});

		for (auto& quad : drawList)
		{
			renderer2D.drawQuad(quad);
		}
		renderer2D.endScene();
	}
}

void Scene::editorModeUpdate(f32 dt, const EditorCamera& camera)
{
	auto& renderer2D = Application::subsystems().st<Renderer2D>();
	renderer2D.beginScene(camera);

	auto group = m_registry.group<SpriteComponent>(entt::get<TransformComponent>);
	std::vector<QuadCfg> drawList;
	drawList.reserve(group.size());
	for (auto& e : group)
	{
		auto& transform = group.get<TransformComponent>(e);
		auto& sprite = group.get<SpriteComponent>(e);

		QuadCfg quad;
		quad.m_entityId = static_cast<i32>(e);
		quad.m_color = sprite.m_color;
		if (sprite.m_texture)
		{
			quad.m_textureOpt = TextureOpt::Textured;
			quad.m_texture = sprite.m_texture;
		}
		quad.m_transform = transform.transform();
		quad.m_zDepth = transform.m_position.z;

		drawList.push_back(quad);
	}

	std::ranges::sort(drawList, [](const auto& quad1, const auto& quad2)
		{
			return quad1.m_zDepth < quad2.m_zDepth;
		});

	for (auto& quad : drawList)
	{
		renderer2D.drawQuad(quad);
	}
	renderer2D.endScene();
}

void Scene::onViewportResize(u32 width, u32 height)
{
	m_viewportWidth = width;
	m_viewportHeight = height;

	for (auto& e : m_registry.view<CameraComponent>())
	{
		auto& cam = m_registry.get<CameraComponent>(e);
		if (!cam.m_fixedAspectRatio)
		{
			cam.onViewportResize(m_viewportWidth, m_viewportHeight);
		}
	}
}

Entity Scene::createEntity()
{
	Entity e = Entity(this);
	e.addComponent<TransformComponent>();
	e.addComponent<EntityNameComponent>();
	return e;
}

void Scene::removeEntity(Entity e)
{
	m_registry.destroy(e.m_entity);
}

void Scene::removeEntity(entt::entity e)
{
	m_registry.destroy(e);
}

void Scene::serialize(const std::string& filename)
{
	setName(filename);
	SceneSerializer s(this);
	s.serialize(filename);
}

void Scene::deserialize(const std::string& filename)
{
	SceneSerializer s(this);
	s.deserialize(filename);
}

Entity Scene::activeCameraEntity()
{
	for (const auto e : m_registry.view<CameraComponent>())
	{
		auto& cam = m_registry.get<CameraComponent>(e);
		if (cam.m_primer)
		{
			return { e, this };
		}
	}
	return {};
}

void Scene::switchToGameMode()
{
	std::ranges::for_each(m_gameSystems, [](auto& s)
		{
			s->attach();
		});
}

void Scene::switchToEditorMode()
{
	std::ranges::for_each(m_gameSystems, [](auto& s)
		{
			s->detach();
		});
}

void Scene::onComponentCreation(CameraComponent& c)
{
	c.onViewportResize(m_viewportWidth, m_viewportHeight);
}

} //-- jny