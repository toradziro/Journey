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

	glm::vec3							m_squareColor = { 0.2f, 0.3f, 0.8f };

	jny::Ref<jny::Shader>				m_shader;
	jny::Ref<jny::Texture2D>			m_bombTexture;

	jny::Ref<jny::VertexArray>			m_vertexArray;
};

