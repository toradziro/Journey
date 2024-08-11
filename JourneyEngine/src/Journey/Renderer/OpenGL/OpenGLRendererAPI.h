#pragma once

#include "Journey/Renderer/RendererAPI.h"

namespace jny
{

class OpenGLRendererAPI : public RendererAPI
{
public:

	virtual void clear() override;
	virtual void setClearColor(const glm::vec4& color) override;

	virtual void drawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
};

} //-- jny
