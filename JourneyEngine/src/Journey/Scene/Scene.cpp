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
	m_gameSystems.push_back(std::move(std::make_unique<MainHeroSystem>(m_registry)));
	m_gameSystems.push_back(std::move(std::make_unique<PhysicSystem>(m_registry)));
}

Scene::~Scene()
{
	auto view = m_registry.view<EntityNameComponent>();
	m_registry.destroy(view.begin(), view.end());
}

void Scene::fillQuadsDrawList()
{
	auto group = m_registry.group<SpriteComponent>(entt::get<TransformComponent>);
	m_quadsDrawList.clear();
	m_quadsDrawList.reserve(group.size());
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
		quad.m_entityId = static_cast<i32>(e);

		m_quadsDrawList.push_back(quad);
	}

	std::ranges::sort(m_quadsDrawList, [](const auto& quad1, const auto& quad2)
		{
			return quad1.m_zDepth < quad2.m_zDepth;
		});
}

void Scene::fillCirclesDrawList()
{
	auto group = m_registry.group<CircleComponent>(entt::get<TransformComponent>);
	m_circleDrawList.clear();
	m_circleDrawList.reserve(group.size());
	for (auto& e : group)
	{
		auto& transform = group.get<TransformComponent>(e);
		auto& circleComponent = group.get<CircleComponent>(e);

		CircleCfg circleCfg;
		circleCfg.m_color = circleComponent.m_color;
		circleCfg.m_radius = circleComponent.m_radius;
		circleCfg.m_thikness = circleComponent.m_edgeThikness;
		circleCfg.m_transform = transform.transform();
		circleCfg.m_zDepth = transform.m_position.z;
		circleCfg.m_entityId = static_cast<i32>(e);

		m_circleDrawList.push_back(circleCfg);
	}

	std::ranges::sort(m_circleDrawList, [](const auto& c1, const auto& c2)
		{
			return c1.m_zDepth < c2.m_zDepth;
		});
}

void Scene::fillLinesDrawList()
{
	auto group = m_registry.group<LineComponent>(entt::get<TransformComponent>);
	m_linesDrawList.clear();
	m_linesDrawList.reserve(group.size());
	for (auto& e : group)
	{
		auto& transform = group.get<TransformComponent>(e);
		auto& lineComponent = group.get<LineComponent>(e);

		LineCfg lineCfg;
		lineCfg.m_color = lineComponent.m_color;
		lineCfg.m_startPoint = lineComponent.m_startPoint;
		lineCfg.m_endPoint = lineComponent.m_endPoint;

		lineCfg.m_entityId = static_cast<i32>(e);

		m_linesDrawList.push_back(lineCfg);
	}
}

void Scene::fillDrawLists()
{
	fillQuadsDrawList();
	fillCirclesDrawList();
	fillLinesDrawList();
}

void Scene::drawAllPrimitives()
{
	auto& renderer2D = Application::subsystems().st<Renderer2D>();

	for (auto& quad : m_quadsDrawList)
	{
		renderer2D.drawQuad(quad);
		//renderer2D.drawRectangle({ quad.m_transform, { 1.0f, 0.0f, 0.0f, 1.0f }, quad.m_entityId });
	}
	for (auto& circle : m_circleDrawList)
	{
		renderer2D.drawCircle(circle);
	}
	for (auto& line : m_linesDrawList)
	{
		renderer2D.drawLine(line);
	}
}

void Scene::update(f32 dt)
{
	CameraComponent* mainCamera = nullptr;
	glm::mat4 mainCameraTransform = {};

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

		//-- update all game systems
		for (auto& s : m_gameSystems)
		{
			s->update(dt);
		}

		fillDrawLists();
		drawAllPrimitives();

		renderer2D.endScene();
	}
}

void Scene::editorModeUpdate(f32 dt, const EditorCamera& camera)
{
	auto& renderer2D = Application::subsystems().st<Renderer2D>();
	renderer2D.beginScene(camera);

	fillDrawLists();
	drawAllPrimitives();

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
	e.addComponent<UuidComponent>();
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
	copyToSnapshot();

	std::ranges::for_each(m_gameSystems, [](auto& s)
		{
			s->attach();
		});
}

void Scene::switchToEditorMode()
{
	restoreFromSnapshot();

	std::ranges::for_each(m_gameSystems, [](auto& s)
		{
			s->detach();
		});
}

void Scene::onComponentCreation(CameraComponent& c)
{
	c.onViewportResize(m_viewportWidth, m_viewportHeight);
}

template<typename Comp>
void copyComponent(Entity& e, entt::registry& registry, entt::entity idToCopy)
{
	if (e.hasComponent<Comp>())
	{
		registry.emplace<Comp>(idToCopy, e.component<Comp>());
	}
}

template<typename Comp>
void copyComponentFromSnapshot(entt::registry& registry
	, entt::registry& snapshot
	, entt::entity idInRegistry
	, entt::entity idInSnapshot)
{
	if (snapshot.all_of<Comp>(idInSnapshot))
	{
		registry.emplace<Comp>(idInRegistry, snapshot.get<Comp>(idInSnapshot));
	}
}

void Scene::copyToSnapshot()
{
	m_registry.view<UuidComponent>().each([&](entt::entity e, UuidComponent& comp)
		{
			Entity wrapperE(e, this);

			entt::entity snapshotE = m_snapshot.create();
			m_snapshot.emplace<UuidComponent>(snapshotE, comp);

			copyComponent<TransformComponent>(wrapperE, m_snapshot, snapshotE);
			copyComponent<SpriteComponent>(wrapperE, m_snapshot, snapshotE);
			copyComponent<EntityNameComponent>(wrapperE, m_snapshot, snapshotE);
			copyComponent<CameraComponent>(wrapperE, m_snapshot, snapshotE);
			copyComponent<MainHeroComponent>(wrapperE, m_snapshot, snapshotE);
			copyComponent<RigidBodyComponent>(wrapperE, m_snapshot, snapshotE);
			copyComponent<BoxColliderComponent>(wrapperE, m_snapshot, snapshotE);
			copyComponent<CircleColliderComponent>(wrapperE, m_snapshot, snapshotE);
			copyComponent<CircleComponent>(wrapperE, m_snapshot, snapshotE);
			copyComponent<LineComponent>(wrapperE, m_snapshot, snapshotE);
		});
}

void Scene::restoreFromSnapshot()
{
	m_registry.clear();

	m_snapshot.view<UuidComponent>().each([&](entt::entity snapshotE, UuidComponent& comp)
		{
			entt::entity registryE = m_registry.create();
			m_registry.emplace<UuidComponent>(registryE, comp);

			copyComponentFromSnapshot<TransformComponent>(m_registry, m_snapshot, registryE, snapshotE);
			copyComponentFromSnapshot<SpriteComponent>(m_registry, m_snapshot, registryE, snapshotE);
			copyComponentFromSnapshot<EntityNameComponent>(m_registry, m_snapshot, registryE, snapshotE);
			copyComponentFromSnapshot<CameraComponent>(m_registry, m_snapshot, registryE, snapshotE);
			copyComponentFromSnapshot<MainHeroComponent>(m_registry, m_snapshot, registryE, snapshotE);
			copyComponentFromSnapshot<RigidBodyComponent>(m_registry, m_snapshot, registryE, snapshotE);
			copyComponentFromSnapshot<BoxColliderComponent>(m_registry, m_snapshot, registryE, snapshotE);
			copyComponentFromSnapshot<CircleColliderComponent>(m_registry, m_snapshot, registryE, snapshotE);
			copyComponentFromSnapshot<CircleComponent>(m_registry, m_snapshot, registryE, snapshotE);
			copyComponentFromSnapshot<LineComponent>(m_registry, m_snapshot, registryE, snapshotE);
		});

	m_snapshot.clear();
}

} //-- jny