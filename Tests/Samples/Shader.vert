#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 12) in vec2 inTextureCoordinates;

layout(location = 0) out vec2 outTextureCoordinates;

layout (binding = 0) uniform Camera 
{
	mat4 view;
	mat4 projection;
} camera;

void main()
{
	gl_Position = camera.projection * camera.view * mat4(1.0f) * vec4(inPosition, 100.0f);
	outTextureCoordinates = inTextureCoordinates;
} 