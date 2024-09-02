#pragma once

#include "Journey/Renderer/RendererAPI.h"

namespace jny
{

class OpenGLRendererAPI : public RendererAPI
{
public:
	virtual void init() override;

	virtual void windowResized(uint32_t width, uint32_t height) override;

	virtual void clear() override;
	virtual void setClearColor(const glm::vec4& color) override;

	virtual void drawIndexed(const Ref<VertexArray>& vertexArray) override;
};

} //-- jny
