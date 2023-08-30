#include <math.h>

#include "ColorIterator.h"

ColorIterator::ColorIterator() : ColorIterator(1.0f, 0.0f, 0.0f){}
ColorIterator::ColorIterator(float r, float g, float b) :
	currentColor(glm::vec3(r, g, b)),
	maxValue(glm::max(glm::max(r, g), b)),
	minValue(glm::min(glm::min(r, g), b)) {}

glm::vec3 ColorIterator::current() const {
	return currentColor;
}

glm::vec3 ColorIterator::next() {
	glm::vec3 offset = offsets[currentOffsetIndex];
	currentColor += offset;
	currentIterations++;
	if (currentIterations >= 255) {
		currentIterations = 0;
		currentOffsetIndex = (currentOffsetIndex + 1) % 6;
	}
	return currentColor;
}