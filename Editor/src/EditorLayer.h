#pragma once

#include "Journey.h"

struct ImVec2;

namespace jny
{

class EditorLayer : public Layer
{
public:
	EditorLayer();

	virtual void attach() override;
	virtual void detach() override;

	void update(float dt) override;
	void onEvent(Event& event) override;
	void imGuiRender() override;

private:
	OrthographicCameraController	m_orthoCameraCtrl;
	ParticleSystem					m_particleSystem;

	QuadCfg							m_quad;
	QuadCfg							m_backgroundQuad;

	glm::vec2						m_sampledTexture = { 2.0f, 3.0f };
	ImVec2							m_viewportSize = { 0.0f, 0.0f };

	Ref<Texture2D>					m_checkerboardTexture;
	Ref<Framebuffer>				m_framebuffer;

	float							m_FPS = 0.0f;
	bool							m_tstOpt = false;
};

}