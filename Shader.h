#pragma once

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>

class Shader {
public:

	// Id of the corresponding shader program
	unsigned int id;

	Shader();
	Shader(const char* vertexSourcePath, const char* fragmentSourcePath);

	void use();
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setMat4(const std::string& name, glm::mat4 mat) const;
	void setVec3(const std::string& name, glm::vec3 value) const;

};

