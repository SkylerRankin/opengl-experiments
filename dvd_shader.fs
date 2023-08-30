#version 330 core

out vec4 FragColor;

in vec2 fragTextureCoordinate;

uniform sampler2D fragTexture;
uniform vec3 foregroundColor;

void main() {
	vec4 textureColor = texture(fragTexture, fragTextureCoordinate);
	FragColor = mix(vec4(0.2f, 0.3f, 0.3f, 1.0f), vec4(foregroundColor, 1.0f), textureColor.r);
}
