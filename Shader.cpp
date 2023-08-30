#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>

Shader::Shader() {}

Shader::Shader(const char* vertexSourcePath, const char* fragmentSourcePath) {
	// Load each shader file source code

	std::string vertexShaderText, fragmentShaderText;
	std::ifstream vertexShaderFile, fragmentShaderFile;
	vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		std::stringstream vertexShaderStream, fragmentShaderStream;

		vertexShaderFile.open(vertexSourcePath);
		vertexShaderStream << vertexShaderFile.rdbuf();
		vertexShaderFile.close();
		vertexShaderText = vertexShaderStream.str();

		fragmentShaderFile.open(fragmentSourcePath);
		fragmentShaderStream << fragmentShaderFile.rdbuf();
		fragmentShaderFile.close();
		fragmentShaderText = fragmentShaderStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "Shader: Failed to read shader source files." << std::endl;
	}

	// Compile the shaders

	int success;
	char infoLog[512];

	unsigned int vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	const char* vertexShaderString = vertexShaderText.c_str();
	glShaderSource(vertexShaderId, 1, &vertexShaderString, nullptr);
	glCompileShader(vertexShaderId);

	glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog);
		std::cout << "Shader: Failed to compile vertex shader.\n" << infoLog << std::endl;
	}

	unsigned int fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fragmentShaderString = fragmentShaderText.c_str();
	glShaderSource(fragmentShaderId, 1, &fragmentShaderString, nullptr);
	glCompileShader(fragmentShaderId);

	glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShaderId, 512, NULL, infoLog);
		std::cout << "Shader: Failed to compile fragment shader.\n" << infoLog << std::endl;
	}

	id = glCreateProgram();
	glAttachShader(id, vertexShaderId);
	glAttachShader(id, fragmentShaderId);
	glLinkProgram(id);

	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(id, 512, NULL, infoLog);
		std::cout << "Shader: Failed to link program.\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);

}

void Shader::use() {
	glUseProgram(id);
}

void Shader::setBool(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setMat4(const std::string& name, glm::mat4 mat) const {
	glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setVec3(const std::string& name, glm::vec3 value) const {
	glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, glm::value_ptr(value));
}
