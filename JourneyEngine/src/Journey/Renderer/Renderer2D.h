#pragma once

#include "Journey/Core/Reference.h"
#include "Journey/Core/SingletonInterface.h"
#include "Journey/Renderer/OrthographicCamera.h"

namespace jny
{

class Renderer2D : public ReferenceCounter, public ISingleton
{
	JNY_SINGLETON_TYPE(Renderer2D);
public:
	void init();
	void shutdown();

	void beginScene(const OrthographicCamera& camera);
	void endScene();

	//-- Primitives
	void drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
	void drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
};

} //-- jny