#include "jnypch.h"
#include "ScenesManager.h"
#include "Journey/Core/fs/VirtualFileSystem.h"
#include "Journey/Core/Core.h"
#include "Journey/Core/Application.h"
#include "Journey/Scene/Scene.h"

namespace jny
{

namespace
{

constexpr const char* C_SCENES_FOLDER_PATH = "assets/scenes";

std::string fromNativeToVirtual(const std::string& native)
{
	u64 pos = native.find(std::string(C_SCENES_FOLDER_PATH));
	if (pos != std::string::npos)
	{
		return native.substr(pos);
	}
	return {};
}

}



void ScenesManager::init()
{
	auto& vfs = Application::subsystems().st<VFS>();
	fs_path nativePath = vfs.virtualToNativePath(C_SCENES_FOLDER_PATH);

	for (const auto& entry : fs::recursive_directory_iterator(nativePath))
	{
		if (!entry.is_directory())
		{
			m_assetsPaths.push_back(entry.path());
		}
	}
	Log::info("Scenes paths loaded");
}

Ref<Scene> ScenesManager::create(const std::string& scenePath)
{
	Ref<Scene> res = nullptr;
	//if (m_scenesLibrary.count(scenePath))
	//{
	//	res = m_scenesLibrary[scenePath];
	//	return res;
	//}

	if (auto it = std::ranges::find(m_assetsPaths, scenePath); it != m_assetsPaths.end())
	{
		res = Ref<Scene>::create();
		res->deserialize(it->filename().string());
		//m_scenesLibrary[scenePath] = res;
		return res;
	}
	JNY_ASSERT(false, "Add to assets folder!");
	return res;
}

void ScenesManager::update(float dt)
{
	auto& vfs = Application::subsystems().st<VFS>();
	fs_path nativePath = vfs.virtualToNativePath(C_SCENES_FOLDER_PATH);

	for (auto it = m_assetsPaths.begin(); it != m_assetsPaths.end(); ++it)
	{
		if (!fs::exists(*it))
		{
			it = m_assetsPaths.erase(it);
		}
	}

	for (const auto& entry : fs::recursive_directory_iterator(nativePath))
	{
		if (!entry.is_directory())
		{
			auto pos = std::find(m_assetsPaths.begin(), m_assetsPaths.end(), entry.path());
			if (pos == m_assetsPaths.end())
			{
				m_assetsPaths.push_back(entry.path());
			}
		}
	}
}

}