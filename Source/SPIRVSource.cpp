// Copyright (c) 2022 Dhiraj Wishal

#include "ShaderBuilder/SPIRVSource.hpp"

#include <sstream>

namespace ShaderBuilder
{
	void FunctionBlock::enableRecording()
	{
		m_Definition.setShouldRecord(true);
		m_Instructions.setShouldRecord(true);
		m_Parameters.setShouldRecord(true);
		m_Variables.setShouldRecord(true);
	}

	void FunctionBlock::disableRecording()
	{
		m_Definition.setShouldRecord(false);
		m_Instructions.setShouldRecord(false);
		m_Parameters.setShouldRecord(false);
		m_Variables.setShouldRecord(false);
	}

	void SPIRVSource::insertCapability(std::string&& instruction)
	{
		m_Capabilities.insert(std::move(instruction));
	}

	void SPIRVSource::insertExtension(std::string&& instruction)
	{
		m_Extensions.insert(std::move(instruction));
	}

	void SPIRVSource::insertExtendedInstructionSet(std::string&& instruction)
	{
		m_ExtendedInstructions.insert(std::move(instruction));
	}

	void SPIRVSource::setMemoryModel(std::string&& instruction)
	{
		m_MemoryModel = std::move(instruction);
	}

	void SPIRVSource::insertEntryPoint(std::string&& instruction)
	{
		m_EntryPoints.insert(std::move(instruction));
	}

	void SPIRVSource::insertExecutionMode(std::string&& instruction)
	{
		m_ExecutionModes.insert(std::move(instruction));
	}

	void SPIRVSource::insertName(std::string&& instruction)
	{
		m_DebugNames.insert(std::move(instruction));
	}

	void SPIRVSource::insertAnnotation(std::string&& instruction)
	{
		m_Annotations.insert(std::move(instruction));
	}

	void SPIRVSource::insertType(std::string&& instruction)
	{
		m_Types.insert(std::move(instruction));
	}

	ShaderBuilder::FunctionBlock& SPIRVSource::pushFunctionBlock()
	{
		return m_FunctionBlockStack.emplace();
	}

	ShaderBuilder::FunctionBlock& SPIRVSource::getCurrentFunctionBlock()
	{
		if (m_FunctionBlockStack.empty())
			return m_FunctionBlockStack.emplace();

		return m_FunctionBlockStack.top();
	}

	void SPIRVSource::finishFunctionBlock()
	{
		m_FunctionBlocks.emplace_back(m_FunctionBlockStack.top());
		m_FunctionBlockStack.pop();
	}

	std::string SPIRVSource::getSourceAssembly() const
	{
		std::stringstream finalTransform;
		finalTransform << "; Magic:     0x07230203 (SPIR-V)" << std::endl;
		finalTransform << "; Version:   0x00010000 (Version: 1.0.0)" << std::endl;
		finalTransform << "; Generator: 0x00000000 (Shader Builder; 1)" << std::endl;
		finalTransform << "; Schema:    0" << std::endl;

		// Insert the capabilities.
		finalTransform << std::endl << "; Capabilities." << std::endl;
		for (const auto& instruction : m_Capabilities)
			finalTransform << instruction << std::endl;

		// Insert the extensions.
		finalTransform << std::endl << "; Extensions." << std::endl;
		for (const auto& instruction : m_Extensions)
			finalTransform << instruction << std::endl;

		// Insert the extended instructions.
		finalTransform << std::endl << "; Extended Instructions." << std::endl;
		for (const auto& instruction : m_ExtendedInstructions)
			finalTransform << instruction << std::endl;

		// Set the memory model.
		finalTransform << std::endl << "; Memory Model." << std::endl;
		finalTransform << m_MemoryModel << std::endl;

		// Insert the entry points.
		finalTransform << std::endl << "; Entry Points." << std::endl;
		for (const auto& instruction : m_EntryPoints)
			finalTransform << instruction << std::endl;

		// Insert the execution modes.
		finalTransform << std::endl << "; Execution modes." << std::endl;
		for (const auto& instruction : m_ExecutionModes)
			finalTransform << instruction << std::endl;

		// Insert the debug names.
		finalTransform << std::endl << "; Debug information." << std::endl;
		for (const auto& instruction : m_DebugNames)
			finalTransform << instruction << std::endl;

		// Insert the annotations.
		finalTransform << std::endl << "; Annotations." << std::endl;
		for (const auto& instruction : m_Annotations)
			finalTransform << instruction << std::endl;

		// Insert type information.
		finalTransform << std::endl << "; Type declarations." << std::endl;
		for (const auto& instruction : m_Types)
			finalTransform << instruction << std::endl;

		// Insert function declarations.
		finalTransform << std::endl << "; Function declarations." << std::endl;
		for (const auto& instruction : m_FunctionDeclarations)
			finalTransform << instruction << std::endl;

		// Insert function definitions.
		finalTransform << std::endl << std::endl << "; Function definitions." << std::endl;
		for (const auto& block : m_FunctionBlocks)
		{
			// Insert the function definition.
			for (const auto& instruction : block.m_Definition)
				finalTransform << instruction << std::endl;

			// Insert the parameters.
			for (const auto& instruction : block.m_Parameters)
				finalTransform << instruction << std::endl;

			// Insert the first block containing the variables.
			finalTransform << "%first_block_" << block.m_Name << " = OpLabel" << std::endl;

			// Insert the variables.
			for (const auto& instruction : block.m_Variables)
				finalTransform << instruction << std::endl;

			// Insert the instructions.
			for (const auto& instruction : block.m_Instructions)
				finalTransform << instruction << std::endl;

			// End the function definition.
			finalTransform << "OpFunctionEnd" << std::endl << std::endl;
		}

		return finalTransform.str();
	}
} // namespace ShaderBuilder