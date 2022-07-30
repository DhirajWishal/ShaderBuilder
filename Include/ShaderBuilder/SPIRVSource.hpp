// Copyright (c) 2022 Dhiraj Wishal

#pragma once

#include <sstream>
#include <string>
#include <set>

namespace ShaderBuilder
{
	/**
	 * SPIR-V source structure.
	 * This contains the SPIR-V source data.
	 */
	class SPIRVSource final
	{
	public:
		/**
		 * Convert all the internally stored data to a string.
		 *
		 * @return The combined string.
		 */
		[[nodiscard]] std::string toString() const;

	public:
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
		std::stringstream m_FunctionDeclarations;				// All function declarations ("declarations" are functions without a body; there is no forward declaration to a function with a body).
		std::stringstream m_FunctionDefinitions;				// All function definitions (functions with a body).

		std::string m_OpMemoryModel;							// The single required OpMemoryModel instruction.

		std::set<uint64_t> m_TypeAvailability;					// Contains information if a type is registered or not.
		uint32_t m_UniqueIDs = 1;
	};
} // namespace ShaderBuilder