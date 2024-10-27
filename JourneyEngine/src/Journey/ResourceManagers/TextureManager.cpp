#include "jnypch.h"
#include "TextureManager.h"
#include "Journey/Core/fs/VirtualFileSystem.h"
#include "Journey/Core/Core.h"
#include "Journey/Core/Application.h"
#include "Journey/Renderer/Texture.h"

namespace jny
{

constexpr const char*	C_TEXTURE_FOLDER_PATH = "assets/textures";

void TextureManager::init()
{
	auto& vfs = Application::subsystems().st<VFS>();
	fs_path nativePath = vfs.virtualToNativePath(C_TEXTURE_FOLDER_PATH);

	for (const auto& entry : fs::recursive_directory_iterator(nativePath))
	{
		if (!entry.is_directory())
		{
			m_assetsPaths.push_back(entry.path());
		}
	}
	Log::info("Textures paths loaded");
}

Ref<Texture2D> TextureManager::create(const std::string& texturePath)
{
	Ref<Texture2D> res = nullptr;
	if (m_textureLibrary.count(texturePath))
	{
		res = m_textureLibrary[texturePath];
		return res;
	}

	if (auto it = std::ranges::find(m_assetsPaths, texturePath); it != m_assetsPaths.end())
	{
		res = Texture2D::create(texturePath);
		m_textureLibrary[texturePath] = res;
		return res;
	}
	JNY_ASSERT(false, "Add to assets folder!");
	return res;
}

Ref<Texture2D> TextureManager::create(const std::string& textureName, u32 width, u32 height)
{
	Ref<Texture2D> res = nullptr;
	if (m_textureLibrary.count(textureName))
	{
		res = m_textureLibrary[textureName];
	}
	else
	{
		res = Texture2D::create(width, height);
		m_textureLibrary[textureName] = res;
	}

	return res;
}

}