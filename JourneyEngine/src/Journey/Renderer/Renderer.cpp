#include "jnypch.h"
#include "Renderer.h"
#include "Journey/Core.h"

namespace jny
{

Renderer::Renderer(RendererAPI rendererAPI) : m_rendererAPI(rendererAPI)
{
	JNY_ASSERT(m_rendererAPI != RendererAPI::None);
}

} //-- jny