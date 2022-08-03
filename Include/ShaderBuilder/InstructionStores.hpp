// Copyright (c) 2022 Dhiraj Wishal

#pragma once

#include <sstream>
#include <string>
#include <set>
#include <cstdint>

namespace ShaderBuilder
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
		virtual void insert(std::string&& line);

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
		void insert(std::string&& line) override;

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

	/**
	 * Block instruction class.
	 * This class is used to store information regarding a single block.
	 */
	class BlockInstructions final : public InstructionStore {};
} // namespace ShaderBuilder