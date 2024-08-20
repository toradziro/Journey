#pragma once

#include "glm/glm.hpp"

namespace jny
{

class Shader : public ReferenceCounter
{
public:
	virtual ~Shader() = default;

	virtual void bind() const = 0;
	virtual void unbind() const = 0;

	virtual void uploadUniformInt(const int value, std::string_view name) const = 0;

	virtual void uploadUniformFloat(const float value, std::string_view name) const = 0;
	virtual void uploadUniformFloat2(const glm::vec2& value, std::string_view name) const = 0;
	virtual void uploadUniformFloat3(const glm::vec3& value, std::string_view name) const = 0;
	virtual void uploadUniformFloat4(const glm::vec4& value, std::string_view name) const = 0;

	virtual void uploadUniformMat3(const glm::mat3& value, std::string_view name) const = 0;
	virtual void uploadUniformMat4(const glm::mat4& value, std::string_view name) const = 0;

	static Shader* create(const std::string& vertexSrc, const std::string& fragmentSrc);
};

} //-- jny