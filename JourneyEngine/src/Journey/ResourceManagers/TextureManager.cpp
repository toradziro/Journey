#include "jnypch.h"
#include "TextureManager.h"
#include "Journey/Core/fs/VirtualFileSystem.h"
#include "Journey/Core/Core.h"
#include "Journey/Core/Application.h"
#include "Journey/Renderer/Texture.h"

namespace jny
{

constexpr const char*	C_TEXTURE_FOLDER_PATH = "assets/textures";
constexpr const char*	C_EDITOR_TEXTURES_PATH = "editor/icons";

void TextureManager::init()
{
	auto& vfs = Application::subsystems().st<VFS>();

	auto pathCollect = [&](fs_path nativePath)
		{
			for (const auto& entry : fs::recursive_directory_iterator(nativePath))
			{
				if (!entry.is_directory())
				{
					m_assetsPaths.push_back(jny::normalizePath(entry.path()));
				}
			}
		};

	fs_path projectTexturesNativePath = vfs.virtualToNativePath(C_TEXTURE_FOLDER_PATH);
	fs_path editorTexturesNativePath = vfs.virtualToNativePath(C_EDITOR_TEXTURES_PATH);
	pathCollect(projectTexturesNativePath);
	pathCollect(editorTexturesNativePath);

	Log::info("Textures paths loaded");
}

s_ptr<Texture2D> TextureManager::create(const std::string& texturePath)
{
	std::string normalizedPath = jny::normalizePath(texturePath);

	s_ptr<Texture2D> res = nullptr;
	if (m_textureLibrary.count(normalizedPath))
	{
		res = m_textureLibrary[normalizedPath];
		return res;
	}

	if (auto it = std::ranges::find(m_assetsPaths, normalizedPath); it != m_assetsPaths.end())
	{
		res = Texture2D::create(normalizedPath);
		m_textureLibrary[normalizedPath] = res;
		return res;
	}
	JNY_ASSERT(false, "Add to assets folder!");
	return res;
}

s_ptr<Texture2D> TextureManager::create(const std::string& textureName, u32 width, u32 height)
{
	s_ptr<Texture2D> res = nullptr;
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

void TextureManager::update(float dt)
{
	auto& vfs = Application::subsystems().st<VFS>();
	fs_path nativePath = vfs.virtualToNativePath(C_TEXTURE_FOLDER_PATH);

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