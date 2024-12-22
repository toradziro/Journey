#include "jnypch.h"
#include "Buffer.h"
#include "Journey/Core/Core.h"
#include "Journey/Renderer/OpenGL/OpenGLBuffer.h"
#include "Journey/Core/Application.h"
#include "Journey/Renderer/Renderer2D.h"

namespace jny
{

namespace buff_utils
{

u32 shaderDataTypeSize(ShaderDataType type)
{
	switch (type)
	{
	case ShaderDataType::Bool:		return sizeof(bool);
	case ShaderDataType::Int:		return sizeof(int);
	case ShaderDataType::Int2:		return sizeof(int) * 2;
	case ShaderDataType::Int3:		return sizeof(int) * 3;
	case ShaderDataType::Int4:		return sizeof(int) * 4;
	case ShaderDataType::Float:		return sizeof(float);
	case ShaderDataType::Float2:	return sizeof(float) * 2;
	case ShaderDataType::Float3:	return sizeof(float) * 3;
	case ShaderDataType::Float4:	return sizeof(float) * 4;
		//-- 3x3 matrix
	case ShaderDataType::Mat3:		return sizeof(float) * 3 * 3;
		//-- 4x4 matrix
	case ShaderDataType::Mat4:		return sizeof(float) * 4 * 4;
	default:
		break;
	}

	JNY_ASSERT(false, "Invalid Type");
	return 0;
}

u32 componentsCount(buff_utils::ShaderDataType type)
{
	using namespace buff_utils;

	switch (type)
	{
	case ShaderDataType::Bool: [[fallthrough]];
	case ShaderDataType::Int: [[fallthrough]];
	case ShaderDataType::Float:		return 1;
	case ShaderDataType::Int2: [[fallthrough]];
	case ShaderDataType::Float2:	return 2;
	case ShaderDataType::Int3: [[fallthrough]];
	case ShaderDataType::Float3:	return 3;
	case ShaderDataType::Int4: [[fallthrough]];
	case ShaderDataType::Float4:	return 4;
		//-- 3x3 matrix
	case ShaderDataType::Mat3:		return 3 * 3;
		//-- 4x4 matrix
	case ShaderDataType::Mat4:		return 4 * 4;
	default:
		break;
	}

	JNY_ASSERT(false, "Invalid type");
	return 0;
}

}

VertexBuffer* VertexBuffer::create(float* vertices, u32 count)
{
	auto rendererApi = Application::subsystems().st<RenderCommand>().api();
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
	auto rendererApi = Application::subsystems().st<RenderCommand>().api();
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
	auto rendererApi = Application::subsystems().st<RenderCommand>().api();
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