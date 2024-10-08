#include "jnypch.h"
#include "Renderer.h"
#include "Journey/Core/Core.h"
#include "Journey/Core/Application.h"
#include "VertexArray.h"

#include "Journey/Core/Profiling/TimeInstruments.h"

namespace jny
{

void Renderer::init()
{
	PROFILE_FUNC;

	Application::subsystems().st<RenderCommand>().init();
}

void Renderer::windowResized(u32 width, u32 height)
{
	PROFILE_FUNC;

	Application::subsystems().st<RenderCommand>().windowResized(width, height);
}

void Renderer::beginScene(const OrthographicCamera& camera)
{
	PROFILE_FUNC;

	m_sceneData.m_vpMatrix = camera.viewProjectionMatrix();
}

void Renderer::endScene()
{
}

void Renderer::submit(const Ref<VertexArray>& vertexArray,
	const Ref<Shader>& shader,
	const glm::mat4& objTransform) const
{
	PROFILE_FUNC;

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