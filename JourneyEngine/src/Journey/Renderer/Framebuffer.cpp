#include "jnypch.h"
#include "Framebuffer.h"
#include "Journey/Core/Application.h"
#include "Journey/Renderer/Renderer2D.h"
#include "Journey/Renderer/OpenGL/OpenGLFramebuffer.h"

namespace jny
{

Ref<Framebuffer> Framebuffer::create(const FramebufferSpecs& specs)
{
	auto rendererApi = Application::subsystems().st<RenderCommand>().api();
	Ref<Framebuffer> buffer;

	switch (rendererApi)
	{
	case RendererAPI::API::None:
		JNY_ASSERT(true, "Can't be none");
		break;
	case RendererAPI::API::OpenGL:
		buffer = new OpenGLFramebuffer(specs);
		break;
	default:
		JNY_ASSERT(true, "Can't be none");
		break;
	}

	return buffer;
}

}