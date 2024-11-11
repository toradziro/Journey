#pragma once
#pragma once

#include "Journey/Core/SingletonInterface.h"
#include "Journey/Core/Reference.h"
#include "Journey/Core/fs/VirtualFileSystem.h"

namespace jny
{

class Scene;

class ScenesManager : ISingleton
{
	JNY_SINGLETON_TYPE(ScenesManager);
	ScenesManager() = default;

public:
	void				init();
	Ref<Scene>			create(const std::string& scenePath);

	void				update(float dt);

	const std::vector<fs_path>& allScenesOnDisk() const
	{
		return m_assetsPaths;
	}

private:
	using PathsToReferences = std::unordered_map<std::string, Ref<Scene>>;

	PathsToReferences			m_scenesLibrary;
	std::vector<fs_path>		m_assetsPaths;
};

}