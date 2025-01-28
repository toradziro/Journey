#pragma once

#include <glm/glm.hpp>
#include "VertexArray.h"
#include "Journey/Core/SingletonInterface.h"

namespace jny
{

class RendererAPI
{
public:
	virtual ~RendererAPI() = default;

	enum class API : u8
	{
		None = 0,
		OpenGL
	};

	virtual void init() = 0;

	virtual void windowResized(u32 width, u32 height) = 0;

	virtual void clear() = 0;
	virtual void setClearColor(const glm::vec4& color) = 0;
	virtual void clearRedIntTexture(u32 textureIndex, i32 value) = 0;

	virtual void drawIndexed(const s_ptr<VertexArray>& vertexArray, u32 count = 0) = 0;

	inline API api() const { return m_API; }
	void setApi(API api) { m_API = api; }

private:
	API m_API;
};

} //-- jny