#pragma once

#include "Journey.h"
#include "Panels/Panel.h"
#include "Context.h"

struct ImVec2;

namespace jny
{

class EditorLayer : public Layer
{
public:
	EditorLayer(Application* app);

	virtual void attach() override;
	virtual void detach() override;

	void update(f32 dt) override;
	void onEvent(Event& event) override;
	void imGuiRender() override;

private:
	void saveSceneUI();

private:
	Ref<EditorContext>				m_context;
	std::vector<IPanel*>			m_panels;

	glm::vec2						m_sampledTexture = { 2.0f, 3.0f };
	ImVec2							m_viewportSize = { 0.0f, 0.0f };

	Ref<Framebuffer>				m_framebuffer;
	Application*					m_app;

	Entity							m_sampleE;
	Entity							m_sampleE2;
	Entity							m_cameraE;

	f32								m_FPS = 0.0f;
	f32								m_dt = 0.0f;
	bool							m_openDemo = false;
	bool							m_viewportActive = false;
	//-- TODO: move to other class
	bool							m_saveScene = false;
	std::string						m_sceneFilename = "";
};

}