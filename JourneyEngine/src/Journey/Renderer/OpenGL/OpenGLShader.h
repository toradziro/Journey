#pragma once

#include "Journey/Renderer/Shader.h"

#include <GLAD/glad.h>

namespace jny
{

class OpenGLShader : public Shader
{
public:
	OpenGLShader(const std::string& path);
	OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
	virtual ~OpenGLShader();

	virtual void bind() const override;
	virtual void unbind() const override;

	//-- Uniforms
	virtual void uploadUniformInt(const int value, std::string_view name) const override;

	virtual void uploadUniformFloat(const float value, std::string_view name) const override;
	virtual void uploadUniformFloat2(const glm::vec2& value, std::string_view name) const override;
	virtual void uploadUniformFloat3(const glm::vec3& value, std::string_view name) const override;
	virtual void uploadUniformFloat4(const glm::vec4& value, std::string_view name) const override;

	virtual void uploadUniformMat3(const glm::mat3& value, std::string_view name) const override;
	virtual void uploadUniformMat4(const glm::mat4& value, std::string_view name) const override;

private:
	using ShaderSources = std::unordered_map<GLenum, std::string>;

	std::string readFile(const std::string& path);
	ShaderSources preprocess(const std::string& source);
	void compile(const ShaderSources& source);

	uint32_t	m_rendererId = 0;
};

}