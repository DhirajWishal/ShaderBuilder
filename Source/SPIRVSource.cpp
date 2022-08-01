// Copyright (c) 2022 Dhiraj Wishal

#include "ShaderBuilder/SPIRVSource.hpp"

namespace ShaderBuilder
{
	std::string SPIRVSource::getSourceAssembly() const
	{
		std::stringstream finalTransform;
		finalTransform << "; Magic:     0x07230203 (SPIR-V)" << std::endl;
		finalTransform << "; Version:   0x00010000 (Version: 1.0.0)" << std::endl;
		finalTransform << "; Generator: 0x00000000 (Shader Builder; 1)" << std::endl;
		finalTransform << "; Bound:     " << m_TypeDeclarations.getUniqueIDCount() + 1 << std::endl;
		finalTransform << "; Schema:    0" << std::endl;
		finalTransform << std::endl;

		finalTransform << "; Core instructions." << std::endl;
		finalTransform << m_CompatibilityInstructions.getString();
		finalTransform << m_ExtensionInstructions.getString();
		finalTransform << m_ExtInstImportInstructions.getString();
		finalTransform << m_MemoryModel << std::endl;
		finalTransform << m_EntryPoints.getString();
		finalTransform << m_ExecutionModes.getString();
		finalTransform << std::endl;

		finalTransform << "; Debug information." << std::endl;
		finalTransform << m_DebugSources.getString();
		finalTransform << m_DebugNames.getString();
		finalTransform << m_DebugModuleProcessedInstructions.getString();
		finalTransform << std::endl;

		finalTransform << "; Annotations." << std::endl;
		finalTransform << m_Annotations.getString();
		finalTransform << std::endl;

		finalTransform << "; Type declarations." << std::endl;
		finalTransform << m_TypeDeclarations.getString();
		finalTransform << std::endl;

		finalTransform << "; Function declarations." << std::endl;
		finalTransform << m_FunctionDeclarations.getString();
		finalTransform << std::endl;

		finalTransform << "; Function definitions." << std::endl;
		finalTransform << m_FunctionDefinitions.getString();
		finalTransform << std::endl;

		return finalTransform.str();
	}

} // namespace ShaderBuilder