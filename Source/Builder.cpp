// Copyright (c) 2022 Dhiraj Wishal

#include "ShaderBuilder/Builder.hpp"
#include "ShaderBuilder/BuilderError.hpp"

#include <spirv-tools/libspirv.hpp>

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

	std::vector<uint32_t> Builder::compile() const
	{
		const auto shaderCode = getString();

		spvtools::SpirvTools tools(SPV_ENV_UNIVERSAL_1_3);
		tools.SetMessageConsumer([](spv_message_level_t level, const char*, const spv_position_t& position, const char* message) { throw BuilderError(message); });

		std::vector<uint32_t> spirv;
		if (!tools.Assemble(shaderCode, &spirv))
			throw BuilderError("Failed the assemble the assembly!");

		if (!tools.Validate(spirv))
			throw BuilderError("The generated SPIR-V is invalid!");

		return spirv;
	}

} // namespace ShaderBuilder