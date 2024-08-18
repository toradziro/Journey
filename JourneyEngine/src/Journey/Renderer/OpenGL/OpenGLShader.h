#pragma once

#include "Journey/Renderer/Shader.h"

namespace jny
{

class OpenGLShader : public Shader
{
public:
	OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
	virtual ~OpenGLShader();

	virtual void bind() const override;
	virtual void unbind() const override;

	virtual void uploadUniformMat4(const glm::mat4& matrix, std::string_view name) override;
	virtual void uploadUniformVec3(const glm::vec3& vector, std::string_view name) override;

private:
	uint32_t	m_rendererId = 0;
};

}