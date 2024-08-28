#include "jnypch.h"
#include "Shader.h"
#include "Journey/Application.h"
#include "Journey/Renderer/Renderer.h"
#include "Journey/Renderer/OpenGL/OpenGLShader.h"

namespace jny
{

Shader* Shader::create(const std::string& vertexSrc, const std::string& fragmentSrc)
{
	const auto api = Application::subsystems().st<Renderer>().API();
	Shader* shader = nullptr;

	switch (api)
	{
		case RendererAPI::API::OpenGL:
			shader = new OpenGLShader(vertexSrc, fragmentSrc);
			break;
		case RendererAPI::API::None:
			JNY_ASSERT(false, "Can't be none");
			break;
		default:
			JNY_ASSERT(false, "Create valid renderer API");
			break;
	}
	return shader;
}

jny::Shader* Shader::create(const std::string& path)
{
	const auto api = Application::subsystems().st<Renderer>().API();
	Shader* shader = nullptr;

	switch (api)
	{
	case RendererAPI::API::OpenGL:
		shader = new OpenGLShader(path);
		break;
	case RendererAPI::API::None:
		JNY_ASSERT(false, "Can't be none");
		break;
	default:
		JNY_ASSERT(false, "Create valid renderer API");
		break;
	}
	return shader;
}

} //-- jny