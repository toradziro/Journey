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

	struct ProfileResult
	{
		const char* m_name;
		float		m_time;
	};

private:
	jny::OrthographicCameraController	m_orthoCameraCtrl;

	std::vector<ProfileResult>			m_profileData;

	glm::vec4							m_squareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
	glm::vec2							m_pos = { 0.0f, 0.0f };
	glm::vec2							m_size = { 1.0f, 1.0f };

	jny::Ref<jny::Texture2D>			m_checkerboardTexture;
	jny::Ref<jny::VertexArray>			m_vertexArray;
	jny::Ref<jny::Shader>				m_shader;
};
