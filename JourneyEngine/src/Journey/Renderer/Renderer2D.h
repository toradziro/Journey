#pragma once

#include "Journey/Core/Reference.h"
#include "Journey/Core/SingletonInterface.h"
#include "Journey/Renderer/OrthographicCamera.h"
#include "Journey/Renderer/VertexArray.h"
#include "Journey/Renderer/Shader.h"
#include "Journey/Renderer/Texture.h"

namespace jny
{

class Renderer2D : public ISingleton
{
	JNY_SINGLETON_TYPE(Renderer2D)
public:
	void init();
	void shutdown();

	void beginScene(const OrthographicCamera& camera);
	void endScene();

	//-- Primitives
	void drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
	void drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);

	void drawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor = 1.0f);
	void drawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor = 1.0f);

private:
	Ref<VertexArray>	m_quadVertexArray;
	Ref<Shader>			m_textureShader;
	Ref<Texture2D>		m_whiteTexture;
};

} //-- jny