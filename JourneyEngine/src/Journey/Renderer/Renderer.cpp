#include "jnypch.h"
#include "Renderer.h"
#include "Journey/Core.h"
#include "Journey/Application.h"
#include "VertexArray.h"

namespace jny
{

void Renderer::beginScene()
{
}

void Renderer::endScene()
{
}

void Renderer::submit(const std::shared_ptr<VertexArray>& vertexArray)
{
	Application::subsystems().st<RenderCommand>().drawIndexed(vertexArray);
}

void Renderer::setClearColor(const glm::vec4& color)
{
	Application::subsystems().st<RenderCommand>().setClearColor(color);
}

void Renderer::clear()
{
	Application::subsystems().st<RenderCommand>().clear();
}

jny::RendererAPI::API Renderer::API()
{
	return Application::subsystems().st<RenderCommand>().api();
}

} //-- jny