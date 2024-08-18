#pragma once

#include "glm/glm.hpp"

namespace jny
{

class Shader
{
public:
	virtual ~Shader() {}

	virtual void bind() const = 0;
	virtual void unbind() const = 0;

	virtual void uploadUniformMat4(const glm::mat4& matrix, std::string_view name) = 0;
	virtual void uploadUniformVec3(const glm::vec3& vector, std::string_view name) = 0;

	static Shader* create(const std::string& vertexSrc, const std::string& fragmentSrc);
};

} //-- jny