// Copyright (c) 2022 Dhiraj Wishal

#pragma once

#include "InstructionStores.hpp"
#include <vector>

namespace ShaderBuilder
{
	/**
	 * SPIR-V Source class.
	 * This contains all the source information provided by the data types and others.
	 */
	class SPIRVSource final
	{
	public:
		/**
		 * Insert a new set of compatibility instructions to the source.
		 * Note that this will append an end line instruction at the end so you don't have to provide one.
		 *
		 * @tparam Types The data types.
		 * @param data The data to enter.
		 */
		template<class...Types>
		void insertCompatibilityInstruction(Types&&... data) { m_CompatibilityInstructions.insert(toString(std::forward<Types>(data)...)); }

		/**
		 * Insert a new set of extension instructions to the source.
		 * Note that this will append an end line instruction at the end so you don't have to provide one.
		 *
		 * @tparam Types The data types.
		 * @param data The data to enter.
		 */
		template<class...Types>
		void insertExtensionInstruction(Types&&... data) { m_ExtensionInstructions.insert(toString(std::forward<Types>(data)...)); }

		/**
		 * Insert a new set of ExtInstImport instructions to the source.
		 * Note that this will append an end line instruction at the end so you don't have to provide one.
		 *
		 * @tparam Types The data types.
		 * @param data The data to enter.
		 */
		template<class...Types>
		void insertExtInstImportInstruction(Types&&... data) { m_ExtInstImportInstructions.insert(toString(std::forward<Types>(data)...)); }

		/**
		 * Set a memory model to the source.
		 *
		 * @tparam Types The data types.
		 * @param data The data to enter.
		 */
		template<class...Types>
		void setMemoryModel(Types&&... data) { m_MemoryModel = toString(std::forward<Types>(data)...); }

		/**
		 * Insert a new set of entry point instructions to the source.
		 * Note that this will append an end line instruction at the end so you don't have to provide one.
		 *
		 * @tparam Types The data types.
		 * @param data The data to enter.
		 */
		template<class...Types>
		void insertEntryPoint(Types&&... data) { m_EntryPoints.insert(toString(std::forward<Types>(data)...)); }

		/**
		 * Insert a new set of execution modes instructions to the source.
		 * Note that this will append an end line instruction at the end so you don't have to provide one.
		 *
		 * @tparam Types The data types.
		 * @param data The data to enter.
		 */
		template<class...Types>
		void insertExecutionMode(Types&&... data) { m_ExecutionModes.insert(toString(std::forward<Types>(data)...)); }

		/**
		 * Insert a new set of debug source instructions to the source.
		 * Note that this will append an end line instruction at the end so you don't have to provide one.
		 *
		 * @tparam Types The data types.
		 * @param data The data to enter.
		 */
		template<class...Types>
		void insertDebugSource(Types&&... data) { m_DebugSources.insert(toString(std::forward<Types>(data)...)); }

		/**
		 * Insert a new set of debug name instructions to the source.
		 * Note that this will append an end line instruction at the end so you don't have to provide one.
		 *
		 * @tparam Types The data types.
		 * @param data The data to enter.
		 */
		template<class...Types>
		void insertDebugName(Types&&... data) { m_DebugNames.insert(toString(std::forward<Types>(data)...)); }

		/**
		 * Insert a new set of debug module processed instructions to the source.
		 * Note that this will append an end line instruction at the end so you don't have to provide one.
		 *
		 * @tparam Types The data types.
		 * @param data The data to enter.
		 */
		template<class...Types>
		void insertDebugModuleProcessed(Types&&... data) { m_DebugModuleProcessedInstructions.insert(toString(std::forward<Types>(data)...)); }

		/**
		 * Insert a new set of annotation instructions to the source.
		 * Note that this will append an end line instruction at the end so you don't have to provide one.
		 *
		 * @tparam Types The data types.
		 * @param data The data to enter.
		 */
		template<class...Types>
		void insertAnnotation(Types&&... data) { m_Annotations.insert(toString(std::forward<Types>(data)...)); }

		/**
		 * Insert a new set of type declaration instructions to the source.
		 * Note that this will append an end line instruction at the end so you don't have to provide one.
		 *
		 * @tparam Types The data types.
		 * @param data The data to enter.
		 */
		template<class...Types>
		void insertTypeDeclaration(Types&&... data) { m_TypeDeclarations.insert(toString(std::forward<Types>(data)...)); }

		/**
		 * Insert a new set of function declaration instructions to the source.
		 * Note that this will append an end line instruction at the end so you don't have to provide one.
		 *
		 * @tparam Types The data types.
		 * @param data The data to enter.
		 */
		template<class...Types>
		void insertFunctionDeclaration(Types&&... data) { m_FunctionDeclarations.insert(toString(std::forward<Types>(data)...)); }

		/**
		 * Insert a new set of function definition instructions to the source.
		 * Note that this will append an end line instruction at the end so you don't have to provide one.
		 *
		 * @tparam Types The data types.
		 * @param data The data to enter.
		 */
		template<class...Types>
		void insertFunctionDefinition(Types&&... data) { m_FunctionDefinitions.insert(toString(std::forward<Types>(data)...)); }

	public:
		/**
		 * Get the source assembly.
		 *
		 * @return The assembly string
		 */
		[[nodiscard]] std::string getSourceAssembly() const;

	private:
		/**
		 * Convert the variadic arguments to a string.
		 *
		 * @tparam Types The variadic argument types.
		 * @param data The data to convert.
		 * @return The converted string.
		 */
		template<class...Types>
		[[nodiscard]] std::string toString(Types&&... data) const
		{
			std::stringstream stream;
			auto insertData = [this, &stream](auto&& member) { stream << std::move(member); };
			(insertData(std::move(data)), ...);

			return stream.str();
		}

	private:
		CompatibiltyInstructions m_CompatibilityInstructions;
		ExtensionInstructions m_ExtensionInstructions;
		ExtInstImportInstructions m_ExtInstImportInstructions;
		EntryPointInstructions m_EntryPoints;
		ExecutionModeInstructions m_ExecutionModes;

		DebugSourceInstructions m_DebugSources;
		DebugNameInstructions m_DebugNames;
		DebugModuleProcesseInstructions m_DebugModuleProcessedInstructions;

		AnnotationInstructions m_Annotations;
		TypeDeclarationInstructions m_TypeDeclarations;

		FunctionDeclarationInstructions m_FunctionDeclarations;
		FunctionDefinitionInstructions m_FunctionDefinitions;

		std::string m_MemoryModel;
	};
} // namespace ShaderBuilder