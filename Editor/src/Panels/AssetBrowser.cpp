#include "jnypch.h"
#include "AssetBrowser.h"
#include "Journey/Core/Application.h"
#include "Journey/ResourceManagers/TextureManager.h"
#include "Journey/Renderer/Texture.h"
#include <imgui.h>

namespace
{

constexpr std::string_view	C_ROOT_PATH = "/";
constexpr std::string_view	C_DIRECTORY_ICON = "editor/icons/directory_icon.png";
constexpr std::string_view	C_TEXTURE_ICON = "editor/icons/texture_icon.png";
constexpr std::string_view	C_TXT_ICON = "editor/icons/txt_icon.png";
constexpr std::string_view	C_SHADER_ICON = "editor/icons/shader_icon.png";
constexpr std::string_view	C_SCENE_ICON = "editor/icons/scene_icon.png";
constexpr std::string_view	C_FONT_ICON = "editor/icons/font_icon.png";
constexpr float				C_BUTTON_SIZE = 96.0f;


enum class ResourceType : u8
{
	Directory,
	Texture,
	Shader,
	Font,
	Text,
	Scene,

	None
};

std::string convertToVfsPath(jny::fs_path path)
{
	std::string currPathAsStr = jny::normalizePath(path);
	u64 rootIt = currPathAsStr.find(jny::VFS::C_RESOURCE_DIR_NAME);
	currPathAsStr = currPathAsStr.substr(rootIt + jny::VFS::C_RESOURCE_DIR_NAME.size(), currPathAsStr.size());
	if (currPathAsStr.empty())
	{
		return std::string(C_ROOT_PATH);
	}
	return currPathAsStr;
}

jny::Ref<jny::Texture2D> loadIcon(ResourceType type)
{
	jny::Ref<jny::Texture2D> res = nullptr;
	auto& tm = jny::Application::subsystems().st<jny::TextureManager>();
	auto& vfs = jny::Application::subsystems().st<jny::VFS>();

	switch (type)
	{
		case ResourceType::Directory:
			res = tm.create(vfs.virtualToNativePath(C_DIRECTORY_ICON).string());
			break;
		case ResourceType::Texture:
			res = tm.create(vfs.virtualToNativePath(C_TEXTURE_ICON).string());
			break;
		case ResourceType::Shader:
			res = tm.create(vfs.virtualToNativePath(C_SHADER_ICON).string());
			break;
		case ResourceType::Font:
			res = tm.create(vfs.virtualToNativePath(C_FONT_ICON).string());
			break;
		case ResourceType::Text:
			res = tm.create(vfs.virtualToNativePath(C_TXT_ICON).string());
			break;
		case ResourceType::Scene:
			res = tm.create(vfs.virtualToNativePath(C_SCENE_ICON).string());
			break;
		case ResourceType::None:
			break;
		default:
			break;
	}

	return res;
}

ResourceType nameToResourceType(std::string extention)
{
	if (extention == ".png")
	{
		return ResourceType::Texture;
	}
	else if (extention == ".txt")
	{
		return ResourceType::Text;
	}
	else if (extention == ".ini")
	{
		return ResourceType::Text;
	}
	else if (extention == ".glsl")
	{
		return ResourceType::Shader;
	}
	//-- TODO: change to scene
	else if (extention == ".yaml")
	{
		return ResourceType::Scene;
	}
	else if (extention == ".ttf")
	{
		return ResourceType::Font;
	}
	return ResourceType::None;
}

using ButtonCallback = std::function<void(void)>;

void drawButtonInGrid(
	ImTextureID		rendererId,
	ButtonCallback	callback,
	std::string		text,
	ResourceType	resType,
	jny::fs_path	fullPath = {})
{
	const ImVec2 buttonSizeWithDpi = ImVec2{ C_BUTTON_SIZE, C_BUTTON_SIZE } *ImGui::GetWindowDpiScale();

	ImGui::PushID(fullPath.string().c_str());

	ImGui::PushStyleColor(ImGuiCol_Button, { 0.0f, 0.0f, 0.0f, 0.0f });
	ImGui::ImageButton
	(
		rendererId,
		buttonSizeWithDpi,
		ImVec2{ 0.0f, 1.0f },
		ImVec2{ 1.0f, 0.0f },
		0
	);

	if (resType != ResourceType::Directory)
	{
		if (ImGui::BeginDragDropSource())
		{
			std::string itemPath = jny::normalizePath(fullPath);
			ImGui::SetDragDropPayload("ASSET_BROWSER_ITEM", itemPath.c_str(), itemPath.size(), ImGuiCond_Always);
			ImGui::EndDragDropSource();
		}
	}

	ImGui::PopStyleColor();

	if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
	{
		callback();
	}

	ImGui::TextWrapped("%s", text.c_str());

	ImGui::PopID();
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

	int column = 0;
	if (ImGui::BeginTable("##ContentGrid", 6, ImGuiTableFlags_SizingFixedFit))
	{
		for (const auto& dirIt : dir_it(m_currPath))
		{
			fs_path path = dirIt.path();
			std::string filename = path.filename().string();

			if (column >= 6)
			{
				ImGui::TableNextRow();
				column = 0;
			}
			ImGui::TableNextColumn();

			if (dirIt.is_directory())
			{
				auto iconTexture = loadIcon(ResourceType::Directory);
				//-- Actual drawing
				u64 iconRendererId = static_cast<u64>(iconTexture->rendererId());
				auto onClickCallback = [&]()
					{
						m_currPath = normalizePath(path);
					};
				drawButtonInGrid(
					reinterpret_cast<void*>(iconRendererId),
					onClickCallback,
					filename,
					ResourceType::Directory
				);
			}
			else
			{
				Ref<Texture2D> iconTexture;
				ResourceType resourceType = nameToResourceType(path.extension().string());
				if (resourceType == ResourceType::Texture)
				{
					auto& tm = jny::Application::subsystems().st<jny::TextureManager>();
					iconTexture = tm.create(path.string());
				}
				else
				{
					iconTexture = loadIcon(resourceType);
				}
				//-- Actual drawing
				u64 iconRendererId = static_cast<u64>(iconTexture->rendererId());
				auto onClickCallback = [&, dirIt]()
					{
						if (resourceType == ResourceType::Scene)
						{
							m_ctx->m_selectedEntity = {};
							m_ctx->m_currentScene = Ref<Scene>::create();
							m_ctx->m_currentScene->deserialize(dirIt.path().filename().string());
						}
					};
				drawButtonInGrid(
					reinterpret_cast<void*>(iconRendererId),
					onClickCallback,
					filename,
					resourceType,
					dirIt.path()
				);
			}
		}
		ImGui::EndTable();
	}
}

}