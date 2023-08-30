#include <glad/glad.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#include "stb_image.h"
#include "Scene.h"
#include "Shader.h"

void DvdScene::render(GLFWwindow* window) {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);


	// Update position based on velocity
	float time = (float) glfwGetTime();
	float elapsed = time - previousTime;
	position += speed * elapsed * direction;

	// Check for boundary collosisions
	if (position.x <= minX) {
		position.x = minX;
		direction.x *= -1;
	} else if (position.x >= maxX) {
		position.x = maxX;
		direction.x *= -1;
	}

	if (position.y <= minY) {
		position.y = minY;
		direction.y *= -1;
	} else if (position.y >= maxY) {
		position.y = maxY;
		direction.y *= -1;
	}

	direction = glm::normalize(direction);

	previousTime = time;

	// Set uniforms in shaders
	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::translate(transform, glm::vec3(position.x, position.y, 0.0f));
	shader.setMat4("transform", transform);
	shader.setVec3("foregroundColor", colorIterator.next());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	return;
}

void DvdScene::setup(GLFWwindow* window) {
	shader = Shader{ "dvd_shader.vs", "dvd_shader.fs" };
	glGenTextures(1, &textureId);
	
	const std::string texturePath = "Textures\\dvd_logo.jpg";
	stbi_set_flip_vertically_on_load(true);
	int width, height, numberChannels;
	unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &numberChannels, 0);

	if (!data) {
		std::cout << "Failed to load texture from " << texturePath << std::endl;
		std::exit(-1);
	}

	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	float aspectRatio = 800.0f / 600.0f;
	float xScale = 0.1f;
	float yScale = xScale * aspectRatio;
	// Format: [x, y, z, u, v]
	float vertexData[] = {
		1.0f * xScale, 1.0f * yScale, 0.0f, 1.0f, 1.0f,
		1.0f * xScale, -1.0f * yScale, 0.0f, 1.0f, 0.0f,
		-1.0f * xScale, -1.0f * yScale, 0.0f, 0.0f, 0.0f,
		-1.0f * xScale, 1.0f * yScale, 0.0f, 0.0f, 1.0f
	};

	unsigned int indexData[] = {
		1, 3, 2,
		1, 0, 3
	};

	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData), indexData, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	shader.use();
	shader.setInt("fragTexture", 0);

	position = glm::vec2(0.13f, 0.77f);
}
