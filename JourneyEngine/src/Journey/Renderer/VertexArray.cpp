#include "jnypch.h"
#include "VertexArray.h"
#include "Journey/Core/Application.h"
#include "Journey/Renderer/Renderer.h"
#include "Journey/Renderer/OpenGL/OpenGLVertexArray.h"

namespace jny
{

VertexArray* VertexArray::create()
{
	auto rendererApi = Application::subsystems().st<Renderer>().API();
	VertexArray* buffer = nullptr;

	switch (rendererApi)
	{
	case RendererAPI::API::None:
		JNY_ASSERT(true, "Can't be None");
		break;
	case RendererAPI::API::OpenGL:
		buffer = new OpenGLVertexArray();
		break;
	default:
		break;
	}

	return buffer;
}

}