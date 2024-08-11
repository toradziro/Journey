#pragma once

#include "Journey/SingletonInterface.h"
#include "RenderCommand.h"

namespace jny
{

class Renderer : ISingleton
{
	JNY_SINGLETON_TYPE(Renderer)
	Renderer() = default;

public:
	void beginScene();
	void endScene();

	void submit(const std::shared_ptr<VertexArray>& vertexArray);

	void setClearColor(const glm::vec4& color);
	void clear();

	RendererAPI::API API();
};

} //-- jny