// Copyright (c) 2022 Dhiraj Wishal

#pragma once

#include "Callable.hpp"
#include "Vec4.hpp"

#include "SPIRVSource.hpp"
#include "SPIRVBinary.hpp"

#include <sstream>
#include <set>

namespace ShaderBuilder
{
	/**
	 * Shader type enum.
	 */
	enum class ShaderType : uint8_t
	{
		Vertex,
		TessellationControl,
		TessellationEvaluation,
		Geometry,
		Fragment,
		Compute,
	};

	/**
	 * Addressing model enum.
	 */
	enum class AddressingModel : uint8_t
	{
		Logical,
		Physical32,
		Physical64,
		PhysicalStorageBuffer64
	};

	/**
	 * Memory model enum.
	 */
	enum class MemoryModel : uint8_t
	{
		Simple,
		GLSL450,
		OpenCL,
		Vulkan
	};

	/**
	 * Configuration structure.
	 * This contains basic information about the shader code that is being built.
	 */
	struct Configuration final
	{
		AddressingModel m_AddressingModel = AddressingModel::Logical;
		MemoryModel m_MemoryModel = MemoryModel::GLSL450;

	};

	/**
	 * Builder class.
	 * This class contains the base code for SPIR-V generation and can be used to
	 * compile to other high level languages, like GLSL and HLSL.
	 */
	class Builder final
	{
	public:
		/**
		 * Explicit constructor.
		 *
		 * @param config The builder's initial configuration.
		 */
		explicit Builder(Configuration config = Configuration());

		/**
		 * Create a new shader input.
		 *
		 * @tparam Type The type of the variable.
		 * @param name The variable name.
		 * @return The created variable.
		 */
		template<class Type>
		[[nodiscard]] Type createInput(uint32_t location, std::string&& name)
		{
			registerType<Type>();
			m_Source.insertTypeDeclaration("%input_", name, " = OpTypePointer Input ", TypeTraits<Type>::Identifier);
			m_Source.insertTypeDeclaration("%", name, " = OpVariable %input_", name, " Input");
			m_Source.insertDebugName("OpName %", name, " \"", name, "\"");

			return Type(std::move(name));
		}

		/**
		 * Create a new shader output.
		 *
		 * @tparam Type The type of the variable.
		 * @param name The variable name.
		 * @return The created variable.
		 */
		template<class Type>
		[[nodiscard]] Type createOutput(uint32_t location, std::string&& name)
		{
			registerType<Type>();
			m_Source.insertTypeDeclaration("%output_", name, " = OpTypePointer Output ", TypeTraits<Type>::Identifier);
			m_Source.insertTypeDeclaration("%", name, " = OpVariable %output_", name, " Output");
			m_Source.insertDebugName("OpName %", name, " \"", name, "\"");

			return Type(std::move(name));
		}

		/**
		 * Create a new local variable.
		 * These variables must be created within a function.
		 *
		 * @tparam Type The type of the variable.
		 * @param name The variable name.
		 * @return The created variable.
		 */
		template<class Type>
		[[nodiscard]] Type createLocalVariable(std::string&& name)
		{
			registerType<Type>();
			m_Source.insertTypeDeclaration("%variable_", name, " = OpTypePointer Function ", TypeTraits<Type>::Identifier);
			m_Source.insertFunctionDefinition("%", name, " = OpVariable ", "%variable_", name, " Function");
			m_Source.insertDebugName("OpName %", name, " \"", name, "\"");

			return Type(std::move(name));
		}

		/**
		 * Create a new Uniform.
		 *
		 * Note that members should be pointers to the member variables and should be in the same order they appear in the actual struct.
		 * For example,
		 *
		 * ```c++
		 * struct Camera { Mat4 Projection = Mat4("Projection"); Mat4 View = Mat4("View"); };
		 * builder.createUniform<Camera>(0, 0, "camera", &Camera::Projection, &Camera::View);
		 * ```
		 *
		 * @tparam Type The type of the uniform.
		 * @tparam Members The uniform's members.
		 * @param set The descriptor set index.
		 * @param binding The uniform's binding.
		 * @param name The name of the uniform.
		 * @param members The members of the uniform.
		 * @return The created uniform.
		 */
		template<class Type, class... Members>
		[[nodiscard]] Type createUniform(uint32_t set, uint32_t binding, std::string&& name, Members... members)
		{
			// Register the members.
			m_Source.insertTypeDeclaration("%type_", name, " = OpTypeStruct ", resolveMemberVariableTypeIdentifiers<Members...>());

			// Setup type declarations.
			m_Source.insertTypeDeclaration("%uniform_", name, " = OpTypePointer Uniform %type_", name);
			m_Source.insertTypeDeclaration("%", name, " = OpVariable %uniform_", name, " Uniform");

			// Set the type debug information and annotations.
			m_Source.insertDebugName("OpName %uniform_", name, " \"", name, "\"");
			m_Source.insertDebugName("OpName %", name, " \"\"");

			m_Source.insertAnnotation("OpDecorate %", name, " DescriptorSet ", set);
			m_Source.insertAnnotation("OpDecorate %", name, " Binding ", binding);

			// Create the uniform.
			auto uniform = Type();

			uint32_t counter = 0;
			uint64_t offsets = 0;
			auto logMemberInformation = [this, &uniform, &name, &counter, &offsets](auto member)
			{
				using MemberType = std::remove_cv_t<std::remove_reference_t<decltype(uniform.*member)>>;

				m_Source.insertDebugName("OpMemberName %type_", name, " ", counter, " \"", uniform.*member, "\"");
				m_Source.insertAnnotation("OpMemberDecorate %type_", name, " ", counter, " Offset ", offsets);

				counter++;
				offsets += TypeTraits<MemberType>::Size;
			};
			(logMemberInformation(members), ...);

			return uniform;
		}

		/**
		 * Create a new function.
		 *
		 * @tparam Type The function type.
		 * @param name The name of the function.
		 * @param function The function definition.
		 * @return The callable type.
		 */
		template<class Type>
		[[nodiscard]] decltype(auto) createFunction(std::string&& name, Type&& function)
		{
			using ReturnType = std::invoke_result_t<Type>;
			registerCallable<Callable<ReturnType>>();

			m_Source.insertDebugName("OpName %", name, " \"", name, "\"");
			m_Source.insertFunctionDefinition("%", name, " = OpFunction ", TypeTraits<ReturnType>::Identifier, " None ", TypeTraits<Callable<ReturnType>>::Identifier);
			m_Source.insertFunctionDefinition("%function_block_", name, " = OpLabel");

			function();

			m_Source.insertFunctionDefinition("OpReturn");
			m_Source.insertFunctionDefinition("OpFunctionEnd");

			return Callable<ReturnType>(std::move(name));
		}

		/**
		 * Add an entry point to the shader.
		 *
		 * @tparam Attributes The input and output attribute types.
		 * @param shaderType The type of the shader.
		 * @param name The name of the entry point function.
		 * @param attributes The names of the input and output attributes.
		 */
		template<class... Attributes>
		void addEntryPoint(ShaderType shaderType, std::string&& name, Attributes&&... attributes)
		{
			std::stringstream entryPoint;
			entryPoint << "OpEntryPoint ";

			// Resolve the proper shader type.
			switch (shaderType)
			{
			case ShaderBuilder::ShaderType::Vertex:
				entryPoint << "Vertex ";
				break;
			case ShaderBuilder::ShaderType::TessellationControl:
				entryPoint << "TessellationControl ";
				break;
			case ShaderBuilder::ShaderType::TessellationEvaluation:
				entryPoint << "TessellationEvaluation ";
				break;
			case ShaderBuilder::ShaderType::Geometry:
				entryPoint << "Geometry ";
				break;
			case ShaderBuilder::ShaderType::Fragment:
				entryPoint << "Fragment ";
				break;
			case ShaderBuilder::ShaderType::Compute:
				entryPoint << "GLCompute ";
				break;
			}

			// Setup the entry point function information.
			entryPoint << "%" << name << " \"" << name << "\" ";

			// Setup the inputs.
			auto insertAttribute = [&entryPoint](auto&& attribute) { entryPoint << "%" << attribute << " "; };
			(insertAttribute(std::move(attributes)), ...);

			m_Source.insertEntryPoint(entryPoint.str());
		}

	public:
		/**
		 * Get the internal string.
		 *
		 * @return The string.
		 */
		[[nodiscard]] std::string getString() const;

		/**
		 * Compile the shader code and inform if there were any errors.
		 *
		 * @param shouldOptimize Whether or not to optimize the generated SPIR-V binary. Default is true.
		 * @return The compiled binary.
		 */
		[[nodiscard]] SPIRVBinary compile(bool shouldOptimize = true) const;

	private:
		/**
		 * Register type function.
		 *
		 * @tparam Type The type to register.
		 */
		template<class Type>
		void registerType()
		{
			// Try and register value types if the Type is complex.
			if constexpr (IsCompexType<Type>)
				registerType<typename TypeTraits<Type>::ValueTraits::Type>();

			m_Source.insertTypeDeclaration(TypeTraits<Type>::Declaration);
		}

		/**
		 * Register a function callback type.
		 *
		 * @tparam Type The callback type.
		 */
		template<class Type>
		void registerCallable()
		{
			// Try and register value types.
			registerType<typename TypeTraits<Type>::ValueTraits::Type>();
			m_Source.insertTypeDeclaration(TypeTraits<Type>::Declaration);
		}

		/**
		 * Resolve the member variable type identifiers to register.
		 *
		 * @tparam First The first type.
		 * @tparam Rest The rest of the types.
		 */
		template<class First, class... Rest>
		[[nodiscard]] std::string resolveMemberVariableTypeIdentifiers()
		{
			using MemberType = typename MemberVariableType<First>::Type;
			registerType<MemberType>();

			if constexpr (sizeof...(Rest) > 0)
				return std::string(TypeTraits<MemberType>::Identifier) + " " + resolveMemberVariableTypeIdentifiers<Rest...>();

			else
				return std::string(TypeTraits<MemberType>::Identifier);
		}

	private:
		SPIRVSource m_Source;
	};
} // namespace ShaderBuilder