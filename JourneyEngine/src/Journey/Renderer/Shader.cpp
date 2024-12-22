#include "jnypch.h"
#include "Shader.h"
#include "Journey/Core/Application.h"
#include "Journey/Renderer/Renderer2D.h"
#include "Journey/Renderer/OpenGL/OpenGLShader.h"

namespace jny
{

Shader* Shader::create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
{
	const auto api = Application::subsystems().st<RenderCommand>().api();
	Shader* shader = nullptr;

	switch (api)
	{
		case RendererAPI::API::OpenGL:
			shader = new OpenGLShader(name, vertexSrc, fragmentSrc);
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
	const auto api = Application::subsystems().st<RenderCommand>().api();
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

void ShaderLibrary::add(const Ref<Shader>& shader)
{
	JNY_ASSERT(m_shaders.count(shader->path()) == 0, "Shader with this path already exists");

	m_shaders[shader->path()] = shader;
}

jny::Ref<jny::Shader> ShaderLibrary::load(const std::string& path)
{
	JNY_ASSERT(m_shaders.count(path) == 0, "Shader with this path already exists");

	m_shaders[path] = Shader::create(path);
	return m_shaders[path];
}

jny::Ref<jny::Shader> ShaderLibrary::shader(const std::string& path)
{
	JNY_ASSERT(m_shaders.count(path) == 1, "Unknown shader");

	return m_shaders.at(path);
}

} //-- jny