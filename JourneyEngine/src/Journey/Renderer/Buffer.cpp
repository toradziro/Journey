#include "jnypch.h"
#include "Buffer.h"
#include "Journey/Core.h"
#include "Journey/Renderer/OpenGL/OpenGLBuffer.h"
#include "Journey/Application.h"
#include "Journey/Renderer/Renderer.h"

namespace jny
{

VertexBuffer* VertexBuffer::create(float* vertices, uint32_t count)
{
	auto rendererApi = Application::subsystems().st<Renderer>().API();
	VertexBuffer* buffer = nullptr;

	switch (rendererApi)
	{
		case RendererAPI::None:
			JNY_ASSERT(true);
			break;
		case RendererAPI::OpenGL:
			buffer = new OpenGLVertexBuffer(vertices, count);
			break;
		default:
			break;
	}

	return buffer;
}

IndexBuffer* IndexBuffer::create(uint32_t* indices, uint32_t count)
{
	auto rendererApi = Application::subsystems().st<Renderer>().API();
	IndexBuffer* buffer = nullptr;

	switch (rendererApi)
	{
		case RendererAPI::None:
			JNY_ASSERT(true);
			break;
		case RendererAPI::OpenGL:
			buffer = new OpenGLIndexBuffer(indices, count);
			break;
		default:
			break;
	}

	return buffer;
}

} //-- jny