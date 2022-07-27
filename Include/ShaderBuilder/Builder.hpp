// Copyright (c) 2022 Dhiraj Wishal
#pragma once

#include "Types/DataType.hpp"

namespace ShaderBuilder
{
	/**
	 * Builder class.
	 * This class contains the base code for SPIR-V generation and can be used to
	 * compile to other high level languages, like GLSL and HLSL.
	 *
	 * This class doesn't need to be inherited, so the user can use this class as a
	 * object to create the shaders.
	 */
	class Builder
	{
	public:
		/**
		 * Default constructor.
		 */
		Builder();

		/**
		 * Create a new shader input.
		 *
		 * @param location The input location.
		 * @param variableName The name of the variable.
		 */
		template<class Type>
		[[nodiscard]] Type createInput(uint32_t location, std::string&& variableName)
		{
			m_ShaderCode << "layout(location = " << location << ") in ";
			return Type(std::move(variableName), m_ShaderCode);
		}

		/**
		 * Create a new shader output.
		 *
		 * @param location The output location.
		 * @param variableName The name of the variable.
		 */
		template<class Type>
		[[nodiscard]] Type createOutput(uint32_t location, std::string&& variableName)
		{
			m_ShaderCode << "layout(location = " << location << ") out ";
			return Type(std::move(variableName), m_ShaderCode);
		}

		/**
		 * Get the internally stored code as GLSL.
		 *
		 * @return The GLSL code.
		 */
		[[nodiscard]] std::string getGLSL() const { return m_ShaderCode.str(); }

	private:
		std::stringstream m_ShaderCode;
	};
} // namespace ShaderBuilder

#define SB_CREATE_INPUT(builder, type, location, name) auto name = builder.createInput<type>(location, #name)
#define SB_CREATE_OUTPUT(builder, type, location, name) auto name = builder.createOutput<type>(location, #name)