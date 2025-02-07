#include "jnypch.h"
#include "SceneSerializer.h"
#include "Journey/Scene/Scene.h"
#include "Journey/Scene/Components.h"
#include "Journey/Core/fs/VirtualFileSystem.h"
#include "Journey/Core/Application.h"
#include "yaml_extention.h"

namespace jny
{

namespace
{

constexpr const char* C_SCENE_DIR = "assets/scenes/";
constexpr const char* C_ENTITIES_KEY = "Entities";
constexpr const char* C_SCENE_KEY = "Scene";
constexpr const char* C_FILE_EXTENTION = ".yaml";

}

void SceneSerializer::serialize(const std::string& filename)
{
	using namespace YAML;
	//-- Generate yaml serialization file
	Emitter sFile;
	//-- Here goes scene
	sFile << BeginMap;
	sFile << Key << C_SCENE_KEY << Value << m_scene->name();

	//-- Here entities are being written in the scene
	sFile << Key << C_ENTITIES_KEY << BeginSeq;
	for (auto [e, _] : m_scene->m_registry.storage<EntityNameComponent>().each())
	{
		sFile << BeginMap;
		//-- Here will come uuid later
		serializeEntity(sFile, Entity(e, m_scene));
		sFile << EndMap;
	};
	sFile << EndSeq;
	sFile << EndMap;
	
	//-- Open in scenes in VFS
	//-- Dump yaml in a file
	auto& vfs = Application::subsystems().st<VFS>();
	auto file = vfs.createFile(fs_path(C_SCENE_DIR) / fs_path(filename + C_FILE_EXTENTION));
	std::string sFileAsStr = sFile.c_str();
	file->m_buffer.reserve(sFileAsStr.size());
	for (auto c : sFileAsStr)
	{
		file->m_buffer.push_back(c);
	}
	vfs.writeFile(file);
}

void SceneSerializer::serializeEntity(YAML::Emitter& sFile, Entity e)
{
	using namespace YAML;

	//-- Some small components will come as just a key-value
	//-- for more complex once we will create own maps
	if (e.hasComponent<EntityNameComponent>())
	{
		auto& c = e.component<EntityNameComponent>();
		sFile << Key << c.C_COMPONENT_NAME;
		sFile << BeginMap;
		sFile << Key << "m_name" << Value << c.m_name;
		sFile << EndMap;
	}

	if (e.hasComponent<UuidComponent>())
	{
		auto& c = e.component<UuidComponent>();
		sFile << Key << c.C_COMPONENT_NAME;
		sFile << BeginMap;
		sFile << Key << "m_uuid" << Value << (u64)c.m_uuid;
		sFile << EndMap;
	}

	if (e.hasComponent<TransformComponent>())
	{
		auto& c = e.component<TransformComponent>();
		sFile << Key << c.C_COMPONENT_NAME;
		sFile << BeginMap;
		sFile << Key << "m_position" << Value << c.m_position;
		sFile << Key << "m_rotation" << Value << c.m_rotation;
		sFile << Key << "m_scale" << Value << c.m_scale;
		sFile << EndMap;
	}

	if (e.hasComponent<SpriteComponent>())
	{
		auto& c = e.component<SpriteComponent>();
		sFile << Key << c.C_COMPONENT_NAME;
		sFile << BeginMap;
		sFile << Key << "m_color" << Value << c.m_color;
		if (c.m_texture)
		{
			sFile << Key << "m_texture" << Value << c.m_texture->path();
		}
		sFile << EndMap;
	}

	if (e.hasComponent<CameraComponent>())
	{
		auto& c = e.component<CameraComponent>();
		sFile << Key << c.C_COMPONENT_NAME;
		sFile << BeginMap;
		sFile << Key << "m_zoom" << Value << c.m_zoom;
		sFile << Key << "m_fov" << Value << c.m_fov;
		sFile << Key << "m_primer" << Value << c.m_primer;
		sFile << Key << "m_fixedAspectRatio" << Value << c.m_fixedAspectRatio;
		sFile << Key << "m_perspective" << Value << c.m_perspective;
		sFile << EndMap;
	}

	if (e.hasComponent<MainHeroComponent>())
	{
		auto& c = e.component<MainHeroComponent>();
		sFile << Key << c.C_COMPONENT_NAME;
		sFile << BeginMap;
		sFile << Key << "m_movementSpeed" << Value << c.m_movementSpeed;
		sFile << EndMap;
	}

	if (e.hasComponent<RigidBodyComponent>())
	{
		auto& c = e.component<RigidBodyComponent>();
		sFile << Key << c.C_COMPONENT_NAME;
		sFile << BeginMap;
		
		sFile << Key << "m_bodyType" << Value << static_cast<u32>(c.m_bodyType);
		sFile << Key << "m_angularDamping" << Value << c.m_angularDamping;
		sFile << Key << "m_angularVelocity" << Value << c.m_angularVelocity;
		sFile << Key << "m_gravityScale" << Value << c.m_gravityScale;
		sFile << Key << "m_linearDamping" << Value << c.m_linearDamping;
		sFile << Key << "m_linearVelocity" << Value << c.m_linearVelocity;
		sFile << Key << "m_allowFastRotation" << Value << c.m_allowFastRotation;
		sFile << Key << "m_fixedRotation" << Value << c.m_fixedRotation;

		sFile << EndMap;
	}

	if (e.hasComponent<BoxColliderComponent>())
	{
		auto& c = e.component<BoxColliderComponent>();

		sFile << Key << c.C_COMPONENT_NAME;
		sFile << BeginMap;

		sFile << Key << "m_size" << Value << c.m_size;
		sFile << Key << "m_density" << Value << c.m_density;
		sFile << Key << "m_friction" << Value << c.m_friction;
		sFile << Key << "m_restitution" << Value << c.m_restitution;

		sFile << EndMap;
	}

	if (e.hasComponent<CircleComponent>())
	{
		auto& c = e.component<CircleComponent>();

		sFile << Key << c.C_COMPONENT_NAME;
		sFile << BeginMap;

		sFile << Key << "m_color" << Value << c.m_color;
		sFile << Key << "m_edgeThikness" << Value << c.m_edgeThikness;
		sFile << Key << "m_radius" << Value << c.m_radius;

		sFile << EndMap;
	}
}

void SceneSerializer::deserialize(const std::string& filename)
{
	//-- Read file
	auto& vfs = Application::subsystems().st<VFS>();
	auto file = vfs.loadFile(fs_path(C_SCENE_DIR) / fs_path(filename));
	std::string yamlFromDisk = file->toString();
	YAML::Node node = YAML::Load(yamlFromDisk);

	std::string sceneName = node[C_SCENE_KEY].as<std::string>();
	m_scene->setName(sceneName);
	Log::trace("Scene '{}' being loaded", sceneName);

	if (auto entities = node[C_ENTITIES_KEY]; entities)
	{
		for (auto eSpecs : entities)
		{
			Entity createdE = m_scene->createEntity();

			if (auto uuid = eSpecs[UuidComponent::C_COMPONENT_NAME]; uuid)
			{
				createdE.component<UuidComponent>().m_uuid = uuid["m_uuid"].as<u64>();
			}

			if (auto enc = eSpecs[EntityNameComponent::C_COMPONENT_NAME]; enc)
			{
				createdE.component<EntityNameComponent>().m_name = enc["m_name"].as<std::string>();
			}

			if (auto tc = eSpecs[TransformComponent::C_COMPONENT_NAME]; tc)
			{
				auto& realTc = createdE.component<TransformComponent>();
				realTc.m_position = tc["m_position"].as<glm::vec3>();
				realTc.m_rotation = tc["m_rotation"].as<glm::vec3>();
				realTc.m_scale = tc["m_scale"].as<glm::vec3>();
			}

			if (auto cc = eSpecs[CameraComponent::C_COMPONENT_NAME]; cc)
			{
				//-- dont forget to call set fov, set zoom
				auto& realCc = createdE.addComponent<CameraComponent>();
				realCc.m_zoom = cc["m_zoom"].as<f32>();
				realCc.m_fov = cc["m_fov"].as<f32>();
				realCc.m_primer = cc["m_primer"].as<bool>();
				realCc.m_fixedAspectRatio = cc["m_fixedAspectRatio"].as<bool>();
				realCc.m_perspective = cc["m_perspective"].as<bool>();
			}

			if (auto sc = eSpecs[SpriteComponent::C_COMPONENT_NAME]; sc)
			{
				auto& realSc = createdE.addComponent<SpriteComponent>();

				if (auto texture = sc["m_texture"]; texture)
				{
					std::string tPath = texture.as<std::string>();
					realSc.m_texture = Application::subsystems().st<TextureManager>().create(tPath);
				}
				realSc.m_color = sc["m_color"].as<glm::vec4>();
			}

			if (auto mhc = eSpecs[MainHeroComponent::C_COMPONENT_NAME]; mhc)
			{
				auto& realMhc = createdE.addComponent<MainHeroComponent>();
				if (auto moveSpeed = mhc["m_movementSpeed"]; moveSpeed)
				{
					realMhc.m_movementSpeed = moveSpeed.as<f32>();
				}
			}

			if (auto rbc = eSpecs[RigidBodyComponent::C_COMPONENT_NAME]; rbc)
			{
				auto& realRbc = createdE.addComponent<RigidBodyComponent>();
				if (auto bodyType = rbc["m_bodyType"]; bodyType)
				{
					realRbc.m_bodyType = static_cast<BodyType>(bodyType.as<u32>());
				}
				if (auto angularDamping = rbc["m_angularDamping"]; angularDamping)
				{
					realRbc.m_angularDamping = angularDamping.as<f32>();
				}
				if (auto angularVelocity = rbc["m_angularVelocity"]; angularVelocity)
				{
					realRbc.m_angularVelocity = angularVelocity.as<f32>();
				}
				if (auto gravityScale = rbc["m_gravityScale"]; gravityScale)
				{
					realRbc.m_gravityScale = gravityScale.as<f32>();
				}
				if (auto linearDamping = rbc["m_linearDamping"]; linearDamping)
				{
					realRbc.m_linearDamping = linearDamping.as<f32>();
				}
				if (auto linearVelocity = rbc["m_linearVelocity"]; linearVelocity)
				{
					realRbc.m_linearVelocity = linearVelocity.as<glm::vec2>();
				}
				if (auto allowFastRotation = rbc["m_allowFastRotation"]; allowFastRotation)
				{
					realRbc.m_allowFastRotation = allowFastRotation.as<bool>();
				}
				if (auto fixedRotation = rbc["m_fixedRotation"]; fixedRotation)
				{
					realRbc.m_fixedRotation = fixedRotation.as<bool>();
				}
			}

			if (auto bcc = eSpecs[BoxColliderComponent::C_COMPONENT_NAME]; bcc)
			{
				auto& realBcc = createdE.addComponent<BoxColliderComponent>();
				if (auto size = bcc["m_size"]; size)
				{
					realBcc.m_size = size.as<f32>();
				}
				if (auto friction = bcc["m_friction"]; friction)
				{
					realBcc.m_friction = friction.as<f32>();
				}
				if (auto density = bcc["m_density"]; density)
				{
					realBcc.m_density = density.as<f32>();
				}
				if (auto restitution = bcc["m_restitution"]; restitution)
				{
					realBcc.m_restitution = restitution.as<f32>();
				}
			}

			if (auto cc = eSpecs[CircleComponent::C_COMPONENT_NAME]; cc)
			{
				auto& realCc = createdE.addComponent<CircleComponent>();
				if (auto color = cc["m_color"]; color)
				{
					realCc.m_color = color.as<glm::vec4>();
				}
				if (auto edgeThikness = cc["m_edgeThikness"]; edgeThikness)
				{
					realCc.m_edgeThikness = edgeThikness.as<f32>();
				}
				if (auto radius = cc["m_radius"]; radius)
				{
					realCc.m_radius = radius.as<f32>();
				}
			}
		}
	}
}

void SceneSerializer::runtimeSerialize()
{
}

void SceneSerializer::runtimeDeserialize()
{
}

} //-- jny