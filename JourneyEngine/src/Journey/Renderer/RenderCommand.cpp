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

void RenderCommand::windowResized(u32 width, u32 height)
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
		m_RendererAPI = std::make_unique<OpenGLRendererAPI>();
		break;
	default:
		JNY_ASSERT(false, "Create valid renderer API");
		break;
	}

	m_RendererAPI->setApi(api);
}

} //-- jny