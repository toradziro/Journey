#pragma once

#include "Journey/Renderer/RendererAPI.h"

namespace jny
{

class OpenGLRendererAPI : public RendererAPI
{
public:
	virtual void init() override;

	virtual void windowResized(u32 width, u32 height) override;

	virtual void clear() override;
	virtual void setClearColor(const glm::vec4& color) override;
	virtual void clearRedIntTexture(u32 textureIndex, i32 value) override;

	virtual void drawIndexed(const s_ptr<VertexArray>& vertexArray, u32 count = 0) override;
	virtual void drawLines(const s_ptr<VertexArray>& vertexArray, u32 count = 0) override;
};

} //-- jny
