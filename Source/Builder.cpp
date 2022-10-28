// Copyright (c) 2022 Dhiraj Wishal

#include "ShaderBuilder/Builder.hpp"
#include "ShaderBuilder/BuilderError.hpp"

#include <spirv-tools/libspirv.hpp>
#include <spirv-tools/optimizer.hpp>

#ifdef SB_DEBUG
#include <iostream>

#endif

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
		m_Source.insertCapability("OpCapability Shader");
		m_Source.insertExtendedInstructionSet("%glsl = OpExtInstImport \"GLSL.std.450\"");
		m_Source.setMemoryModel(fmt::format("OpMemoryModel {} {}", GetAddressingModel(config.m_AddressingModel), GetMemoryModel(config.m_MemoryModel)));
	}

	Builder::~Builder()
	{
	}

	std::string Builder::getString() const
	{
		return m_Source.getSourceAssembly();
	}

	std::string Builder::getJSON() const
	{
		return m_Source.getSourceAssembly();
	}

	SPIRVBinary Builder::compile(OptimizationFlags flags /*= OptimizationFlags::Release*/) const
	{
		auto errorMessageConsumer = [](spv_message_level_t level, const char* source, const spv_position_t& position, const char* message) { throw BuilderError(message); };

		auto tools = spvtools::SpirvTools(SPV_ENV_UNIVERSAL_1_6);
		tools.SetMessageConsumer(errorMessageConsumer);

		const auto shaderCode = getString();

#ifdef SB_DEBUG
		std::cout << "-------------------- Debug Output --------------------" << std::endl;
		std::cout << shaderCode << std::endl;
		std::cout << "-------------------- Debug Output --------------------" << std::endl;

#endif

		std::vector<uint32_t> spirv;
		if (!tools.Assemble(shaderCode, &spirv))
			throw BuilderError("Failed the assemble the assembly!");

		if (!tools.Validate(spirv))
			throw BuilderError("The generated SPIR-V is invalid!");

		// Optimize the binary if requested to.
		if (flags != OptimizationFlags::None)
		{
			auto optimizer = spvtools::Optimizer(SPV_ENV_UNIVERSAL_1_6);
			optimizer.SetMessageConsumer(errorMessageConsumer);

			// Configure it.
			if (flags & OptimizationFlags::FreezeCosntants)
				optimizer.RegisterPass(spvtools::CreateFreezeSpecConstantValuePass());

			if (flags & OptimizationFlags::UnifyConstants)
				optimizer.RegisterPass(spvtools::CreateUnifyConstantPass());

			if (flags & OptimizationFlags::StripNonSemanticInfo)
				optimizer.RegisterPass(spvtools::CreateStripNonSemanticInfoPass());

			if (flags & OptimizationFlags::EliminateDeadFunctions)
				optimizer.RegisterPass(spvtools::CreateEliminateDeadFunctionsPass());

			if (flags & OptimizationFlags::EliminateDeadMembers)
				optimizer.RegisterPass(spvtools::CreateEliminateDeadMembersPass());

			if (flags & OptimizationFlags::StripDebugInfo)
				optimizer.RegisterPass(spvtools::CreateStripDebugInfoPass());

			// Optimize!
			if (!optimizer.Run(spirv.data(), spirv.size(), &spirv))
				throw BuilderError("Failed to optimize the binary!");
		}

		return SPIRVBinary(std::move(spirv));
	}

} // namespace ShaderBuilder