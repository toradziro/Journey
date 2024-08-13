#include "jnypch.h"
#include "OpenGLShader.h"
#include "Journey/Log/Log.h"
#include <GLAD/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace jny
{

OpenGLShader::OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc)
{
	// Create an empty vertex shader handle
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// Send the vertex shader source code to GL
	// Note that std::string's .c_str is NULL character terminated.
	const GLchar* source = (const GLchar*)vertexSrc.c_str();
	glShaderSource(vertexShader, 1, &source, 0);

	// Compile the vertex shader
	glCompileShader(vertexShader);

	GLint isCompiled = 0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		char* infoLog = (char*)alloca(maxLength * sizeof(char));
		glGetShaderInfoLog(vertexShader, maxLength, &maxLength, infoLog);

		// We don't need the shader anymore.
		glDeleteShader(vertexShader);

		Log::error("{}", infoLog);
		JNY_ASSERT(false, "Vertex shader compilation error");
	}

	// Create an empty fragment shader handle
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Send the fragment shader source code to GL
	// Note that std::string's .c_str is NULL character terminated.
	source = (const GLchar*)fragmentSrc.c_str();
	glShaderSource(fragmentShader, 1, &source, 0);

	// Compile the fragment shader
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		char* infoLog = (char*)alloca(maxLength * sizeof(char));
		glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, infoLog);

		// We don't need the shader anymore.
		glDeleteShader(fragmentShader);
		// Either of them. Don't leak shaders.
		glDeleteShader(vertexShader);

		Log::error("{}", infoLog);
		JNY_ASSERT(false, "Fragment shader compilation error");
	}

	// Vertex and fragment shaders are successfully compiled.
	// Now time to link them together into a program.
	// Get a program object.
	m_rendererId = glCreateProgram();

	// Attach our shaders to our program
	glAttachShader(m_rendererId, vertexShader);
	glAttachShader(m_rendererId, fragmentShader);

	// Link our program
	glLinkProgram(m_rendererId);

	// Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(m_rendererId, GL_LINK_STATUS, (int*)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(m_rendererId, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		char* infoLog = (char*)alloca(maxLength * sizeof(char));
		glGetProgramInfoLog(m_rendererId, maxLength, &maxLength, &infoLog[0]);

		// We don't need the program anymore.
		glDeleteProgram(m_rendererId);
		// Don't leak shaders either.
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		Log::error("{}", infoLog);
		JNY_ASSERT(false, "Shader linking error");
	}

	// Always detach shaders after a successful link.
	glDetachShader(m_rendererId, vertexShader);
	glDetachShader(m_rendererId, fragmentShader);
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

void OpenGLShader::uploadUniformMat4(const glm::mat4& matrix, std::string_view name)
{
	//glUseProgram(m_rendererId);
	int32_t location = glGetUniformLocation(m_rendererId, name.data());

	JNY_ASSERT(location != -1);

	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

}
