// Copyright (c) 2022 Dhiraj Wishal

#include "ShaderBuilder/Builder.hpp"
#include "ShaderBuilder/Types/Vec2.hpp"
#include "ShaderBuilder/Types/Vec3.hpp"

#include <iostream>

int main()
{
	ShaderBuilder::Builder shaderSource;
	auto inPosition = shaderSource.createInput<ShaderBuilder::Vec3<float>>(0, "inPosition");
	auto inTextureCoordinates = shaderSource.createInput<ShaderBuilder::Vec2<float>>(12, "inTextureCoordinates");
	auto outTextureCoordinates = shaderSource.createOutput<ShaderBuilder::Vec2<float>>(0, "outTextureCoordinates");

	struct Camera final
	{
		ShaderBuilder::Vec4<float> m_Position = ShaderBuilder::Vec4<float>("m_Position");
		ShaderBuilder::Vec2<float> m_Color = ShaderBuilder::Vec2<float>("m_Color");
	};

	auto camera = shaderSource.createUniform<Camera>(0, 0, "camera", &Camera::m_Position, &Camera::m_Color);

	auto mainFunction = shaderSource.createFunction("main", [&shaderSource]()
		{
			auto temporary = shaderSource.createVariable<ShaderBuilder::Vec4<float>>("temporary");
		}
	);

	std::cout << shaderSource.getString() << std::endl;
}
