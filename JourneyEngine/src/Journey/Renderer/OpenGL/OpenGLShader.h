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

private:
	uint32_t	m_rendererId = 0;
};

}