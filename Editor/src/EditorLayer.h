#pragma once

#include "Journey.h"

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
	OrthographicCameraController	m_orthoCameraCtrl;

	glm::vec2						m_sampledTexture = { 2.0f, 3.0f };
	ImVec2							m_viewportSize = { 0.0f, 0.0f };

	Ref<Scene>						m_scene;
	Ref<Texture2D>					m_checkerboardTexture;
	Ref<Framebuffer>				m_framebuffer;
	Application*					m_app;

	f32								m_FPS = 0.0f;
	bool							m_tstOpt = false;
	bool							m_viewportActive = false;
};

}