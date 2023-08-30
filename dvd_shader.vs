#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 textureCoordinate;

out vec2 fragTextureCoordinate;

uniform mat4 transform;

void main() {
	gl_Position = transform * vec4(vertexPosition, 1.0);
	fragTextureCoordinate = textureCoordinate;
}
