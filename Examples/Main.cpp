// Copyright (c) 2022 Dhiraj Wishal

#include "ShaderBuilder/VertexBuilder.hpp"
#include "ShaderBuilder/Vec2.hpp"
#include "ShaderBuilder/Vec3.hpp"
#include "ShaderBuilder/Vec4.hpp"
#include "ShaderBuilder/Parameter.hpp"

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
 *		vec4 temporary = vec4(inPosition, 1);
 *		outTextureCoordinates = inTextureCoordinates;
 *
 *		gl_Position = temporary;
 * }
 * ```
 */
[[nodiscard]] ShaderBuilder::SPIRVBinary CreateVertexShader()
{
	[[maybe_unused]] Profiler _profiler;

	ShaderBuilder::VertexBuilder shaderSource;
	auto inPosition = shaderSource.createInput<ShaderBuilder::Vec3<float>>(0);
	auto inTextureCoordinates = shaderSource.createInput<ShaderBuilder::Vec2<float>>(12);
	auto outTextureCoordinates = shaderSource.createOutput<ShaderBuilder::Vec2<float>>(0);

	class Camera final : public ShaderBuilder::DataType<Camera>
	{
	public:
		explicit Camera(ShaderBuilder::SPIRVSource& source, const std::string& identifier) : ShaderBuilder::DataType<Camera>(source, identifier), m_Projection(source, source.getUniqueIdentifier()), m_View(source, source.getUniqueIdentifier()) {}

		ShaderBuilder::Vec4<float> m_Projection;	// These should be Mat4.
		ShaderBuilder::Vec2<float> m_View;			// These should be Mat4.
	};

	auto helper = shaderSource.createFunction([](ShaderBuilder::FunctionBuilder& builder, ShaderBuilder::Parameter<ShaderBuilder::Vec3<float>> vec, ShaderBuilder::Parameter<ShaderBuilder::Vec3<float>> vec2)
		{
			const auto var = builder.createVariable<ShaderBuilder::Vec4<float>>(vec.value(), 21.0f);
		}
	);

	auto camera = shaderSource.createUniform<Camera>(0, 0, &Camera::m_Projection, &Camera::m_View);

	auto function = shaderSource.createFunction([&](ShaderBuilder::VertexFunctionBuilder& builder)
		{
			builder.call(helper, inPosition.value(), inPosition.value());

			outTextureCoordinates = inTextureCoordinates;
			builder.setPoisition(builder.createVariable<ShaderBuilder::Vec4<float>>(inPosition.value(), 1.0f));
		}
	);

	function();
	shaderSource.addEntryPoint(function, inPosition, inTextureCoordinates, outTextureCoordinates);

	return shaderSource.compile(ShaderBuilder::OptimizationFlags::DebugMode);
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
