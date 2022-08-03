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

	{
		auto function = shaderSource.createFunction<void>("main");
		auto temporary = function.createVariable<ShaderBuilder::Vec4<float>>("temporary", 100);
		auto another = function.createVariable<ShaderBuilder::Vec4<float>>("another");

		another = temporary;

		shaderSource.addEntryPoint(ShaderBuilder::ShaderType::Vertex, function, "inPosition", "inTextureCoordinates", "outTextureCoordinates");
	}

	// {
	// 	auto function = shaderSource.createFunction<ShaderBuilder::Vec4<float>>("something");
	// 	auto temporary = function.createVariable<ShaderBuilder::Vec4<float>>("anotherThing", 100);
	// 	auto another = function.createVariable<ShaderBuilder::Vec4<float>>("someOtherThing");
	// 
	// 	another = temporary;
	// 	function.exit(another);
	// }

	// Show the raw data.
	std::cout << "-------------------- JSON --------------------" << std::endl;
	std::cout << shaderSource.getJSON() << std::endl;

	std::cout << "-------------------- Generated Assembly --------------------" << std::endl;
	std::cout << shaderSource.getString() << std::endl;

	// Generate the shader.
	const auto output = shaderSource.compile();

	// Show the generated data.
	std::cout << "-------------------- Compiled Assembly --------------------" << std::endl;
	std::cout << output.disassemble() << std::endl;

	std::cout << "-------------------- GLSL --------------------" << std::endl;
	std::cout << output.getGLSL() << std::endl;

	std::cout << "-------------------- HLSL --------------------" << std::endl;
	std::cout << output.getHLSL() << std::endl;

	std::cout << "-------------------- MSL --------------------" << std::endl;
	std::cout << output.getMSL();
}
