// Copyright (c) 2022 Dhiraj Wishal

#include "ShaderBuilder/Builder.hpp"
#include "ShaderBuilder/Types/Vec2.hpp"
#include "ShaderBuilder/Types/Vec3.hpp"

#include <iostream>

int main()
{
	ShaderBuilder::Builder shaderSource;

	auto inPosition = shaderSource.createInput<ShaderBuilder::Vec3>(0, "inPosition");
	auto inTextureCoordinates = shaderSource.createInput<ShaderBuilder::Vec2>(12, "inTextureCoordinates");
	auto outTextureCoordinates = shaderSource.createOutput<ShaderBuilder::Vec2>(0, "outTextureCoordinates");

	auto rando = shaderSource.createFunction<void>("main", [&]()
		{
			auto temp =
				shaderSource.createVariable<ShaderBuilder::Vec4>("temp", 1, 2, 3, 4);
			shaderSource.getBuiltIns().gl_Position = temp;
			outTextureCoordinates = inTextureCoordinates;
		}
	);

	std::cout << shaderSource.getGLSL() << std::endl;
}

/**
 * struct Camera final : public ShaderBuilder::Uniform
 * {
 *		Camera() : Uniform(0, 0, "Camera", &Camera::view,
 *&Camera::projection) {}
 *
 *		ShaderBuilder::Mat4 view = ShaderBuilder::Mat4("view",
 *ShaderBuilder::MetaInformation::Member); ShaderBuilder::Mat4 projection =
 *ShaderBuilder::Mat4("projection", ShaderBuilder::MetaInformation::Member);
 * };
 *
 * RegisterUniform<Camera>(0, 0, &Camera::view, &Camera::projection);
 */