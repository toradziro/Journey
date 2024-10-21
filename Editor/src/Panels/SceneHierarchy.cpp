#include "jnypch.h"
#include "SceneHierarchy.h"
#include "Journey/Scene/Components.h"
#include "Journey/Scene/Entity.h"

#include <imgui.h>
#include "Context.h"

namespace jny
{

SceneHierarchy::SceneHierarchy(const Ref<EditorContext>& ctx) :
	IPanel(ctx)
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
		entt::registry& registry = ctx()->m_currentScene->registry();
		registry.view<entt::entity>().each([&](entt::entity entity)
			{
				Entity innerEntity(entity, &ctx()->m_currentScene->registry());

				const std::string_view entityName = innerEntity.component<EntityNameComponent>().m_name;
				int flags = ImGuiTreeNodeFlags_OpenOnArrow;
				if (ctx()->m_selectedEntity.entityId() == entity)
				{
					flags |= ImGuiTreeNodeFlags_Selected;
				}
				//-- TODO: change begin to scoped wrapper
				bool nodeExpanded = ImGui::TreeNodeEx(entityName.data(), flags);
				if (ImGui::IsItemClicked())
				{
					if (ctx()->m_selectedEntity == innerEntity)
					{
						ctx()->m_selectedEntity = {};
					}
					else
					{
						ctx()->m_selectedEntity = innerEntity;
					}
				}
				if (nodeExpanded)
				{
					ImGui::TreePop();
				}
			});
	}
	ImGui::End();
}

} //-- jny