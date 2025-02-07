#include "jnypch.h"
#include "VertexArray.h"
#include "Journey/Core/Application.h"
#include "Journey/Renderer/Renderer2D.h"
#include "Journey/Renderer/OpenGL/OpenGLVertexArray.h"

namespace jny
{

s_ptr<VertexArray> VertexArray::create()
{
	auto rendererApi = Application::subsystems().st<RenderCommand>().api();
	s_ptr<VertexArray> buffer = nullptr;

	switch (rendererApi)
	{
	case RendererAPI::API::None:
		JNY_ASSERT(true, "Can't be None");
		break;
	case RendererAPI::API::OpenGL:
		buffer = std::make_shared<OpenGLVertexArray>();
		break;
	default:
		break;
	}

	return buffer;
}

}