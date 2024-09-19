#pragma once

#include "Journey.h"

class Sandbox2D : public jny::Layer
{
public:
	Sandbox2D();

	virtual void attach() override;
	virtual void detach() override;

	void update(float dt) override;
	void onEvent(jny::Event& event) override;
	void imGuiRender() override;

private:
	jny::OrthographicCameraController	m_orthoCameraCtrl;

	jny::QuadCfg						m_quad;
	jny::QuadCfg						m_backgroundQuad;

	jny::Ref<jny::Texture2D>			m_checkerboardTexture;

	float								m_FPS = 0.0f;
};

