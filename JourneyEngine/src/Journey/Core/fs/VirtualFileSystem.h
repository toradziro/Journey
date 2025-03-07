#pragma once

#include "Journey/Core/SingletonInterface.h"
#include "Journey/Core/Reference.h"

namespace jny
{

using fs_path = std::filesystem::path;
namespace fs = std::filesystem;

struct File
{
	std::string toString()
	{
		std::string res;
		res.resize(m_buffer.size());
		memcpy(res.data(), m_buffer.data(), m_buffer.size());
		return res;
	}

	fs_path					m_nativePath;
	fs_path					m_virtualPath;
	std::vector<char>		m_buffer;
};

class VFS : ISingleton
{
	JNY_SINGLETON_TYPE(VirtualFileSystem)

public:
	constexpr static std::string_view	C_RESOURCE_DIR_NAME = "resources";

	void		init();

	s_ptr<File>	loadFile(const fs_path& path) const;
	s_ptr<File>	createFile(const fs_path& path) const;
	void		writeFile(const s_ptr<File>& file) const;
	
	fs_path		virtualToNativePath(const fs_path& path) const;
	fs_path		rootResourcesPath() const { return m_vfsPath; }

private:
	fs_path		m_vfsPath = {};
};

std::string normalizePath(jny::fs_path path);

}