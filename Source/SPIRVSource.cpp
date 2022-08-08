// Copyright (c) 2022 Dhiraj Wishal

#include "ShaderBuilder/SPIRVSource.hpp"

#include <sstream>

namespace ShaderBuilder
{
	SPIRVSource::SPIRVSource()
	{
	}

	void SPIRVSource::insertCapability(std::string&& instruction)
	{
		m_Capabilities.emplace_back(std::move(instruction));
	}

	void SPIRVSource::insertExtension(std::string&& instruction)
	{
		m_Extensions.emplace_back(std::move(instruction));
	}

	void SPIRVSource::insertExtendedInstructionSet(std::string&& instruction)
	{
		m_ExtendedInstructions.emplace_back(std::move(instruction));
	}

	void SPIRVSource::setMemoryModel(std::string&& instruction)
	{
		m_MemoryModel = std::move(instruction);
	}

	void SPIRVSource::insertEntryPoint(std::string&& instruction)
	{
		m_EntryPoints.emplace_back(std::move(instruction));
	}

	void SPIRVSource::insertExecutionMode(std::string&& instruction)
	{
		m_ExecutionModes.emplace_back(std::move(instruction));
	}

	void SPIRVSource::insertName(std::string&& instruction)
	{
		m_DebugNames.emplace_back(std::move(instruction));
	}

	void SPIRVSource::insertAnnotation(std::string&& instruction)
	{
		m_Annotations.emplace_back(std::move(instruction));
	}

	void SPIRVSource::insertType(std::string&& instruction)
	{
		m_Types.emplace_back(std::move(instruction));
	}

	std::string SPIRVSource::getSourceAssembly() const
	{
		std::stringstream finalTransform;
		finalTransform << "; Magic:     0x07230203 (SPIR-V)" << std::endl;
		finalTransform << "; Version:   0x00010000 (Version: 1.0.0)" << std::endl;
		finalTransform << "; Generator: 0x00000000 (Shader Builder; 1)" << std::endl;
		finalTransform << "; Schema:    0" << std::endl;
		finalTransform << std::endl;

		finalTransform << "; Core instructions." << std::endl;

		// Insert the compatibilities.
		for (const auto& instruction : m_Capabilities)
			finalTransform << instruction << std::endl;

		// Insert the extensions.
		for (const auto& instruction : m_Extensions)
			finalTransform << instruction << std::endl;

		// Insert the extended instructions.
		for (const auto& instruction : m_ExtendedInstructions)
			finalTransform << instruction << std::endl;

		// Set the memory model.
		finalTransform << m_MemoryModel << std::endl;

		// Insert the entry points.
		for (const auto& instruction : m_EntryPoints)
			finalTransform << instruction << std::endl;

		// Insert the execution modes.
		for (const auto& instruction : m_ExecutionModes)
			finalTransform << instruction << std::endl;

		finalTransform << std::endl;

		// Insert the debug names.
		finalTransform << "; Debug information." << std::endl;
		for (const auto& instruction : m_DebugNames)
			finalTransform << instruction << std::endl;

		finalTransform << std::endl;

		// Insert the annotations.
		finalTransform << "; Annotations." << std::endl;
		for (const auto& instruction : m_Annotations)
			finalTransform << instruction << std::endl;

		finalTransform << std::endl;

		// Insert type information.
		finalTransform << "; Type declarations." << std::endl;
		for (const auto& instruction : m_Types)
			finalTransform << instruction << std::endl;

		finalTransform << std::endl;

		// Insert function declarations.
		finalTransform << "; Function declarations." << std::endl;
		finalTransform << std::endl;

		// Insert function definitions.
		// finalTransform << "; Function definitions." << std::endl;
		// for (const auto& object : m_SourceJSON["functionDefinitions"])
		// {
		// 	finalTransform << object["declaration"].get<std::string>() << std::endl;
		// 	finalTransform << "%" << object["firstBlock"].get<std::string>() << " = OpLabel" << std::endl;
		// 
		// 	for (const auto& [name, declaration] : object["variables"].items())
		// 		finalTransform << "%" << name << " = " << declaration.get<std::string>() << std::endl;
		// 
		// 	// Return a value if we have defined it.
		// 	if (object.contains("return"))
		// 		finalTransform << "OpReturnValue " << object["return"].get<std::string>() << std::endl;
		// 
		// 	// Else return nothing.
		// 	else
		// 		finalTransform << "OpReturn" << std::endl;
		// 
		// 	// End the function.
		// 	finalTransform << "OpFunctionEnd" << std::endl;
		// }

		return finalTransform.str();
	}
} // namespace ShaderBuilder