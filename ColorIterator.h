#pragma once
#include <glm/common.hpp>
#include <glm/glm.hpp>
#include <array>

class ColorIterator {
public:
	ColorIterator();
	ColorIterator(float r, float g, float b);
	glm::vec3 current() const;
	glm::vec3 next();
private:
	glm::vec3 currentColor;
	int currentOffsetIndex = 0;
	int currentIterations = 0;
	float minValue, maxValue;
	const float increment = 1.0f / 255;
	const std::array<glm::vec3, 6> offsets = {
		glm::vec3(0.0f, increment, 0.0f),
		glm::vec3(-increment, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, increment),
		glm::vec3(0.0f, -increment, 0.0f),
		glm::vec3(increment, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, -increment)
	};
};
