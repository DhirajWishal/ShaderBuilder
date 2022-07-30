// Copyright (c) 2022 Dhiraj Wishal

#include "ShaderBuilder/SPIRVSource.hpp"

namespace ShaderBuilder
{
	std::string SPIRVSource::toString() const
	{
		std::stringstream finalTransform;

		// Setup the first words of the physical layout.
		finalTransform << "; Magic:     0x07230203 (SPIR-V)" << std::endl;
		finalTransform << "; Version:   0x00010000 (Version: 1.0.0)" << std::endl;
		finalTransform << "; Generator: 0x00000000 (Shader Builder; 1)" << std::endl;
		finalTransform << "; Bound:     " << m_UniqueIDs << std::endl;
		finalTransform << "; Schema:    0" << std::endl;
		finalTransform << std::endl;

		// Setup the instructions.
		finalTransform << m_OpCompatibilityInstructions.str();
		finalTransform << m_OpExtensionInstructions.str();
		finalTransform << m_OpExtInstImportInstructions.str();
		finalTransform << m_OpMemoryModel << std::endl;
		finalTransform << m_EntryPoints.str();
		finalTransform << m_ExecutionModes.str();
		finalTransform << std::endl;

		// Insert the debug information.
		finalTransform << "; Debug information." << std::endl;
		finalTransform << m_DebugSources.str();
		finalTransform << m_DebugNames.str();
		finalTransform << m_DebugModuleProcessedInstructions.str();
		finalTransform << std::endl;

		// Insert the annotations.
		finalTransform << "; Annotations." << std::endl;
		finalTransform << m_Annotations.str();
		finalTransform << std::endl;

		// Insert the type declarations.
		finalTransform << "; Type declarations." << std::endl;
		finalTransform << m_TypeDeclarations.str();
		finalTransform << std::endl;

		// Insert the function declarations.
		finalTransform << "; Function declarations." << std::endl;
		finalTransform << m_FunctionDeclarations.str();
		finalTransform << std::endl;

		// Insert the function definitions.
		finalTransform << "; Function definitions." << std::endl;
		finalTransform << m_FunctionDefinitions.str();

		return finalTransform.str();
	}
} // namespace ShaderBuilder