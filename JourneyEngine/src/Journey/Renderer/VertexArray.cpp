#include "jnypch.h"
#include "VertexArray.h"
#include "Journey/Application.h"
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
	case RendererAPI::None:
		JNY_ASSERT(true);
		break;
	case RendererAPI::OpenGL:
		buffer = new OpenGLVertexArray();
		break;
	default:
		break;
	}

	return buffer;
}

}