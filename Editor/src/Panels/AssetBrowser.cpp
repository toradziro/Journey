#include "jnypch.h"
#include "AssetBrowser.h"
#include "Journey/Core/Application.h"
#include <imgui.h>

namespace
{

constexpr std::string_view	C_ROOT_PATH = "/";

std::string normalizePath(jny::fs_path path)
{
	std::string res = path.string();
	size_t startPos = 0;
	while ((startPos = res.find('\\', startPos)) != std::string::npos)
	{
		res.replace(startPos, 1, "/");
		startPos += 1;
	}
	return res;
}

std::string convertToVfsPath(jny::fs_path path)
{
	std::string currPathAsStr = normalizePath(path);
	u64 rootIt = currPathAsStr.find(jny::VFS::C_RESOURCE_DIR_NAME);
	currPathAsStr = currPathAsStr.substr(rootIt + jny::VFS::C_RESOURCE_DIR_NAME.size(), currPathAsStr.size());
	if (currPathAsStr.empty())
	{
		return std::string(C_ROOT_PATH);
	}
	return currPathAsStr;
}

}

namespace jny
{

AssetBrowser::AssetBrowser(const Ref<EditorContext>& ctx) : IPanel(ctx)
{
	auto& vfs = Application::subsystems().st<VFS>();
	m_currPath = normalizePath(vfs.rootResourcesPath());
}

void AssetBrowser::update(f32 dt)
{}

void AssetBrowser::updateUI()
{
	if (ImGui::Begin("Asset Browser"))
	{
		drawToolbar();
		drawContent();
		ImGui::End();
	}
}

void AssetBrowser::drawToolbar()
{
	constexpr ImVec2 C_TABLE_SIZE = { 0.0f, 20.0f };

	if (ImGui::BeginTable("##ABToolbar", 2, 0, C_TABLE_SIZE * ImGui::GetWindowDpiScale()))
	{
		ImGui::TableSetupColumn("##buttonBack", ImGuiTableColumnFlags_WidthFixed, 40.0f * ImGui::GetWindowDpiScale());
		ImGui::TableSetupColumn("##currPath", ImGuiTableColumnFlags_WidthStretch);

		std::string vfsPath = convertToVfsPath(m_currPath);
		{
			ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, { 0.0f, 0.0f });
			ImGui::TableNextColumn();
			if (ImGui::Button("Back"))
			{
				if (vfsPath != C_ROOT_PATH)
				{
					m_currPath = m_currPath.parent_path();
				}
			}
			ImGui::PopStyleVar();
		}

		ImGui::TableNextColumn();
		ImGui::Text("%s", vfsPath.c_str());

		ImGui::EndTable();
	}
}

void AssetBrowser::drawContent()
{
	using dir_it = std::filesystem::directory_iterator;
	using dir_recurse_it = std::filesystem::recursive_directory_iterator;

	for (const auto& dirIt : dir_it(m_currPath))
	{
		fs_path path = dirIt.path();
		std::string filename = path.filename().string();
		if (dirIt.is_directory())
		{
			if (ImGui::Button(filename.c_str()))
			{
				m_currPath = normalizePath(path);
			}
		}
		else
		{
			ImGui::Text("%s", filename.c_str());
		}
	}
}

}