#include "jnypch.h"
#include "Texture.h"

#include "Journey/Renderer/Renderer.h"
#include "Journey/Renderer/OpenGL/OpenGLTexture.h"
#include "Journey/Core/Application.h"

namespace jny
{

Ref<Texture2D> Texture2D::create(const std::string& texturePath)
{
	auto rendererApi = Application::subsystems().st<Renderer>().API();
	Ref<Texture2D> tex = nullptr;

	switch (rendererApi)
	{
	case RendererAPI::API::None:
		JNY_ASSERT(true, "Can't be None");
		break;
	case RendererAPI::API::OpenGL:
		tex = new OpenGLTexture2D(texturePath);
		break;
	default:
		break;
	}

	return tex;
}

} //-- jny