// Copyright (c) 2022 Dhiraj Wishal
#pragma once

#include "Types/Callable.hpp"
#include "Types/Vec4.hpp"

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
		/**
		 * Built-ins structure.
		 * This contains all the built-in types.
		 */
		struct BuiltIns final
		{
			/**
			 * Explicit constructor.
			 *
			 * @param stream The code stream.
			 */
			explicit BuiltIns(CodeStream& stream);

			Vec4 gl_Position;
		};

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
		 * Create a new variable.
		 *
		 * @param variableName The name of the variable.
		 * @param arguments The optional initializer arguments.
		 */
		template<class Type, class... Arguments>
		[[nodiscard]] Type createVariable(std::string&& variableName, Arguments&&... arguments)
		{
			return Type(std::forward<Arguments>(arguments)..., std::move(variableName), m_ShaderCode);
		}

		/**
		 * Create a new parameter.
		 *
		 * @param variableName The name of the variable.
		 */
		template<class Type>
		[[nodiscard]] Type createParameter(std::string&& variableName)
		{
			return Type(std::move(variableName), m_ShaderCode, MetaInformation::ParameterType);
		}

		/**
		 * Create a new function.
		 *
		 * @param functionName The name of the function.
		 * @param function The function containing the body.
		 * @param parameters The function's parameter types.
		 * @return The callable type.
		 */
		template<class Return, class... Arguments, class Function>
		[[nodiscard]] Callable<Return, Arguments...> createFunction(std::string&& functionName, Function&& function, Arguments&&... parameters)
		{
			const auto callable = Callable<Return, Arguments...>("void", std::move(functionName), m_ShaderCode, std::forward<Arguments>(parameters)...);

			m_ShaderCode << std::endl << "{" << std::endl;
			function();
			m_ShaderCode << "}" << std::endl;

			return callable;
		}

		/**
		 * Return something from the current function.
		 *
		 * @param variable The variable to return.
		 */
		template<class Type>
		void functionReturn(Type&& variable)
		{
			m_ShaderCode << "return " << variable.getName() << std::endl;
		}

		/**
		 * Get the internally stored built-in types.
		 *
		 * @return The built-ins.
		 */
		[[nodiscard]] BuiltIns& getBuiltIns() { return m_BuiltIns; }

		/**
		 * Get the internally stored built-in types.
		 *
		 * @return The built-ins.
		 */
		[[nodiscard]] const BuiltIns& getBuiltIns() const { return m_BuiltIns; }

	public:
		/**
		 * Get the internally stored code as GLSL.
		 *
		 * @return The GLSL code.
		 */
		[[nodiscard]] std::string getGLSL() const { return m_ShaderCode.str(); }

	private:
		CodeStream m_ShaderCode;
		BuiltIns m_BuiltIns;
	};
} // namespace ShaderBuilder

#define SB_CREATE_INPUT(builder, type, location, name) auto name = builder.createInput<type>(location, #name)
#define SB_CREATE_OUTPUT(builder, type, location, name) auto name = builder.createOutput<type>(location, #name)