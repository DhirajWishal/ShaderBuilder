// Copyright (c) 2022 Dhiraj Wishal

#pragma once

#include "Callable.hpp"
#include "Vec4.hpp"

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
			m_TypeDeclarations << "%input_" << name << " = OpTypePointer Input " << TypeTraits<Type>::Identifier << std::endl;
			m_TypeDeclarations << "%" << name << " = OpVariable %input_" << name << " Input" << std::endl;
			m_DebugNames << "OpName %" << name << " \"" << name << "\"" << std::endl;
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
			m_TypeDeclarations << "%output_" << name << " = OpTypePointer Output " << TypeTraits<Type>::Identifier << std::endl;
			m_TypeDeclarations << "%" << name << " = OpVariable %output_" << name << " Output" << std::endl;
			m_DebugNames << "OpName %" << name << " \"" << name << "\"" << std::endl;
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

			std::string variableType = "%variable_" + name + " = OpTypePointer Function " + TypeTraits<Type>::Identifier;

			if (m_TypeAvailability.insert(variableType).second)
				m_TypeDeclarations << variableType << std::endl;

			m_FunctionDefinitions << "%" << name << " = OpVariable " << "%variable_" << name << " Function" << std::endl;
			m_DebugNames << "OpName %" << name << " \"" << name << "\"" << std::endl;
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
			// Create the uniform.
			auto uniform = Type();

			// Register the members.
			std::vector<const char*> memberTypes;
			auto registerMemberTypes = [this, &uniform, &memberTypes](auto member)
			{
				using MemberType = std::remove_cv_t<std::remove_reference_t<decltype(uniform.*member)>>;
				registerType<MemberType>();

				memberTypes.emplace_back(TypeTraits<MemberType>::Identifier);
			};
			(registerMemberTypes(members), ...);

			// Setup type declarations.
			m_TypeDeclarations << "%type_" << name << " = OpTypeStruct ";
			for (const auto identifier : memberTypes) m_TypeDeclarations << identifier << " ";
			m_TypeDeclarations << std::endl;

			m_TypeDeclarations << "%uniform_" << name << " = OpTypePointer Uniform %type_" << name << std::endl;
			m_TypeDeclarations << "%" << name << " = OpVariable %uniform_" << name << " Uniform" << std::endl;

			// Set the type debug information and annotations.
			m_DebugNames << "OpName %uniform_" << name << " \"" << name << "\"" << std::endl;
			m_DebugNames << "OpName %" << name << " \"\"" << std::endl;

			m_Annotations << "OpDecorate %" << name << " DescriptorSet " << set << std::endl;
			m_Annotations << "OpDecorate %" << name << " Binding " << binding << std::endl;

			uint32_t counter = 0;
			uint64_t offsets = 0;
			auto logMemberInformation = [this, &uniform, &name, &counter, &offsets](auto member)
			{
				using MemberType = std::remove_cv_t<std::remove_reference_t<decltype(uniform.*member)>>;

				m_DebugNames << "OpMemberName %type_" << name << " " << counter << " \"" << uniform.*member << "\"" << std::endl;
				m_Annotations << "OpMemberDecorate %type_" << name << " " << counter << " Offset " << offsets << std::endl;

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

			m_DebugNames << "OpName %" << name << " \"" << name << "\"" << std::endl;
			m_FunctionDefinitions << "%" << name << " = OpFunction " << TypeTraits<ReturnType>::Identifier << " None " << TypeTraits<Callable<ReturnType>>::Identifier << std::endl;
			m_FunctionDefinitions << "%function_block_" << name << " = OpLabel" << std::endl;
			function();
			m_FunctionDefinitions << "OpReturn" << std::endl;
			m_FunctionDefinitions << "OpFunctionEnd" << std::endl;

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
			m_EntryPoints << "OpEntryPoint ";

			// Resolve the proper shader type.
			switch (shaderType)
			{
			case ShaderBuilder::ShaderType::Vertex:
				m_EntryPoints << "Vertex ";
				break;
			case ShaderBuilder::ShaderType::TessellationControl:
				m_EntryPoints << "TessellationControl ";
				break;
			case ShaderBuilder::ShaderType::TessellationEvaluation:
				m_EntryPoints << "TessellationEvaluation ";
				break;
			case ShaderBuilder::ShaderType::Geometry:
				m_EntryPoints << "Geometry ";
				break;
			case ShaderBuilder::ShaderType::Fragment:
				m_EntryPoints << "Fragment ";
				break;
			case ShaderBuilder::ShaderType::Compute:
				m_EntryPoints << "GLCompute ";
				break;
			}

			// Setup the entry point function information.
			m_EntryPoints << "%" << name << " \"" << name << "\" ";

			// Setup the inputs.
			auto insertAttribute = [this](auto&& attribute) { m_EntryPoints << "%" << attribute << " "; };
			(insertAttribute(std::move(attributes)), ...);

			m_EntryPoints << std::endl;
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

			// Register only if we haven't registered it previously
			if (m_TypeAvailability.insert(TypeTraits<Type>::Declaration).second)
				m_TypeDeclarations << TypeTraits<Type>::Declaration << std::endl;
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

			// Register only if we haven't registered it previously
			if (m_TypeAvailability.insert(TypeTraits<Type>::Declaration).second)
				m_TypeDeclarations << TypeTraits<Type>::Declaration << std::endl;
		}

	private:
		std::stringstream m_OpCompatibilityInstructions;		// All OpCapability instructions.
		std::stringstream m_OpExtensionInstructions;			// Optional OpExtension instructions (extensions to SPIR-V).
		std::stringstream m_OpExtInstImportInstructions;		// Optional OpExtInstImport instructions.
		std::stringstream m_EntryPoints;						// All entry point declarations, using OpEntryPoint.
		std::stringstream m_ExecutionModes;						// All execution-mode declarations, using OpExecutionMode or OpExecutionModeId.

		std::stringstream m_DebugSources;						// All OpString, OpSourceExtension, OpSource, and OpSourceContinued, without forward references.
		std::stringstream m_DebugNames;							// All OpName and all OpMemberName.
		std::stringstream m_DebugModuleProcessedInstructions;	// All OpModuleProcessed instructions.

		std::stringstream m_Annotations;						// All annotation instructions:
		std::stringstream m_TypeDeclarations;					// All type information.
		std::set<std::string_view> m_TypeAvailability;			// Contains information if a type is registered or not.

		std::string m_OpMemoryModel;							// The single required OpMemoryModel instruction.

		std::stringstream m_FunctionDeclarations;				// All function declarations ("declarations" are functions without a body; there is no forward declaration to a function with a body).
		std::stringstream m_FunctionDefinitions;				// All function definitions (functions with a body).
	};
} // namespace ShaderBuilder