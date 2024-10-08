#include "jnypch.h"
#include "Buffer.h"
#include "Journey/Core/Core.h"
#include "Journey/Renderer/OpenGL/OpenGLBuffer.h"
#include "Journey/Core/Application.h"
#include "Journey/Renderer/Renderer.h"

namespace jny
{

VertexBuffer* VertexBuffer::create(float* vertices, u32 count)
{
	auto rendererApi = Application::subsystems().st<Renderer>().API();
	VertexBuffer* buffer = nullptr;

	switch (rendererApi)
	{
		case RendererAPI::API::None:
			JNY_ASSERT(true, "Can't be none");
			break;
		case RendererAPI::API::OpenGL:
			buffer = new OpenGLVertexBuffer(vertices, count);
			break;
		default:
			JNY_ASSERT(true, "Can't be none");
			break;
	}

	return buffer;
}

jny::VertexBuffer* VertexBuffer::create(u32 size)
{
	auto rendererApi = Application::subsystems().st<Renderer>().API();
	VertexBuffer* buffer = nullptr;

	switch (rendererApi)
	{
	case RendererAPI::API::None:
		JNY_ASSERT(true, "Can't be none");
		break;
	case RendererAPI::API::OpenGL:
		buffer = new OpenGLVertexBuffer(size);
		break;
	default:
		JNY_ASSERT(true, "Can't be none");
		break;
	}

	return buffer;
}

IndexBuffer* IndexBuffer::create(u32* indices, u32 count)
{
	auto rendererApi = Application::subsystems().st<Renderer>().API();
	IndexBuffer* buffer = nullptr;

	switch (rendererApi)
	{
		case RendererAPI::API::None:
			JNY_ASSERT(true, "Can't be none");
			break;
		case RendererAPI::API::OpenGL:
			buffer = new OpenGLIndexBuffer(indices, count);
			break;
		default:
			JNY_ASSERT(true, "Can't be none");
			break;
	}

	return buffer;
}

} //-- jny