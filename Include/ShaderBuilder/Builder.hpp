// Copyright (c) 2022 Dhiraj Wishal

#pragma once

#include "Types/Callable.hpp"
#include "Types/Vec4.hpp"

#include <sstream>
#include <vector>
#include <functional>
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
	 * Configuration structure.
	 * This contains basic information about the shader code that is being built.
	 */
	struct Configuration final
	{
		ShaderType m_Type = ShaderType::Vertex;
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
			m_TypeDeclarations << "%" << name << " = OpTypePointer Input " << TypeTraits<Type>::Identifier << std::endl;
			m_DebugInstructions.m_Names << "OpName %" << name << " \"" << name << "\"" << std::endl;
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
			m_TypeDeclarations << "%" << name << " = OpTypePointer Output " << TypeTraits<Type>::Identifier << std::endl;
			m_DebugInstructions.m_Names << "OpName %" << name << " \"" << name << "\"" << std::endl;
			return Type(std::move(name));
		}

		/**
		 * Create a new variable.
		 *
		 * @tparam Type The type of the variable.
		 * @param name The variable name.
		 * @return The created variable.
		 */
		template<class Type>
		[[nodiscard]] Type createVariable(std::string&& name)
		{
			registerType<Type>();
			m_TypeDeclarations << "%" << name << " = OpTypePointer Function " << TypeTraits<Type>::Identifier << std::endl;
			m_DebugInstructions.m_Names << "OpName %" << name << " \"" << name << "\"" << std::endl;
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
			for (const auto identifier : memberTypes)
				m_TypeDeclarations << identifier << " ";
			m_TypeDeclarations << std::endl;

			m_TypeDeclarations << "%uniform_" << name << " = OpTypePointer Uniform %uniform_" << name << std::endl;
			m_TypeDeclarations << "%" << name << " = OpVariable %uniform_" << name << " Uniform" << std::endl;

			// Set the type debug information and annotations.
			m_DebugInstructions.m_Names << "OpName %uniform_" << name << " \"" << name << "\"" << std::endl;
			m_DebugInstructions.m_Names << "OpName %" << name << " \"\"" << std::endl;

			m_Annotations << "OpDecorate %" << name << " DescriptorSet " << set << std::endl;
			m_Annotations << "OpDecorate %" << name << " Binding " << binding << std::endl;

			uint32_t counter = 0;
			uint64_t offsets = 0;
			auto logMemberInformation = [this, &uniform, &name, &counter, &offsets](auto member)
			{
				using MemberType = std::remove_cv_t<std::remove_reference_t<decltype(uniform.*member)>>;

				m_DebugInstructions.m_Names << "OpMemberName %type_" << name << " " << counter << " \"" << uniform.*member << "\"" << std::endl;
				m_Annotations << "OpMemberDecorate %type_" << name << " " << counter << " Offset " << offsets << std::endl;

				counter++;
				offsets += TypeTraits<MemberType>::Size;
			};
			(logMemberInformation(members), ...);

			return uniform;
		}

	public:
		/**
		 * Get the internal string.
		 *
		 * @return The string.
		 */
		[[nodiscard]] std::string getString() const;

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
			{
				m_TypeDeclarations << TypeTraits<Type>::Declaration << std::endl;
				m_UniqueIDs++;
			}
		}

	private:
		std::stringstream m_OpCompatibilityInstructions;		// All OpCapability instructions.
		std::stringstream m_OpExtensionInstructions;			// Optional OpExtension instructions (extensions to SPIR-V).
		std::stringstream m_OpExtInstImportInstructions;		// Optional OpExtInstImport instructions.
		std::stringstream m_EntryPoints;						// All entry point declarations, using OpEntryPoint.
		std::stringstream m_ExecutionModes;						// All execution-mode declarations, using OpExecutionMode or OpExecutionModeId.

		struct
		{
			std::stringstream m_Sources;						// All OpString, OpSourceExtension, OpSource, and OpSourceContinued, without forward references.
			std::stringstream m_Names;							// All OpName and all OpMemberName.
			std::stringstream m_ModuleProcessedInstructions;	// All OpModuleProcessed instructions.
		} m_DebugInstructions;

		std::stringstream m_Annotations;						// All annotation instructions:
		std::stringstream m_TypeDeclarations;					// All type information.
		std::set<const char*> m_TypeAvailability;			// Contains information if a type is registered or not.

		std::string m_OpMemoryModel;							// The single required OpMemoryModel instruction.

		std::vector<std::stringstream> m_FunctionDeclarations;	// All function declarations ("declarations" are functions without a body; there is no forward declaration to a function with a body).
		std::vector<std::stringstream> m_FunctionDefinitions;	// All function definitions (functions with a body).

		uint32_t m_UniqueIDs = 1;
	};
} // namespace ShaderBuilder