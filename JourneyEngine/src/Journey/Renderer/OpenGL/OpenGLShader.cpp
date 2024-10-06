#include "jnypch.h"
#include "OpenGLShader.h"
#include "Journey/Log/Log.h"
#include "Journey/Core/Application.h"
#include "Journey/Core/fs/VirtualFileSystem.h"

#include <glm/gtc/type_ptr.hpp>

namespace jny
{

namespace
{

constexpr GLenum C_INVALID_TYPE = std::numeric_limits<GLenum>::max();

GLenum stringToGlType(const std::string& line)
{
	constexpr std::string_view C_VERTEX = "vertex";
	constexpr std::string_view C_FRAGMENT = "fragment";

	if (line.find(C_VERTEX) != std::string::npos)
	{
		return GL_VERTEX_SHADER;
	}
	else if (line.find(C_FRAGMENT) != std::string::npos)
	{
		return GL_FRAGMENT_SHADER;
	}

	JNY_ASSERT(false, "Unknown shader type");

	return C_INVALID_TYPE;
}

std::string glTypeToStr(GLuint type)
{
	constexpr const char* C_VERTEX_SHADER_TYPE = "Vertex";
	constexpr const char* C_FRAGMENT_SHADER_TYPE = "Fragment";
	constexpr const char* C_UNKNOWN_SHADER_TYPE = "Unknown";

	if (type == GL_VERTEX_SHADER)
	{
		return C_VERTEX_SHADER_TYPE;
	}
	else if (type == GL_FRAGMENT_SHADER)
	{
		return C_FRAGMENT_SHADER_TYPE;
	}
	return C_UNKNOWN_SHADER_TYPE;
}

} //-- unnamed

OpenGLShader::OpenGLShader(const std::string& path)
	: m_path(path)
{
	Ref<File> shaderSource = Application::subsystems().st<VFS>().loadFile(path);
	std::string sourceAsStr = shaderSource->toString();
	auto splitSources = preprocess(sourceAsStr);
	compile(splitSources);
}

OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	: m_path(name)
{
	ShaderSources sources;
	sources[GL_VERTEX_SHADER] = vertexSrc;
	sources[GL_FRAGMENT_SHADER] = fragmentSrc;
	compile(sources);
}

OpenGLShader::~OpenGLShader()
{
	glDeleteProgram(m_rendererId);
}

void OpenGLShader::bind() const
{
	glUseProgram(m_rendererId);
}

void OpenGLShader::unbind() const
{
	glUseProgram(0);
}

OpenGLShader::ShaderSources OpenGLShader::preprocess(const std::string& source)
{
	constexpr std::string_view C_WIN_LINEBREAK = "\r\n";
	constexpr std::string_view C_LIN_LINEBREAK = "\n";
	constexpr std::string_view C_TOKEN = "#type";

	ShaderSources shaderSources;

	size_t pos = 0;
	std::string currShader;
	GLenum currShaderType = C_INVALID_TYPE;

	std::string_view linebreak = C_WIN_LINEBREAK;
	if (source.find(C_WIN_LINEBREAK) == std::string::npos)
	{
		linebreak = C_LIN_LINEBREAK;
	}

	while (pos < source.size() && pos != std::string::npos)
	{
		size_t lineEnd = source.find(linebreak, pos);
		std::string line = source.substr(pos, lineEnd - pos);

		if (line.find(C_TOKEN) != std::string::npos)
		{
			if (currShaderType != C_INVALID_TYPE)
			{
				shaderSources[currShaderType] = currShader;
			}
			currShaderType = stringToGlType(line);
			currShader = {};
		}
		else
		{
			currShader += line;
		}
		currShader += linebreak;
		pos = lineEnd + linebreak.size();

		//-- Process last parsed shader
		if ((pos == source.size() || lineEnd == std::string::npos) && !currShader.empty() && currShaderType != C_INVALID_TYPE)
		{
			shaderSources[currShaderType] = currShader;
			break;
		}
	}

	return shaderSources;
}

void OpenGLShader::compile(const OpenGLShader::ShaderSources& sources)
{
	std::vector<GLuint> shaders;
	shaders.reserve(sources.size());

	//-- Compile all sources
	std::ranges::for_each(sources, [&](auto& source)
		{
			auto& [sType, shaderSourceCode] = source;
			//-- Create an empty vertex shader handle
			GLuint shader = glCreateShader(sType);

			//-- Send the shader source code to GL
			const GLchar* toGlTypeShader = static_cast<const GLchar*>(shaderSourceCode.c_str());
			glShaderSource(shader, 1, &toGlTypeShader, 0);

			//-- Compile the vertex shader
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				//-- The maxLength includes the NULL character
				char* infoLog = (char*)alloca(maxLength * sizeof(char));
				glGetShaderInfoLog(shader, maxLength, &maxLength, infoLog);

				//-- We don't need the shader anymore.
				glDeleteShader(shader);

				Log::error("{}", infoLog);
				JNY_ASSERT(false, "Shader compilation error. Shader type: {}", glTypeToStr(sType));
			}
			shaders.push_back(shader);
		});

	//-- Shaders are successfully compiled.
	//-- Now time to link them together into a program.
	//-- Get a program object.
	m_rendererId = glCreateProgram();

	//-- Attach our shaders to our program
	std::ranges::for_each(shaders, [&](GLuint shader)
		{
			glAttachShader(m_rendererId, shader);
		});
	
	//-- Link our program
	glLinkProgram(m_rendererId);

	//-- Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(m_rendererId, GL_LINK_STATUS, (int*)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(m_rendererId, GL_INFO_LOG_LENGTH, &maxLength);

		//-- The maxLength includes the NULL character
		char* infoLog = (char*)alloca(maxLength * sizeof(char));
		glGetProgramInfoLog(m_rendererId, maxLength, &maxLength, &infoLog[0]);

		//-- We don't need the program anymore.
		glDeleteProgram(m_rendererId);
		//-- Don't leak shaders either.
		std::ranges::for_each(shaders, [&](GLuint shader)
			{
				glDeleteShader(shader);
			});

		Log::error("{}", infoLog);
		JNY_ASSERT(false, "Shader linking error");
	}

	//-- Always detach shaders after a successful link.
	std::ranges::for_each(shaders, [&](GLuint shader)
		{
			glDetachShader(m_rendererId, shader);
			glDeleteShader(shader);
		});
}

void OpenGLShader::uploadUniformInt(const int value, std::string_view name) const
{
	int32_t location = glGetUniformLocation(m_rendererId, name.data());

	JNY_ASSERT(location != -1, "invalid location");

	glUniform1i(location, value);
}

void OpenGLShader::uploadUniformIntArray(const int32_t* values, uint32_t count, std::string_view name) const
{
	int32_t location = glGetUniformLocation(m_rendererId, name.data());

	JNY_ASSERT(location != -1, "invalid location");

	glUniform1iv(location, count, values);
}

void OpenGLShader::uploadUniformFloat(const float value, std::string_view name) const
{
	int32_t location = glGetUniformLocation(m_rendererId, name.data());

	JNY_ASSERT(location != -1, "invalid location");

	glUniform1f(location, value);
}

void OpenGLShader::uploadUniformFloat2(const glm::vec2& value, std::string_view name) const
{
	int32_t location = glGetUniformLocation(m_rendererId, name.data());

	JNY_ASSERT(location != -1, "invalid location");

	glUniform2fv(location, 1, glm::value_ptr(value));
}

void OpenGLShader::uploadUniformFloat3(const glm::vec3& value, std::string_view name) const
{
	int32_t location = glGetUniformLocation(m_rendererId, name.data());

	JNY_ASSERT(location != -1, "invalid location");

	glUniform3fv(location, 1, glm::value_ptr(value));
}

void OpenGLShader::uploadUniformFloat4(const glm::vec4& value, std::string_view name) const
{
	int32_t location = glGetUniformLocation(m_rendererId, name.data());

	JNY_ASSERT(location != -1, "invalid location");

	glUniform4fv(location, 1, glm::value_ptr(value));
}

void OpenGLShader::uploadUniformMat3(const glm::mat3& value, std::string_view name) const
{
	int32_t location = glGetUniformLocation(m_rendererId, name.data());

	JNY_ASSERT(location != -1, "invalid location");

	glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void OpenGLShader::uploadUniformMat4(const glm::mat4& value, std::string_view name) const
{
	int32_t location = glGetUniformLocation(m_rendererId, name.data());

	JNY_ASSERT(location != -1, "invalid location");

	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

}
