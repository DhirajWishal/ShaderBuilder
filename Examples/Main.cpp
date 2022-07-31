// Copyright (c) 2022 Dhiraj Wishal

#include "ShaderBuilder/Builder.hpp"
#include "ShaderBuilder/Vec2.hpp"
#include "ShaderBuilder/Vec3.hpp"

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
			auto temporary = shaderSource.createLocalVariable<ShaderBuilder::Vec4<float>>("temporary");
		}
	);

	shaderSource.addEntryPoint(ShaderBuilder::ShaderType::Vertex, "main", "inPosition", "inTextureCoordinates", "outTextureCoordinates");

	const auto output = shaderSource.compile();

	std::cout << "-------------------- Assembly --------------------" << std::endl;
	std::cout << output.disassemble() << std::endl;

	std::cout << "-------------------- GLSL --------------------" << std::endl;
	std::cout << output.getGLSL() << std::endl;

	std::cout << "-------------------- HLSL --------------------" << std::endl;
	std::cout << output.getHLSL() << std::endl;

	std::cout << "-------------------- MSL --------------------" << std::endl;
	std::cout << output.getMSL();
}
