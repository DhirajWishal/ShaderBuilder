// Copyright (c) 2022 Dhiraj Wishal

#include "ShaderBuilder/Builder.hpp"
#include "ShaderBuilder/Vec2.hpp"
#include "ShaderBuilder/Vec3.hpp"
#include "ShaderBuilder/Vec4.hpp"

#include <iostream>

class Camera final : public ShaderBuilder::DataType<Camera>
{
public:
	explicit Camera(ShaderBuilder::SPIRVSource& source, const std::string& name) : ShaderBuilder::DataType<Camera>(source, name), m_Position(source, "m_Position"), m_Color(source, "m_Color") {}

	ShaderBuilder::Vec4<float> m_Position;
	ShaderBuilder::Vec2<float> m_Color;
};

int main()
{
	ShaderBuilder::Builder shaderSource;
	auto inPosition = shaderSource.createInput<ShaderBuilder::Vec3<float>>(0, "inPosition");
	auto inTextureCoordinates = shaderSource.createInput<ShaderBuilder::Vec2<float>>(12, "inTextureCoordinates");
	auto outTextureCoordinates = shaderSource.createOutput<ShaderBuilder::Vec2<float>>(0, "outTextureCoordinates");

	auto camera = shaderSource.createUniform<Camera>(0, 0, "camera", &Camera::m_Position, &Camera::m_Color);

	auto mainFunction = shaderSource.createFunction<void>("main", [&shaderSource](ShaderBuilder::FunctionBuilder&& builder)
		{
			auto temporary = builder.createVariable<ShaderBuilder::Vec4<float>>("temporary", 100);
			auto another = builder.createVariable<ShaderBuilder::Vec4<float>>("another");

			another = temporary;
		}
	);

	shaderSource.addEntryPoint(ShaderBuilder::ShaderType::Vertex, "main", "inPosition", "inTextureCoordinates", "outTextureCoordinates");

	std::cout << "-------------------- Generated Assembly --------------------" << std::endl;
	std::cout << shaderSource.getString() << std::endl;

	const auto output = shaderSource.compile();

	std::cout << "-------------------- Compiled Assembly --------------------" << std::endl;
	std::cout << output.disassemble() << std::endl;

	std::cout << "-------------------- GLSL --------------------" << std::endl;
	std::cout << output.getGLSL() << std::endl;

	std::cout << "-------------------- HLSL --------------------" << std::endl;
	std::cout << output.getHLSL() << std::endl;

	std::cout << "-------------------- MSL --------------------" << std::endl;
	std::cout << output.getMSL();
}
