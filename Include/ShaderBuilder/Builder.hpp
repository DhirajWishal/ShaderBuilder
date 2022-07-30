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
		Type createInput(uint32_t location, std::string&& name)
		{
			registerType<Type>();
			m_TypeDeclarations << "%" << name << " = OpTypePointer Input " << TypeTraits<Type>::Identifier.data() << std::endl;
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
		Type createOutput(uint32_t location, std::string&& name)
		{
			registerType<Type>();
			m_TypeDeclarations << "%" << name << " = OpTypePointer Output " << TypeTraits<Type>::Identifier.data() << std::endl;
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
		Type createVariable(std::string&& name)
		{
			registerType<Type>();
			m_TypeDeclarations << "%" << name << " = OpTypePointer Function " << TypeTraits<Type>::Identifier.data() << std::endl;
			return Type(std::move(name));
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
		std::set<std::string_view> m_TypeAvailability;			// Contains information if a type is registered or not.

		std::string m_OpMemoryModel;							// The single required OpMemoryModel instruction.

		std::vector<std::stringstream> m_FunctionDeclarations;	// All function declarations ("declarations" are functions without a body; there is no forward declaration to a function with a body).
		std::vector<std::stringstream> m_FunctionDefinitions;	// All function definitions (functions with a body).

		uint32_t m_UniqueIDs = 1;
	};
} // namespace ShaderBuilder