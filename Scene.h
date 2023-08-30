#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "ColorIterator.h"

class Scene {
public:
	virtual void render(GLFWwindow* window) {}
	virtual void setup(GLFWwindow* window) {}
};

class DvdScene : public Scene {
public:
	virtual void render(GLFWwindow* window) override;
	virtual void setup(GLFWwindow* window) override;
private:
	const float logoWidth = 0.1f;
	const float speed = 1.5f;
	const float minX = -1.0f + logoWidth / 2.0f;
	const float maxX = 1.0f - logoWidth / 2.0f;
	const float minY = -1.0f + logoWidth / 2.0f;
	const float maxY = 1.0f - logoWidth / 2.0f;
	const float aspectRatio = 800.0f / 600.0f;

	unsigned int VAO, VBO, EBO, textureId;
	Shader shader;
	ColorIterator colorIterator;
	glm::vec2 position;
	glm::vec2 direction = glm::vec2(1.0f, aspectRatio) / aspectRatio;
	float previousTime = 0;
};

class RectPacking : public Scene {
public:
	virtual void render(GLFWwindow* window) override;
};
