#pragma once

#include <glm/glm.hpp>
#include "VertexArray.h"
#include "Journey/SingletonInterface.h"

namespace jny
{

class RendererAPI
{
public:
	virtual ~RendererAPI() {}

	enum class API : uint8_t
	{
		None = 0,
		OpenGL
	};

	virtual void clear() = 0;
	virtual void setClearColor(const glm::vec4& color) = 0;

	virtual void drawIndexed(const Ref<VertexArray>& vertexArray) = 0;

	inline API api() const { return m_API; }
	void setApi(API api) { m_API = api; }

private:
	API m_API;
};

} //-- jny