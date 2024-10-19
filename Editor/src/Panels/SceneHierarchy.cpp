#include "jnypch.h"
#include "SceneHierarchy.h"
#include "Journey/Scene/Components.h"
#include "Journey/Scene/Entity.h"
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace jny
{

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
					ImGui::TableNextColumn();
					const std::string_view entityName = innerEntity.component<EntityNameComponent>().m_name;
					if (ImGui::TreeNode(entityName.data()))
					{
						if (registry.all_of<TransformComponent>(entity))
						{
							auto& transform = innerEntity.component<TransformComponent>().m_transform[3];
							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::TextUnformatted("Transform");
							ImGui::TableNextColumn();
							ImGui::DragFloat3("##TransformComponent", glm::value_ptr(transform), 0.1f);
						}
						ImGui::TreePop();
					}
				});
				ImGui::EndTable();
		}
		ImGui::End();
	}
}

} //-- jny