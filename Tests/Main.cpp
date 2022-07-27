// Copyright (c) 2022 Dhiraj Wishal

#include "ShaderBuilder/Builder.hpp"
#include "ShaderBuilder/Types/Vec2.hpp"
#include "ShaderBuilder/Types/Vec3.hpp"

#include <iostream>

int main()
{
	ShaderBuilder::Builder shaderSource;

	SB_CREATE_INPUT(shaderSource, ShaderBuilder::Vec3, 0, inPosition);
	SB_CREATE_INPUT(shaderSource, ShaderBuilder::Vec2, 12, inTextureCoordinates);

	SB_CREATE_OUTPUT(shaderSource, ShaderBuilder::Vec2, 0, outTextureCoordinates);

	outTextureCoordinates = inTextureCoordinates;

	std::cout << shaderSource.getGLSL() << std::endl;
}