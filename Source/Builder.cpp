// Copyright (c) 2022 Dhiraj Wishal

#include "ShaderBuilder/Builder.hpp"
#include "ShaderBuilder/BuilderError.hpp"

#include <spirv-tools/libspirv.hpp>
#include <spirv-tools/optimizer.hpp>

namespace /* anonymous */
{
	/**
	 * Get the addressing model string.
	 *
	 * @param model The addressing model.
	 * @return The addressing model string.
	 */
	const char* GetAddressingModel(ShaderBuilder::AddressingModel model)
	{
		switch (model)
		{
		case ShaderBuilder::AddressingModel::Logical:							return "Logical ";
		case ShaderBuilder::AddressingModel::Physical32:						return "Physical32 ";
		case ShaderBuilder::AddressingModel::Physical64:						return "Physical64 ";
		case ShaderBuilder::AddressingModel::PhysicalStorageBuffer64:			return "PhysicalStorageBuffer64 ";
		default:																throw ShaderBuilder::BuilderError("Invalid addressing model!");
		}
	}

	/**
	 * Get the memory model string.
	 *
	 * @param model The memory model.
	 * @return The memory model string.
	 */
	const char* GetMemoryModel(ShaderBuilder::MemoryModel model)
	{
		switch (model)
		{
		case ShaderBuilder::MemoryModel::Simple:								return "Simple";
		case ShaderBuilder::MemoryModel::GLSL450:								return "GLSL450";
		case ShaderBuilder::MemoryModel::OpenCL:								return "OpenCL";
		case ShaderBuilder::MemoryModel::Vulkan:								return "Vulkan";
		default:																throw ShaderBuilder::BuilderError("Invalid memory model!");
		}
	}
}

namespace ShaderBuilder
{
	Builder::Builder(Configuration config /*= Configuration()*/)
	{
		m_OpCompatibilityInstructions << "OpCapability Shader" << std::endl;
		m_OpExtInstImportInstructions << "%1 = OpExtInstImport \"GLSL.std.450\"" << std::endl;
		m_OpMemoryModel = std::string("OpMemoryModel ") + GetAddressingModel(config.m_AddressingModel) + GetMemoryModel(config.m_MemoryModel);
	}

	std::string Builder::getString() const
	{
		std::stringstream finalTransform;

		// Setup the first words of the physical layout.
		finalTransform << "; Magic:     0x07230203 (SPIR-V)" << std::endl;
		finalTransform << "; Version:   0x00010000 (Version: 1.0.0)" << std::endl;
		finalTransform << "; Generator: 0x00000000 (Shader Builder; 1)" << std::endl;
		finalTransform << "; Bound:     " << m_TypeAvailability.size() + 1 << std::endl;
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

	SPIRVBinary Builder::compile(bool shouldOptimize /*= true*/) const
	{
		auto errorMessageConsumer = [](spv_message_level_t level, const char*, const spv_position_t& position, const char* message) { throw BuilderError(message); };

		auto tools = spvtools::SpirvTools(SPV_ENV_UNIVERSAL_1_6);
		tools.SetMessageConsumer(errorMessageConsumer);

		std::vector<uint32_t> spirv;
		const auto shaderCode = getString();

		if (!tools.Assemble(shaderCode, &spirv))
			throw BuilderError("Failed the assemble the assembly!");

		if (!tools.Validate(spirv))
			throw BuilderError("The generated SPIR-V is invalid!");

		// Optimize the binary if requested to.
		if (shouldOptimize)
		{
			auto optimizer = spvtools::Optimizer(SPV_ENV_UNIVERSAL_1_6);
			optimizer.SetMessageConsumer(errorMessageConsumer);

			// Configure it.
			optimizer.RegisterPass(spvtools::CreateFreezeSpecConstantValuePass())
				.RegisterPass(spvtools::CreateUnifyConstantPass())
				.RegisterPass(spvtools::CreateStripNonSemanticInfoPass())
				.RegisterPass(spvtools::CreateEliminateDeadFunctionsPass())
				.RegisterPass(spvtools::CreateEliminateDeadMembersPass())
				.RegisterPass(spvtools::CreateStripDebugInfoPass());

			// Optimizeee!
			if (!optimizer.Run(spirv.data(), spirv.size(), &spirv))
				throw BuilderError("Failed to optimize the binary!");
		}

		return SPIRVBinary(std::move(spirv));
	}

} // namespace ShaderBuilder