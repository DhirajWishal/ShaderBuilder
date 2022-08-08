// Copyright (c) 2022 Dhiraj Wishal

#include "ShaderBuilder/Builder.hpp"
#include "ShaderBuilder/Vec2.hpp"
#include "ShaderBuilder/Vec3.hpp"
#include "ShaderBuilder/Vec4.hpp"

#include "Profiler.hpp"

#include <iostream>

/**
 * This generates the same SPIRV as the following GLSL.
 *
 * ```GLSL
 * layout (location = 0) in vec3 inPosition;
 * layout (location = 12) in vec2 inTextureCoordinates;
 * layout (location = 0) out vec2 outTextureCoordinates;
 *
 * layout (set = 0, binding = 0) uniform Camera
 * {
 * 		mat4 m_Projection;
 * 		mat4 m_View;
 * } camera;
 *
 * void main()
 * {
 *		vec4 temporary = vec4(100);
 *		vec4 another;
 *
 *		another = temporary;
 * }
 * ```
 */
[[nodiscard]] ShaderBuilder::SPIRVBinary CreateVertexShader()
{
	[[maybe_unused]] Profiler _profiler;

	ShaderBuilder::Builder shaderSource;
	auto inPosition = shaderSource.createInput<ShaderBuilder::Vec3<float>>(0, "inPosition");						
	auto inTextureCoordinates = shaderSource.createInput<ShaderBuilder::Vec2<float>>(12, "inTextureCoordinates");	
	auto outTextureCoordinates = shaderSource.createOutput<ShaderBuilder::Vec2<float>>(0, "outTextureCoordinates"); 

	class Camera final : public ShaderBuilder::DataType<Camera>
	{
	public:
		explicit Camera(ShaderBuilder::SPIRVSource& source, const std::string& name) : ShaderBuilder::DataType<Camera>(source, name), m_Projection(source, "m_Projection"), m_View(source, "m_View") {}

		ShaderBuilder::Vec4<float> m_Projection;	// These should be Mat4.
		ShaderBuilder::Vec2<float> m_View;			// These should be Mat4.
	};

	auto camera = shaderSource.createUniform<Camera>(0, 0, "camera", &Camera::m_Projection, &Camera::m_View);

	// {
	// 	auto function = shaderSource.createFunction<void>("main");
	// 	auto temporary = function.createVariable<ShaderBuilder::Vec4<float>>("temporary", 100);
	// 	auto another = function.createVariable<ShaderBuilder::Vec4<float>>("another");
	// 
	// 	another = temporary;
	// 
	// 	shaderSource.addEntryPoint(ShaderBuilder::ShaderType::Vertex, function, "inPosition", "inTextureCoordinates", "outTextureCoordinates");
	// }

	return shaderSource.compile();
}

int main()
{
	// Generate the shader.
	const auto output = CreateVertexShader();

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
