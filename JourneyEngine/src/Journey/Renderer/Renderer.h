#pragma once

#include "Journey/SingletonInterface.h"

namespace jny
{

enum class RendererAPI : uint8_t
{
	None = 0,
	OpenGL
};

class Renderer : ISingleton
{
	JNY_SINGLETON_TYPE(Renderer)
	Renderer(RendererAPI rendererAPI);

public:
	inline RendererAPI API() const { return m_rendererAPI; }

private:
	RendererAPI m_rendererAPI = RendererAPI::None;
};

} //-- jny