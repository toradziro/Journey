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

	enum class API : uint8_t
	{
		None = 0,
		OpenGL
	};

	virtual void init() = 0;

	virtual void windowResized(uint32_t width, uint32_t height) = 0;

	virtual void clear() = 0;
	virtual void setClearColor(const glm::vec4& color) = 0;

	virtual void drawIndexed(const Ref<VertexArray>& vertexArray, uint32_t count = 0) = 0;

	inline API api() const { return m_API; }
	void setApi(API api) { m_API = api; }

private:
	API m_API;
};

} //-- jny