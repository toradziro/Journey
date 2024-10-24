#include "EntityProperties.h"

#include "Journey/Scene/Components.h"
#include "Journey/Scene/Entity.h"

#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace jny
{

template<typename T>
void drawComponent(entt::id_type typeId, T& obj)
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
		auto fieldDataType = fieldData.type();

		//-- Type updating handling
		if (fieldDataType == entt::resolve<float>())
		{
			std::string imGuiId = fmt::format("##{}float", propName);

			float val = fieldData.cast<float>();
			if (ImGui::DragFloat(imGuiId.data(), &val, 0.01f))
			{
				data.set(obj, val);
			}

			if (auto prop = data.prop(C_ON_PROP_CHANGE_HS); prop)
			{
				auto onChangeCall = prop.value().cast<std::function<void(T&, f32)>>();
				onChangeCall(obj, val);
			}
		}
		else if (fieldDataType == entt::resolve<bool>())
		{
			std::string imGuiId = fmt::format("##{}bool", propName);

			bool val = fieldData.cast<bool>();
			if (ImGui::Checkbox(imGuiId.data(), &val))
			{
				data.set(obj, val);
			}
		}
		else if (fieldDataType == entt::resolve<std::string>())
		{
			std::string imGuiId = fmt::format("##{}str", propName);

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
		}
		else if (fieldDataType == entt::resolve<glm::vec3>())
		{
			std::string imGuiId = fmt::format("##{}float3", propName);

			glm::vec3 val = fieldData.cast<glm::vec3>();
			ImGui::PushItemWidth(-FLT_MIN);
			if (ImGui::DragFloat3(imGuiId.data(), glm::value_ptr(val), 0.01f, 0.0f, 100.0f))
			{
				data.set(obj, val);
			}
		}
		else if (fieldDataType == entt::resolve<glm::vec4>())
		{
			std::string imGuiId = fmt::format("##{}float4", propName);

			glm::vec4 val = fieldData.cast<glm::vec4>();
			ImGui::PushItemWidth(-FLT_MIN);
			if (ImGui::ColorEdit4(imGuiId.data(), glm::value_ptr(val)))
			{
				data.set(obj, val);
			}
		}
	}
}

template<typename T>
void drawComponent(entt::entity entity, entt::registry& registry, Entity& innerEntity)
{
	if (registry.all_of<T>(entity))
	{
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		if (ImGui::TreeNodeEx(T::C_COMPONENT_NAME, ImGuiTreeNodeFlags_DefaultOpen))
		{
			entt::id_type typeId = entt::hashed_string::value(T::C_COMPONENT_NAME);
			drawComponent<T>(typeId, innerEntity.component<T>());
			ImGui::TreePop();
		}
	}
}

void drawComponents(Entity& innerEntity, Ref<Scene>	m_scene)
{
	drawComponent<EntityNameComponent>(innerEntity.entityId(), m_scene->registry(), innerEntity);
	drawComponent<TransformComponent>(innerEntity.entityId(), m_scene->registry(), innerEntity);
	drawComponent<SpriteComponent>(innerEntity.entityId(), m_scene->registry(), innerEntity);
	drawComponent<CameraComponent>(innerEntity.entityId(), m_scene->registry(), innerEntity);
}

EntityProperties::EntityProperties(const Ref<EditorContext>& ctx) :
	IPanel(ctx)
{
}

EntityProperties::~EntityProperties()
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