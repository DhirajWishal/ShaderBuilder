// Copyright (c) 2022 Dhiraj Wishal

#include "ShaderBuilder/SPIRVSource.hpp"
#include "ShaderBuilder/BuilderError.hpp"

#define XXH_INLINE_ALL
#include <xxhash.h>

#include <set>

namespace /* anonymous */
{
	/**
	 * Instruction store structure.
	 * This structure is the base class for all the instruction storages.
	 */
	class InstructionStore
	{
	public:
		/**
		 * Insert a new line to the store.
		 *
		 * @param line The line to insert.
		 */
		virtual void insert(std::string&& line) { m_Instructions << std::move(line) << std::endl; }

		/**
		 * Get the string stream as a string.
		 *
		 * @return The string.
		 */
		[[nodiscard]] std::string getString() const { return m_Instructions.str(); }

	protected:
		std::stringstream m_Instructions;
	};

	/**
	 * Compatibility instructions store.
	 */
	class CompatibiltyInstructions final : public InstructionStore {};

	/**
	 * Extension instructions store.
	 */
	class ExtensionInstructions final : public InstructionStore {};

	/**
	 * ExtInstImport instructions store.
	 */
	class ExtInstImportInstructions final : public InstructionStore {};

	/**
	 * Entry point instructions store.
	 */
	class EntryPointInstructions final : public InstructionStore {};

	/**
	 * Execution instructions store.
	 */
	class ExecutionModeInstructions final : public InstructionStore {};

	/**
	 * Debug source instructions store.
	 */
	class DebugSourceInstructions final : public InstructionStore {};

	/**
	 * Debug name instructions store.
	 */
	class DebugNameInstructions final : public InstructionStore {};

	/**
	 * Debug module instructions store.
	 */
	class DebugModuleProcesseInstructions final : public InstructionStore {};

	/**
	 * Annotation instructions store.
	 */
	class AnnotationInstructions final : public InstructionStore {};

	/**
	 * Type declaration instructions store.
	 */
	class TypeDeclarationInstructions final : public InstructionStore
	{
	public:
		/**
		 * Insert a new line to the store.
		 * This will only insert the line if it haven't been inserted before.
		 *
		 * @param line The line to insert.
		 */
		void insert(std::string&& line) override
		{
			if (m_AvailabilityMap.insert(static_cast<uint64_t>(XXH64(line.data(), line.size(), 0))).second)
				m_Instructions << std::move(line) << std::endl;
		}

		/**
		 * Get the number of unique IDs used by the source.
		 *
		 * @return The unique ID count.
		 */
		[[nodiscard]] uint64_t getUniqueIDCount() const { return m_AvailabilityMap.size(); }

	private:
		std::set<uint64_t> m_AvailabilityMap;
	};

	/**
	 * Function declaration instructions store.
	 */
	class FunctionDeclarationInstructions final : public InstructionStore {};

	/**
	 * Function definition instructions store.
	 */
	class FunctionDefinitionInstructions final : public InstructionStore {};
}

namespace ShaderBuilder
{
	std::string SPIRVSource::prepare() const
	{
		CompatibiltyInstructions compatibilityInstructions;
		ExtensionInstructions extensionInstructions;
		ExtInstImportInstructions extInstImportInstructions;
		EntryPointInstructions entryPoints;
		ExecutionModeInstructions executionModes;

		DebugSourceInstructions debugSources;
		DebugNameInstructions debugNames;
		DebugModuleProcesseInstructions debugModuleProcessedInstructions;

		AnnotationInstructions annotations;
		TypeDeclarationInstructions typeDeclarations;

		std::string memoryModel;

		FunctionDeclarationInstructions functionDeclarations;
		FunctionDefinitionInstructions functionDefinitions;

		auto assembly = getSourceAssembly();
		auto pCategory = m_SourceCategories.begin();
		for (std::string line; std::getline(assembly, line); ++pCategory)
		{
			switch (*pCategory)
			{
			case ShaderBuilder::AssemblyCategory::CompatibilityInstructions:
				compatibilityInstructions.insert(std::move(line));
				break;

			case ShaderBuilder::AssemblyCategory::ExtensionInstructions:
				extensionInstructions.insert(std::move(line));
				break;

			case ShaderBuilder::AssemblyCategory::ExtInstImportInstructions:
				extInstImportInstructions.insert(std::move(line));
				break;

			case ShaderBuilder::AssemblyCategory::MemoryModel:
				memoryModel = std::move(line);
				break;

			case ShaderBuilder::AssemblyCategory::EntryPoints:
				entryPoints.insert(std::move(line));
				break;

			case ShaderBuilder::AssemblyCategory::ExecutionModes:
				executionModes.insert(std::move(line));
				break;

			case ShaderBuilder::AssemblyCategory::DebugSources:
				debugSources.insert(std::move(line));
				break;

			case ShaderBuilder::AssemblyCategory::DebugNames:
				debugNames.insert(std::move(line));
				break;

			case ShaderBuilder::AssemblyCategory::DebugModuleProcessed:
				debugModuleProcessedInstructions.insert(std::move(line));
				break;

			case ShaderBuilder::AssemblyCategory::Annotations:
				annotations.insert(std::move(line));
				break;

			case ShaderBuilder::AssemblyCategory::TypeDeclarations:
				typeDeclarations.insert(std::move(line));
				break;

			case ShaderBuilder::AssemblyCategory::FunctionDeclarations:
				functionDeclarations.insert(std::move(line));
				break;

			case ShaderBuilder::AssemblyCategory::FunctionDefinitions:
				functionDefinitions.insert(std::move(line));
				break;

			default:
				throw BuilderError("Invalid assembly category!");
			}
		}

		std::stringstream finalTransform;
		finalTransform << "; Magic:     0x07230203 (SPIR-V)" << std::endl;
		finalTransform << "; Version:   0x00010000 (Version: 1.0.0)" << std::endl;
		finalTransform << "; Generator: 0x00000000 (Shader Builder; 1)" << std::endl;
		finalTransform << "; Bound:     " << typeDeclarations.getUniqueIDCount() + 1 << std::endl;
		finalTransform << "; Schema:    0" << std::endl;
		finalTransform << std::endl;

		finalTransform << "; Core instructions." << std::endl;
		finalTransform << compatibilityInstructions.getString();
		finalTransform << extensionInstructions.getString();
		finalTransform << extInstImportInstructions.getString();
		finalTransform << memoryModel;
		finalTransform << entryPoints.getString();
		finalTransform << executionModes.getString();

		finalTransform << "; Debug information." << std::endl;
		finalTransform << debugSources.getString();
		finalTransform << debugNames.getString();
		finalTransform << debugModuleProcessedInstructions.getString();

		finalTransform << "; Annotations." << std::endl;
		finalTransform << annotations.getString();

		finalTransform << "; Type declarations." << std::endl;
		finalTransform << typeDeclarations.getString();

		finalTransform << "; Function declarations." << std::endl;
		finalTransform << functionDeclarations.getString();

		finalTransform << "; Function definitions." << std::endl;
		finalTransform << functionDefinitions.getString();

		return finalTransform.str();
	}
} // namespace ShaderBuilder