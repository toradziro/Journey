#include "EntityProperties.h"

#include "Journey/Scene/Components.h"
#include "Journey/Scene/Entity.h"

#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <format>

namespace jny
{

template<typename T>
void drawComponent(entt::id_type typeId, T& obj, entt::entity e)
{
	//-- Get properties of type
	entt::meta_type metaType = entt::resolve(typeId);
	for (auto&& [id, data] : metaType.data())
	{
		//-- Current property
		entt::meta_any fieldData = data.get(obj);

		ImGui::TableNextRow();
		ImGui::TableNextColumn();

		//-- Which name was used to register property
		std::string propName = {};
		if (auto prop = data.prop(C_PROP_NAME_HS); prop)
		{
			propName = prop.value().cast<const char*>();
			ImGui::TextUnformatted(propName.c_str());
		}
		else
		{
			ImGui::TextUnformatted("Unknown prop");
		}

		const char* formatStr = "%0.3f";
		if (auto prop = data.prop(C_FORMAT_STR_HS); prop)
		{
			formatStr = prop.value().cast<const char*>();
		}

		ImGui::TableNextColumn();
		if (auto prop = data.prop(C_CASTOM_UI_DRAW); prop)
		{
			auto customDraw = prop.value().cast<std::function<void(T&, entt::entity)>>();
			ImGui::PushItemWidth(-FLT_MIN);
			customDraw(obj, e);
			ImGui::PopItemWidth();
			continue;
		}
		auto fieldDataType = fieldData.type();

		//-- Type updating handling
		if (fieldDataType == entt::resolve<float>())
		{
			std::string imGuiId = fmt::format("##{}{}float", propName, static_cast<u32>(e));

			float val = fieldData.cast<float>();
			bool valueChanged = ImGui::DragFloat(imGuiId.data(), &val, 0.1f);
			if (valueChanged)
			{
				if (auto prop = data.prop(C_ON_PROP_CHANGE_HS); prop)
				{
					auto onChangeCall = prop.value().cast<std::function<void(T&, f32)>>();
					onChangeCall(obj, val);
				}
				else
				{
					data.set(obj, val);
				}
			}
		}
		else if (fieldDataType == entt::resolve<bool>())
		{
			const std::string imGuiId = fmt::format("##{}{}bool", propName, static_cast<u32>(e));

			bool val = fieldData.cast<bool>();
			if (ImGui::Checkbox(imGuiId.data(), &val))
			{
				data.set(obj, val);
			}
		}
		else if (fieldDataType == entt::resolve<std::string>())
		{
			const std::string imGuiId = fmt::format("##{}{}str", propName, static_cast<u32>(e));

			std::string& val = fieldData.cast<std::string&>();
			const i32 C_BUF_LENGTH = 1024;

			char buff[C_BUF_LENGTH];
			memset(buff, 0, C_BUF_LENGTH);
			//-- No use strcpy since copiler considers in unsafe
			for (u32 i = 0; i < val.size(); ++i)
			{
				buff[i] = val[i];
			}

			ImGui::PushItemWidth(-FLT_MIN);
			if (ImGui::InputText(imGuiId.c_str(), buff, C_BUF_LENGTH))
			{
				val = std::string(buff);
				data.set(obj, val);
			}
			ImGui::PopItemWidth();
		}
		else if (fieldDataType == entt::resolve<glm::vec3>())
		{
			const std::string imGuiIdX = fmt::format("##{}{}float3x", propName, static_cast<u32>(e));
			const std::string imGuiIdY = fmt::format("##{}{}float3y", propName, static_cast<u32>(e));
			const std::string imGuiIdZ = fmt::format("##{}{}float3z", propName, static_cast<u32>(e));
			glm::vec3 val = fieldData.cast<glm::vec3>();
			const float lettersSize = ImGui::CalcTextSize("XYZ").x;
			const float itemWidth = (ImGui::GetColumnWidth() / 3.0f) - lettersSize;

			bool valueChanged = false;
			ImGui::AlignTextToFramePadding();
			ImGui::PushStyleColor(ImGuiCol_Text, { 0.7f, 0.0f, 0.0f, 0.9f });
			ImGui::TextUnformatted("X");
			ImGui::PopStyleColor();
			ImGui::SameLine();
			ImGui::PushItemWidth(itemWidth);
			valueChanged |= ImGui::DragFloat(imGuiIdX.c_str(), &val.x, 0.1f);
			ImGui::PopItemWidth();

			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Text, { 0.0f, 0.7f, 0.0f, 0.9f });
			ImGui::TextUnformatted("Y");
			ImGui::PopStyleColor();
			ImGui::SameLine();
			ImGui::PushItemWidth(itemWidth);
			valueChanged |= ImGui::DragFloat(imGuiIdY.c_str(), &val.y, 0.1f);
			ImGui::PopItemWidth();

			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Text, { 0.0f, 0.0f, 0.7f, 0.9f });
			ImGui::TextUnformatted("Z");
			ImGui::PopStyleColor();
			ImGui::SameLine();
			ImGui::PushItemWidth(itemWidth);
			valueChanged |= ImGui::DragFloat(imGuiIdZ.c_str(), &val.z, 0.1f);
			ImGui::PopItemWidth();

			if (valueChanged)
			{
				if (auto prop = data.prop(C_ON_PROP_CHANGE_HS); prop)
				{
					auto onChangeCall = prop.value().cast<std::function<void(T&, glm::vec3)>>();
					onChangeCall(obj, val);
				}
				else
				{
					data.set(obj, val);
				}
			}
		}
		else if (fieldDataType == entt::resolve<glm::vec4>())
		{
			const std::string imGuiId = fmt::format("##{}{}float4", propName, static_cast<u32>(e));

			glm::vec4 val = fieldData.cast<glm::vec4>();
			ImGui::PushItemWidth(-FLT_MIN);
			if (ImGui::ColorEdit4(imGuiId.data(), glm::value_ptr(val)))
			{
				data.set(obj, val);
			}
			ImGui::PopItemWidth();
		}
		else if (fieldDataType == entt::resolve<glm::vec2>())
		{
			const std::string imGuiIdX = fmt::format("##{}{}float2x", propName, static_cast<u32>(e));
			const std::string imGuiIdY = fmt::format("##{}{}float2y", propName, static_cast<u32>(e));
			glm::vec2 val = fieldData.cast<glm::vec2>();
			const float lettersSize = ImGui::CalcTextSize("XY").x;
			const float itemWidth = (ImGui::GetColumnWidth() / 2.0f) - lettersSize;

			bool valueChanged = false;
			ImGui::AlignTextToFramePadding();
			ImGui::PushStyleColor(ImGuiCol_Text, { 0.7f, 0.0f, 0.0f, 0.9f });
			ImGui::TextUnformatted("X");
			ImGui::PopStyleColor();
			ImGui::SameLine();
			ImGui::PushItemWidth(itemWidth);
			valueChanged |= ImGui::DragFloat(imGuiIdX.c_str(), &val.x, 0.1f);
			ImGui::PopItemWidth();

			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Text, { 0.0f, 0.7f, 0.0f, 0.9f });
			ImGui::TextUnformatted("Y");
			ImGui::PopStyleColor();
			ImGui::SameLine();
			ImGui::PushItemWidth(itemWidth);
			valueChanged |= ImGui::DragFloat(imGuiIdY.c_str(), &val.y, 0.1f);
			ImGui::PopItemWidth();

			if (valueChanged)
			{
				if (auto prop = data.prop(C_ON_PROP_CHANGE_HS); prop)
				{
					auto onChangeCall = prop.value().cast<std::function<void(T&, glm::vec2)>>();
					onChangeCall(obj, val);
				}
				else
				{
					data.set(obj, val);
				}
			}
		}
	}
}

template<typename T>
void drawContextMenu(entt::id_type typeId, entt::registry& registry, Entity& innerEntity)
{
	if (ImGui::BeginPopupContextItem("##componentsContextMenu"))
	{
		bool enabled = false;

		//-- Check if component removable
		entt::meta_type metaType = entt::resolve(typeId);
		if (auto prop = metaType.prop(C_PROP_REMOVABLE); prop)
		{
			enabled = true;
		}

		if (ImGui::MenuItem("Remove Component", nullptr, false, enabled))
		{
			innerEntity.removeComponent<T>();
		}
		ImGui::EndPopup();
	}
}

template<typename T>
void drawComponent(entt::registry& registry, Entity& innerEntity)
{
	if (registry.all_of<T>(innerEntity.entityId()))
	{
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		constexpr ImVec2 framePaddingSize = { 1.0f, 0.0f };
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, framePaddingSize);
		const auto flags = ImGuiTreeNodeFlags_SpanAllColumns
			| ImGuiTreeNodeFlags_DefaultOpen
			| ImGuiTreeNodeFlags_OpenOnArrow
			| ImGuiTreeNodeFlags_Framed
			| ImGuiTreeNodeFlags_SpanFullWidth;
		const bool nodePressed = ImGui::TreeNodeEx(T::C_COMPONENT_NAME, flags);
		ImGui::PopStyleVar();

		if (nodePressed)
		{
			entt::id_type typeId = entt::hashed_string::value(T::C_COMPONENT_NAME);
			drawContextMenu<T>(typeId, registry, innerEntity);
			if (innerEntity.hasComponent<T>())
			{
				drawComponent<T>(typeId, innerEntity.component<T>(), innerEntity.entityId());
			}
			ImGui::TreePop();
		}
	}
}

template<typename T>
void drawAddComponent(Entity& selectedE)
{
	std::string resName = std::format("{} Component", T::C_COMPONENT_NAME);
	if (!selectedE.hasComponent<T>() && ImGui::Selectable(resName.c_str()))
	{
		selectedE.addComponent<T>();
	}
}

void drawComponents(Entity& innerEntity, s_ptr<Scene> m_scene)
{
	drawComponent<UuidComponent>(m_scene->registry(), innerEntity);
	drawComponent<EntityNameComponent>(m_scene->registry(), innerEntity);
	drawComponent<TransformComponent>(m_scene->registry(), innerEntity);
	drawComponent<SpriteComponent>(m_scene->registry(), innerEntity);
	drawComponent<CameraComponent>(m_scene->registry(), innerEntity);
	drawComponent<MainHeroComponent>(m_scene->registry(), innerEntity);
	drawComponent<RigidBodyComponent>(m_scene->registry(), innerEntity);
	drawComponent<BoxColliderComponent>(m_scene->registry(), innerEntity);
	drawComponent<CircleColliderComponent>(m_scene->registry(), innerEntity);
	drawComponent<CircleComponent>(m_scene->registry(), innerEntity);
	drawComponent<LineComponent>(m_scene->registry(), innerEntity);
}

EntityProperties::EntityProperties(const s_ptr<EditorContext>& ctx) :
	IPanel(ctx)
{
}

void EntityProperties::update(f32 dt)
{
}

void EntityProperties::updateUI()
{
	if (ImGui::Begin("Entity Properties"))
	{
		if (ctx()->m_selectedEntity)
		{
			constexpr auto tableName = "##conponentsTable";
			constexpr auto tableFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_NoBordersInBodyUntilResize;

			if (ImGui::Button("Add Component"))
			{
				ImGui::OpenPopup("##addComponentPopup");
			}
			if (ImGui::BeginPopup("##addComponentPopup"))
			{
				auto& selectedEntity = ctx()->m_selectedEntity;

				drawAddComponent<SpriteComponent>(selectedEntity);
				drawAddComponent<CameraComponent>(selectedEntity);
				drawAddComponent<MainHeroComponent>(selectedEntity);
				drawAddComponent<RigidBodyComponent>(selectedEntity);
				drawAddComponent<BoxColliderComponent>(selectedEntity);
				drawAddComponent<CircleColliderComponent>(selectedEntity);
				drawAddComponent<CircleComponent>(selectedEntity);
				drawAddComponent<LineComponent>(selectedEntity);

				ImGui::EndPopup();
			}

			if (ImGui::BeginTable(tableName, 2, tableFlags, ImGui::GetContentRegionAvail()))
			{
				drawComponents(ctx()->m_selectedEntity, ctx()->m_currentScene);
				ImGui::EndTable();
			}
		}
		else
		{
			constexpr const char* C_NOTHING_TO_SHOW = "No entities selected";
			ImVec2 windowSize = ImGui::GetWindowSize();
			ImVec2 textSize = ImGui::CalcTextSize(C_NOTHING_TO_SHOW);

			ImVec2 textPosition;
			textPosition.x = (windowSize.x - textSize.x) / 2.0f;
			textPosition.y = (windowSize.y - textSize.y) / 2.0f;

			ImGui::SetCursorPos(textPosition);

			ImGui::Text("%s", C_NOTHING_TO_SHOW);
		}
		ImGui::End();
	}
}

} //-- jny