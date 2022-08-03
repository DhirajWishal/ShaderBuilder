// Copyright (c) 2022 Dhiraj Wishal

#include "ShaderBuilder/SPIRVSource.hpp"

namespace ShaderBuilder
{
	SPIRVSource::SPIRVSource()
	{
		m_SourceJSON["compatibilities"] = {};
		m_SourceJSON["extensions"] = {};
		m_SourceJSON["extendedInstructions"] = {};
		m_SourceJSON["memoryModel"] = {};
		m_SourceJSON["entryPoint"] = {};
		m_SourceJSON["executionModes"] = {};
		m_SourceJSON["debug"]["names"] = {};
		m_SourceJSON["debug"]["memberNames"] = {};
		m_SourceJSON["annotations"] = {};
		m_SourceJSON["types"] = {};
	}

	void SPIRVSource::insertCompatibility(const std::string& compatibility)
	{
		m_SourceJSON["compatibilities"].emplace_back(compatibility);
	}

	void SPIRVSource::insertExtension(const std::string& extension)
	{
		m_SourceJSON["extensions"].emplace_back(extension);
	}

	void SPIRVSource::insertExtendedInstructionSet(const std::string& extensionName, const std::string& extension)
	{
		Json object;
		object["name"] = extensionName;
		object["instructions"] = extension;

		m_SourceJSON["extendedInstructions"].emplace_back(object);
	}

	void SPIRVSource::setMemoryModel(const std::string& addressingModel, const std::string& memoryModel)
	{
		auto& node = m_SourceJSON["memoryModel"];
		node["addressingModel"] = addressingModel;
		node["memoryModel"] = memoryModel;
	}

	void SPIRVSource::insertEntryPoint(const std::string& executionModel, const std::string& entryPointIdentifier, const std::string& name, const std::vector<std::string>& attributes)
	{
		Json object;
		object["executionModel"] = executionModel;
		object["identifier"] = entryPointIdentifier;
		object["name"] = name;
		object["attributes"] = attributes;

		m_SourceJSON["entryPoints"].emplace_back(object);
	}

	void SPIRVSource::insertExecutionMode(const std::string& mode)
	{
		m_SourceJSON["executionModes"].emplace_back(mode);
	}

	void SPIRVSource::insertName(const std::string& type, const std::string& name)
	{
		Json object;
		object["type"] = type;
		object["name"] = name;

		m_SourceJSON["debug"]["names"].emplace_back(object);
	}

	void SPIRVSource::insertMemberName(const std::string& type, uint32_t index, const std::string& name)
	{
		Json object;
		object["type"] = type;
		object["index"] = index;
		object["name"] = name;

		m_SourceJSON["debug"]["memberNames"].emplace_back(object);
	}

	void SPIRVSource::insertAnnotation(const std::string& annotation)
	{
		m_SourceJSON["annotations"].emplace_back(annotation);
	}

	void SPIRVSource::insertType(const std::string& name, const std::string& declaration)
	{
		m_SourceJSON["types"][name] = declaration;
	}

	void SPIRVSource::insertFunctionDefinition(Json&& object)
	{
		m_SourceJSON["functionDefinitions"].emplace_back(std::move(object));
	}

	std::string SPIRVSource::getSourceAssembly() const
	{
		std::stringstream finalTransform;
		finalTransform << "; Magic:     0x07230203 (SPIR-V)" << std::endl;
		finalTransform << "; Version:   0x00010000 (Version: 1.0.0)" << std::endl;
		finalTransform << "; Generator: 0x00000000 (Shader Builder; 1)" << std::endl;
		// finalTransform << "; Bound:     " << m_TypeDeclarations.getUniqueIDCount() + 1 << std::endl;
		finalTransform << "; Schema:    0" << std::endl;
		finalTransform << std::endl;

		finalTransform << "; Core instructions." << std::endl;

		// Insert the compatibilities.
		for (const auto& object : m_SourceJSON["compatibilities"])
			finalTransform << "OpCapability " << object.get<std::string>() << std::endl;

		// Insert the extensions.
		for (const auto& object : m_SourceJSON["extensions"])
			finalTransform << "OpExtension " << object.get<std::string>() << std::endl;

		// Insert the extended instructions.
		for (const auto& object : m_SourceJSON["extendedInstructions"])
			finalTransform << object["name"].get<std::string>() << " = OpExtInstImport \"" << object["instructions"].get<std::string>() << "\"" << std::endl;

		// Set the memory model.
		if (m_SourceJSON["memoryModel"].contains("addressingModel") && m_SourceJSON["memoryModel"].contains("memoryModel"))
			finalTransform << "OpMemoryModel " << m_SourceJSON["memoryModel"]["addressingModel"].get<std::string>() << " " << m_SourceJSON["memoryModel"]["memoryModel"].get<std::string>() << std::endl;

		// Insert the entry points.
		for (const auto& object : m_SourceJSON["entryPoints"])
		{
			finalTransform << "OpEntryPoint " << object["executionModel"].get<std::string>() << " " << object["identifier"].get<std::string>() << " \"" << object["name"].get<std::string>() << "\" ";

			for (const auto& attribute : object["attributes"])
				finalTransform << "%" << attribute.get<std::string>() << " ";

			finalTransform << std::endl;
		}

		// Insert the execution modes.
		for (const auto& object : m_SourceJSON["executionModes"])
			finalTransform << "OpExecutionMode " << object.get<std::string>() << std::endl;

		finalTransform << std::endl;

		// Insert the debug names.
		finalTransform << "; Debug information." << std::endl;
		for (const auto& object : m_SourceJSON["debug"]["names"])
			finalTransform << "OpName " << object["type"].get<std::string>() << " \"" << object["name"].get<std::string>() << "\"" << std::endl;

		// Insert the debug member names.
		for (const auto& object : m_SourceJSON["debug"]["memberNames"])
			finalTransform << "OpMemberName " << object["type"].get<std::string>() << " " << object["index"].get<uint32_t>() << " \"" << object["name"].get<std::string>() << "\"" << std::endl;

		finalTransform << std::endl;

		// Insert the annotations.
		finalTransform << "; Annotations." << std::endl;
		for (const auto& object : m_SourceJSON["annotations"])
			finalTransform << object.get<std::string>() << std::endl;

		finalTransform << std::endl;

		// Insert type information.
		finalTransform << "; Type declarations." << std::endl;
		for (const auto& [key, value] : m_SourceJSON["types"].items())
			finalTransform << key << " = " << value.get<std::string>() << std::endl;

		finalTransform << std::endl;

		// Insert function declarations.
		finalTransform << "; Function declarations." << std::endl;
		finalTransform << std::endl;

		// Insert function definitions.
		finalTransform << "; Function definitions." << std::endl;
		for (const auto& object : m_SourceJSON["functionDefinitions"])
		{
			finalTransform << object["declaration"].get<std::string>() << std::endl;
			finalTransform << "%variables = OpLabel" << std::endl;

			for (const auto& [name, declaration] : object["variables"].items())
				finalTransform << "%" << name << " = " << declaration.get<std::string>() << std::endl;

			finalTransform << "OpReturn" << std::endl;
			finalTransform << "OpFunctionEnd" << std::endl;
		}
		finalTransform << std::endl;

		return finalTransform.str();
	}

	std::string SPIRVSource::getJSON() const
	{
		return m_SourceJSON.dump(2);
	}

} // namespace ShaderBuilder