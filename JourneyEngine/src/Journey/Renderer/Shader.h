#pragma once

#include "glm/glm.hpp"

#include "Journey/Core/SingletonInterface.h"

namespace jny
{

class Shader
{
public:
	virtual ~Shader() = default;

	virtual void bind() const = 0;
	virtual void unbind() const = 0;

	virtual const std::string& path() const = 0;

	virtual void uploadUniformInt(const int value, std::string_view name) const = 0;
	virtual void uploadUniformIntArray(const i32* values, u32 count, std::string_view name) const = 0;

	virtual void uploadUniformFloat(const float value, std::string_view name) const = 0;
	virtual void uploadUniformFloat2(const glm::vec2& value, std::string_view name) const = 0;
	virtual void uploadUniformFloat3(const glm::vec3& value, std::string_view name) const = 0;
	virtual void uploadUniformFloat4(const glm::vec4& value, std::string_view name) const = 0;

	virtual void uploadUniformMat3(const glm::mat3& value, std::string_view name) const = 0;
	virtual void uploadUniformMat4(const glm::mat4& value, std::string_view name) const = 0;

	static s_ptr<Shader> create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
	static s_ptr<Shader> create(const std::string& path);
};

class ShaderLibrary : ISingleton
{
	JNY_SINGLETON_TYPE(ShaderLibrary)

public:
	ShaderLibrary() = default;

	void add(const s_ptr<Shader>& shader);
	s_ptr<Shader> load(const std::string& path);
	s_ptr<Shader> shader(const std::string& path);

private:
	std::unordered_map<std::string, s_ptr<Shader>> m_shaders;
};

} //-- jny