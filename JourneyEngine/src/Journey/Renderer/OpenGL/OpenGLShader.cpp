#include "jnypch.h"
#include "OpenGLShader.h"
#include "Journey/Log/Log.h"

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

} //-- unnamed

OpenGLShader::OpenGLShader(const std::string& path)
{
	std::string shaderSource = readFile(path);
	auto splitSources = preprocess(shaderSource);
	compile(splitSources);
}

OpenGLShader::OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc)
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

std::string OpenGLShader::readFile(const std::string& path)
{
	std::string result;
	std::ifstream in(path, std::ios::binary);

	if (in)
	{
		//-- move to EOF
		in.seekg(0, std::ios::end);
		//-- reserve space for the file size
		result.resize(in.tellg());
		//-- move back to the start of file
		in.seekg(0, std::ios::beg);
		//-- read a file
		in.read(result.data(), result.size());
		in.close();
	}
	else
	{
		JNY_ASSERT(false, "Can't read shader: '{}'", path);
	}
	
	return result;
}

OpenGLShader::ShaderSources OpenGLShader::preprocess(const std::string& source)
{
	constexpr std::string_view C_LINEBREAK = "\r\n";
	constexpr std::string_view C_TOKEN = "#type";

	ShaderSources shaderSources;

	size_t pos = 0;
	std::string currShader;
	GLenum currShaderType = C_INVALID_TYPE;

	while (pos < source.size() && pos != std::string::npos)
	{
		size_t lineEnd = source.find(C_LINEBREAK, pos);
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
		currShader += C_LINEBREAK;
		pos = lineEnd + C_LINEBREAK.size();

		//-- Process last parsed shader
		if (pos == source.size() && !currShader.empty() && currShaderType != C_INVALID_TYPE)
		{
			shaderSources[currShaderType] = currShader;
		}
	}

	return shaderSources;
}

void OpenGLShader::compile(const OpenGLShader::ShaderSources& sources)
{
	std::vector<GLuint> shaders;

	//-- Compile all sources
	std::ranges::for_each(sources, [&](auto& source)
		{
			//-- Create an empty vertex shader handle
			GLuint shader = glCreateShader(source.first);

			//-- Send the shader source code to GL
			const GLchar* toGlTypeShader = static_cast<const GLchar*>(source.second.c_str());
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
				JNY_ASSERT(false, "Shader compilation error");
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
		});
}

void OpenGLShader::uploadUniformInt(const int value, std::string_view name) const
{
	int32_t location = glGetUniformLocation(m_rendererId, name.data());

	JNY_ASSERT(location != -1, "invalid location");

	glUniform1i(location, value);
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
