#include "jnypch.h"
#include "Renderer.h"
#include "Journey/Core.h"
#include "Journey/Application.h"
#include "VertexArray.h"

namespace jny
{

void Renderer::beginScene(const std::shared_ptr<OrthographicCamera>& camera)
{
	m_sceneData.m_vpMatrix = camera->viewProjectionMatrix();
}

void Renderer::endScene()
{
}

void Renderer::submit(const std::shared_ptr<VertexArray>& vertexArray,
	const std::shared_ptr<Shader>& shader,
	const glm::mat4& objTransform)
{
	shader->bind();
	
	shader->uploadUniformMat4(m_sceneData.m_vpMatrix, "u_vpMatrix");
	shader->uploadUniformMat4(objTransform, "u_modelTransform");

	vertexArray->bind();
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