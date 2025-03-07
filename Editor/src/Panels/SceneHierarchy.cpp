#include "jnypch.h"
#include "SceneHierarchy.h"
#include "Journey/Scene/Components.h"
#include "Journey/Scene/Entity.h"

#include <imgui.h>
#include "Context.h"

namespace jny
{

SceneHierarchy::SceneHierarchy(const s_ptr<EditorContext>& ctx) :
	IPanel(ctx)
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
		drawContextMenu();

		entt::registry& registry = ctx()->m_currentScene->registry();
		registry.view<entt::entity>().each([&](entt::entity entity)
			{
				Entity innerEntity(entity, ctx()->m_currentScene.get());

				const std::string_view entityName = innerEntity.component<EntityNameComponent>().m_name;
				int flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanFullWidth;
				if (ctx()->m_selectedEntity.entityId() == entity)
				{
					flags |= ImGuiTreeNodeFlags_Selected;
				}
				//-- TODO: change begin to scoped wrapper
				bool nodeExpanded = ImGui::TreeNodeEx(entityName.empty() ? "_" : entityName.data(), flags);
				if (ImGui::IsItemClicked(ImGuiMouseButton_Left) || ImGui::IsItemClicked(ImGuiMouseButton_Right))
				{
					selectEntity(innerEntity);
				}
				if (ImGui::IsKeyPressed(ImGuiKey_Escape))
				{
					resetSelection();
				}
				if (nodeExpanded)
				{
					ImGui::TreePop();
				}
			});
	}
	ImGui::End();
}

void SceneHierarchy::drawContextMenu()
{
	const ImVec2 regionMin = ImGui::GetWindowContentRegionMin();
	const ImVec2 regionMax = ImGui::GetWindowContentRegionMax();
	const ImVec2 mousePos = ImGui::GetMousePos();

	if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsWindowHovered())
	{
		resetSelection();
		ImGui::OpenPopup("##contextMenu");
	}
	if (ImGui::BeginPopupContextItem("##contextMenu"))
	{
		const bool anyEntitySelected = ctx()->m_selectedEntity.entityId() != entt::null;
		if (anyEntitySelected)
		{
			if (ImGui::MenuItem("Remove Entity"))
			{
				ctx()->m_currentScene->removeEntity(ctx()->m_selectedEntity);
				ctx()->m_selectedEntity = {};
			}
		}
		else
		{
			if (ImGui::MenuItem("New Entity"))
			{
				auto& scene = ctx()->m_currentScene;
				auto newEntity = scene->createEntity();
				newEntity.component<EntityNameComponent>().m_name = "New entity";
				selectEntity(newEntity);
			}
		}
		ImGui::EndPopup();
	}
}

void SceneHierarchy::resetSelection()
{
	ctx()->m_selectedEntity = {};
}

void SceneHierarchy::selectEntity(Entity e)
{
	ctx()->m_selectedEntity = e;
}

} //-- jny