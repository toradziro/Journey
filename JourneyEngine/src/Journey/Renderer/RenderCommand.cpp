#include "jnypch.h"
#include "RenderCommand.h"
#include "Journey/Core/Core.h"
#include "Journey/Renderer/OpenGL/OpenGLRendererAPI.h"

namespace jny
{

void RenderCommand::init()
{
	m_RendererAPI->init();
}

void RenderCommand::windowResized(uint32_t width, uint32_t height)
{
	m_RendererAPI->windowResized(width, height);
}

void RenderCommand::createRenderer(RendererAPI::API api)
{
	switch (api)
	{
	case RendererAPI::API::None:
		JNY_ASSERT(false, "Can't be none");
		break;
	case RendererAPI::API::OpenGL:
		m_RendererAPI = std::unique_ptr<RendererAPI>(new OpenGLRendererAPI);
		break;
	default:
		JNY_ASSERT(false, "Create valid renderer API");
		break;
	}

	m_RendererAPI->setApi(api);
}

} //-- jny