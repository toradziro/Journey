#pragma once

#include "Journey/SingletonInterface.h"
#include "RenderCommand.h"
#include "OrthographicCamera.h"
#include "Shader.h"

namespace jny
{

class Renderer : ISingleton
{
	JNY_SINGLETON_TYPE(Renderer)
	Renderer() = default;

public:
	void beginScene(const std::shared_ptr<OrthographicCamera>& camera);
	void endScene();

	void submit(const std::shared_ptr<VertexArray>& vertexArray,
		const std::shared_ptr<Shader>& shader,
		const glm::mat4& objTransform = glm::mat4(1.0f)) const;

	void setClearColor(const glm::vec4& color);
	void clear();

	RendererAPI::API API();

private:
	struct SceneData
	{
		glm::mat4 m_vpMatrix = {};
	};

	SceneData	m_sceneData = {};
};

} //-- jny