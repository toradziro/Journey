#include "jnypch.h"
#include "AssetBrowser.h"
#include "Journey/Core/Application.h"
#include <imgui.h>

namespace
{

std::string convertToVfsPath(jny::fs_path path)
{
	std::string currPathAsStr = path.string();
	u64 rootIt = currPathAsStr.find(jny::VFS::C_RESOURCE_DIR_NAME);
	currPathAsStr = currPathAsStr.substr(rootIt, /*rootIt + jny::VFS::C_RESOURCE_DIR_NAME.size()*/currPathAsStr.size());
	return currPathAsStr;
}

}

namespace jny
{

AssetBrowser::AssetBrowser(const Ref<EditorContext>& ctx) : IPanel(ctx)
{
	auto& vfs = Application::subsystems().st<VFS>();
	m_currPath = vfs.rootResourcesPath();
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

	if (ImGui::BeginTable("##ABToolbar", 3, 0, C_TABLE_SIZE * ImGui::GetWindowDpiScale()))
	{
		ImGui::TableSetupColumn("##buttonBack", ImGuiTableColumnFlags_WidthFixed, 40.0f * ImGui::GetWindowDpiScale());
		ImGui::TableSetupColumn("##buttonFront", ImGuiTableColumnFlags_WidthFixed, 40.0f * ImGui::GetWindowDpiScale());
		ImGui::TableSetupColumn("##currPath", ImGuiTableColumnFlags_WidthStretch);

		{
			ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, { 0.0f, 0.0f });
			ImGui::TableNextColumn();
			if (ImGui::Button("Back")) {}

			ImGui::TableNextColumn();
			if (ImGui::Button("Front")) {}
			ImGui::PopStyleVar();
		}

		ImGui::TableNextColumn();
		std::string vfsPath = convertToVfsPath(m_currPath);
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
		std::string vfsPath = convertToVfsPath(path);
		ImGui::Text("%s", vfsPath.c_str());
	}
}

}