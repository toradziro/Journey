#include "jnypch.h"
#include "SceneSerializer.h"
#include "Journey/Scene/Scene.h"
#include "Journey/Scene/Components.h"
#include "Journey/Core/fs/VirtualFileSystem.h"
#include "Journey/Core/Application.h"
#include <yaml-cpp/yaml.h>

namespace jny
{

void SceneSerializer::serialize(const std::string& filename)
{
	using namespace YAML;
	//-- Generate yaml file
	Emitter sFile;
	sFile << BeginMap;
	sFile << Key << "Scene" << Value << m_scene->name();

	sFile << Key << "Entities" << BeginSeq;
	for (auto [e, eName] : m_scene->m_registry.storage<EntityNameComponent>().each())
	{
		sFile << BeginMap;
		sFile << Key << eName.C_COMPONENT_NAME << Value << eName.m_name;
		sFile << EndMap;
	};
	sFile << EndSeq;
	sFile << EndMap;
	
	//-- Open in scenes in VFS
	constexpr const char* C_SCENE_DIR = "assets/scenes/";

	//-- Dump yaml in a file
	auto& vfs = Application::subsystems().st<VFS>();
	auto file = vfs.createFile(fs_path(C_SCENE_DIR) / fs_path(filename + ".yaml"));
	std::string sFileAsStr = sFile.c_str();
	file->m_buffer.reserve(sFileAsStr.size());
	for (auto c : sFileAsStr)
	{
		file->m_buffer.push_back(c);
	}
	vfs.writeFile(file);
}

void SceneSerializer::deserialize(const std::string& filename)
{
}

void SceneSerializer::runtimeSerialize()
{
}

void SceneSerializer::runtimeDeserialize()
{
}

} //-- jny