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
constexpr const char* C_ENTITY_ID_KEY = "entity_id";
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
		sFile << Key << C_ENTITY_ID_KEY << Value << static_cast<u32>(e);
		serializeEntity(sFile, { e, m_scene.raw() });
		sFile << Newline;
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
			[[maybe_unused]] u32 eId = eSpecs[C_ENTITY_ID_KEY].as<u32>();
			Entity createdE = m_scene->createEntity();

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
					auto textureP = Application::subsystems().st<TextureManager>().create(tPath);
					realSc.m_texture = textureP;
				}
				realSc.m_color = sc["m_color"].as<glm::vec4>();
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