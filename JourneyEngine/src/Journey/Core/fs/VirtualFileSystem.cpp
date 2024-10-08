#include "jnypch.h"
#include "VirtualFileSystem.h"
#include "Journey/Core/Core.h"

namespace
{

constexpr std::string_view	C_ENGINE_NAME = "Journey";
constexpr std::string_view	C_RESOURCE_DIR_NAME = "resources/";

}

namespace jny
{

void VFS::init()
{
	fs_path currentPath = std::filesystem::current_path();
	
	std::string currPathAsStr = currentPath.string();
	u32 rootIt = currPathAsStr.find(C_ENGINE_NAME);
	if (rootIt == std::string::npos)
	{
		JNY_ASSERT(false, "Tried to load outside working repository - won't work");
	}
	currPathAsStr = currPathAsStr.substr(0, rootIt + C_ENGINE_NAME.size());

	m_vfsPath = currPathAsStr;
	m_vfsPath = m_vfsPath / C_RESOURCE_DIR_NAME;
	Log::info("{}", m_vfsPath.string());
}

Ref<File> VFS::loadFile(const fs_path& path) const
{
	Ref<File> readFile = new File;
	readFile->m_virtualPath = path;
	readFile->m_nativePath = virtualToNativePath(path);

	std::ifstream in(readFile->m_nativePath, std::ios::in | std::ios::binary);
	if (in)
	{
		in.seekg(0, std::ios::end);
		size_t size = in.tellg();
		if (size != -1)
		{
			readFile->m_buffer.resize(size, 0);
			in.seekg(0, std::ios::beg);
			in.read(readFile->m_buffer.data(), readFile->m_buffer.size());
			in.close();
		}
		else
		{
			JNY_ASSERT(false, "Reading error in file: '{}'", readFile->m_nativePath.string());
		}
	}
	else
	{
		JNY_ASSERT(false, "File '{}' doesn't exist", readFile->m_nativePath.string());
	}

	return readFile;
}

void VFS::writeFile(const Ref<File>& file) const
{
	std::ofstream out(file->m_nativePath, std::ios::out | std::ios::trunc | std::ios::binary);
	if (out)
	{
		out.write(file->m_buffer.data(), file->m_buffer.size());
		out.close();
	}
	else
	{
		JNY_ASSERT(false, "Can't open for writing file: '{}'", file->m_nativePath.string());
	}
}

fs_path VFS::virtualToNativePath(const fs_path& path) const
{
	auto res = m_vfsPath / path;
	return res;
}

}