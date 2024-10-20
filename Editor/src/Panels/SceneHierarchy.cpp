#include "jnypch.h"
#include "SceneHierarchy.h"
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
		ImGui::TableNextColumn();
		auto fieldDataType = fieldData.type();

		//-- Type updating handling
		if (fieldDataType == entt::resolve<float>())
		{
			std::string imGuiId = fmt::format("##{}float", propName);

			float val = fieldData.cast<float>();
			if (ImGui::DragFloat(imGuiId.data(), &val))
			{
				data.set(obj, val);
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
			std::string val = fieldData.cast<std::string>();
			ImGui::TextUnformatted(val.data());
		}
		else if (fieldDataType == entt::resolve<glm::vec3>())
		{
			std::string imGuiId = fmt::format("##{}float3", propName);

			glm::vec3 val = fieldData.cast<glm::vec3>();
			if (ImGui::DragFloat3(imGuiId.data(), glm::value_ptr(val)))
			{
				data.set(obj, val);
			}
		}
		else if (fieldDataType == entt::resolve<glm::vec4>())
		{
			std::string imGuiId = fmt::format("##{}float4", propName);

			glm::vec4 val = fieldData.cast<glm::vec4>();
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
		if (ImGui::TreeNode(T::C_COMPONENT_NAME))
		{
			entt::id_type typeId = entt::hashed_string::value(T::C_COMPONENT_NAME);
			drawComponent<T>(typeId, innerEntity.component<T>());
			ImGui::TreePop();
		}
	}
}

SceneHierarchy::SceneHierarchy(const Ref<Scene>& scene) :
	m_scene(scene)
{
}

SceneHierarchy::~SceneHierarchy()
{
}

void SceneHierarchy::update(f32 dt)
{
	//-- Here we will may want have some logic like selected entity processing
}

void SceneHierarchy::updateUI()
{
	//-- Iterate over all entities, draw entity name,
	//-- than iterate over all of its components and draw them inside a node
	//-- TODO: change begin to scoped wrapper
	if (ImGui::Begin("Scene Hierarchy"))
	{
		constexpr auto tableName = "##conponentsTable";
		constexpr auto tableFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_NoBordersInBodyUntilResize;
		if (ImGui::BeginTable(tableName, 2, tableFlags, ImGui::GetContentRegionAvail()))
		{
			entt::registry& registry = m_scene->registry();
			registry.view<entt::entity>().each([&](entt::entity entity)
				{
					Entity innerEntity(entity, &m_scene->registry());

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					const std::string_view entityName = innerEntity.component<EntityNameComponent>().m_name;
					//-- TODO: change begin to scoped wrapper
					if (ImGui::TreeNode(entityName.data()))
					{
						drawComponent<TransformComponent>(entity, m_scene->registry(), innerEntity);
						drawComponent<SpriteComponent>(entity, m_scene->registry(), innerEntity);
						drawComponent<CameraComponent>(entity, m_scene->registry(), innerEntity);
						ImGui::TreePop();
					}
				});
				ImGui::EndTable();
		}
		ImGui::End();
	}
}

} //-- jny