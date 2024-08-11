#include "jnypch.h"
#include "RenderCommand.h"
#include "Journey/Core.h"
#include "Journey/Renderer/OpenGL/OpenGLRendererAPI.h"

namespace jny
{

void RenderCommand::createRenderer(RendererAPI::API api)
{
	switch (api)
	{
	case RendererAPI::API::None:
		JNY_ASSERT(false);
		break;
	case RendererAPI::API::OpenGL:
		m_RendererAPI = std::unique_ptr<RendererAPI>(new OpenGLRendererAPI);
		break;
	default:
		JNY_ASSERT(false);
		break;
	}

	m_RendererAPI->setApi(api);
}

} //-- jny