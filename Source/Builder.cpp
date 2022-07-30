// Copyright (c) 2022 Dhiraj Wishal

#include "ShaderBuilder/Builder.hpp"
#include "ShaderBuilder/BuilderError.hpp"

#include <future>

namespace /* anonymous */
{
	/**
	 * Get the execution model from the shader type.
	 *
	 * @param type The shader type.
	 * @return The execution model string.
	 */
	std::string_view GetExecutionModel(ShaderBuilder::ShaderType type)
	{
		switch (type)
		{
		case ShaderBuilder::ShaderType::Vertex:						return "Vertex";
		case ShaderBuilder::ShaderType::TessellationControl:		return "Vertex";
		case ShaderBuilder::ShaderType::TessellationEvaluation:		return "Vertex";
		case ShaderBuilder::ShaderType::Geometry:					return "Vertex";
		case ShaderBuilder::ShaderType::Fragment:					return "Vertex";
		case ShaderBuilder::ShaderType::Compute:					return "Vertex";
		default:													throw ShaderBuilder::BuilderError("Invalid shader type!");
		}
	}
}

namespace ShaderBuilder
{
	Builder::Builder(Configuration config /*= Configuration()*/)
	{
		m_OpCompatibilityInstructions << "OpCapability Shader" << std::endl;
		m_OpExtInstImportInstructions << "%1 = OpExtInstImport \"GLSL.std.450\"" << std::endl;
		m_OpMemoryModel = "OpMemoryModel Logical GLSL450";
	}

	std::string Builder::getString() const
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
		finalTransform << m_DebugInstructions.m_Sources.str();
		finalTransform << m_DebugInstructions.m_Names.str();
		finalTransform << m_DebugInstructions.m_ModuleProcessedInstructions.str();
		finalTransform << std::endl;

		// Insert the annotations.
		finalTransform << "; Annotations." << std::endl;
		finalTransform << m_Annotations.str();
		finalTransform << std::endl;

		// Insert the type declarations.
		finalTransform << "; Type declarations." << std::endl;
		finalTransform << m_TypeDeclarations.str() << std::endl;
		finalTransform << std::endl;

		// Insert the function declarations.
		finalTransform << "; Function declarations." << std::endl;
		for (const auto& declaration : m_FunctionDefinitions)
			finalTransform << declaration.str();
		finalTransform << std::endl;

		// Insert the function definitions.
		finalTransform << "; Function definitions." << std::endl;
		for (const auto& definitions : m_FunctionDefinitions)
			finalTransform << definitions.str();
		finalTransform << std::endl;

		return finalTransform.str();
	}
} // namespace ShaderBuilder