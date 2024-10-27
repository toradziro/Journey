#pragma once

#include "Journey/Core/SingletonInterface.h"
#include "Journey/Core/Reference.h"
#include "Journey/Core/fs/VirtualFileSystem.h"

namespace jny
{

class Texture2D;

class TextureManager : ISingleton
{
	JNY_SINGLETON_TYPE(TextureManager)
	TextureManager() = default;

public:
	void				init();
	Ref<Texture2D>		create(const std::string& texturePath);
	Ref<Texture2D>		create(const std::string& textureName, u32 width, u32 height);

	const std::vector<fs_path>& allTexturesOnDisk() const { return m_assetsPaths; }

private:
	using PathsToReferences = std::unordered_map<std::string, Ref<Texture2D>>;

	PathsToReferences			m_textureLibrary;
	std::vector<fs_path>		m_assetsPaths;
};

}