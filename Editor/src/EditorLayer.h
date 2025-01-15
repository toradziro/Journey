#pragma once

#include "Journey.h"
#include "Panels/Panel.h"
#include "Context.h"
#include <ImGuizmo.h>

struct ImVec2;

namespace jny
{

struct GizmoData
{
	ImGuizmo::OPERATION	m_gizmoType = ImGuizmo::OPERATION::TRANSLATE;
	ImGuizmo::MODE		m_coordinateType = ImGuizmo::MODE::LOCAL;
};

enum class SceneMode : u8
{
	Game,
	Editor
};

class EditorLayer : public Layer
{
public:
	EditorLayer(Application* app);

	virtual void attach() override;
	virtual void detach() override;

	void update(f32 dt) override;
	void onEvent(Event& event) override;
	void imGuiRender() override;

	void openSceneUI();
	void saveSceneAs();
	void newScene();

private:
	void openScene(std::string scenePath);

	void saveSceneUI();
	void loadSceneUI();

	void drawMenuBar();
	void drawStats();
	void drawGizmos();
	void drawViewportToolbar();

	void setSelectMode();
	void setTranslateGizmo();
	void setRotateGizmo();
	void setScaleGizmo();

	void switchToGameMode();
	void switchToEditorMode();

private:
	Ref<EditorContext>				m_context;
	std::vector<IPanel*>			m_panels;

	glm::vec2						m_sampledTexture = { 2.0f, 3.0f };
	ImVec2							m_viewportSize = { 0.0f, 0.0f };

	std::pair<ImVec2, ImVec2>		m_viewportBounds = {};

	Ref<Framebuffer>				m_framebuffer;
	Application*					m_app;

	GizmoData						m_gizmoData;
	bool							m_showGizmo = false;

	u32								m_frambufferIndex = 0;
	u32								m_frambufferPickingIndex = 0;

	f32								m_FPS = 0.0f;
	f32								m_dt = 0.0f;
	bool							m_openDemo = false;
	bool							m_viewportActive = false;
	//-- TODO: move to other class
	bool							m_saveScene = false;
	bool							m_loadSceneUI = false;
	std::string						m_sceneFilename = "";

	SceneMode						m_sceneMode = SceneMode::Editor;
};

}