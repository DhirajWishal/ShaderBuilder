// Copyright (c) 2022 Dhiraj Wishal

#pragma once

#include <nlohmann/json.hpp>

#include <optional>
#include <stack>

namespace ShaderBuilder
{
	class FunctionBuilder;

	/**
	 * SPIR-V Source class.
	 * This contains all the source information provided by the data types and others.
	 */
	class SPIRVSource final
	{
	public:
		/**
		 * Default constructor.
		 */
		SPIRVSource();

		/**
		 * Insert a new shader capability.
		 *
		 * @param instruction The shader capability.
		 */
		void insertCapability(std::string&& instruction);

		/**
		 * Insert a new extension.
		 *
		 * @param instruction The extension to insert.
		 */
		void insertExtension(std::string&& instruction);

		/**
		 * Insert a new extended instruction set.
		 *
		 * @param instruction The instruction.
		 */
		void insertExtendedInstructionSet(std::string&& instruction);

		/**
		 * Set the memory model.
		 *
		 * @param addressingModel The addressing model to set.
		 * @param memoryModel The memory model to set.
		 */
		void setMemoryModel(std::string&& instruction);

		/**
		 * Insert an entry point.
		 *
		 * @param instruction The instruction.
		 */
		void insertEntryPoint(std::string&& instruction);

		/**
		 * Insert a new execution mode.
		 *
		 * @param instruction The instruction.
		 */
		void insertExecutionMode(std::string&& instruction);

		/**
		 * Insert a new debug name.
		 *
		 * @param instruction The instruction.
		 */
		void insertName(std::string&& instruction);

		/**
		 * Insert a new annotation.
		 *
		 * @param instruction The instruction.
		 */
		void insertAnnotation(std::string&& instruction);

		/**
		 * Insert a new type.
		 *
		 * @param instruction The instruction.
		 */
		void insertType(std::string&& instruction);

	public:
		/**
		 * Get the source assembly.
		 *
		 * @return The assembly string
		 */
		[[nodiscard]] std::string getSourceAssembly() const;

		/**
		 * Get a unique ID.
		 *
		 * @return The unique ID.
		 */
		[[nodiscard]] uint64_t getUniqueID() { return m_UniqueID++; }

	private:
		std::stack<FunctionBuilder> m_FunctionBuilders;
		uint64_t m_UniqueID = 1;

		std::vector<std::string> m_Capabilities;
		std::vector<std::string> m_Extensions;
		std::vector<std::string> m_ExtendedInstructions;

		std::string m_MemoryModel;

		std::vector<std::string> m_EntryPoints;
		std::vector<std::string> m_ExecutionModes;
		std::vector<std::string> m_DebugNames;
		std::vector<std::string> m_Annotations;
		std::vector<std::string> m_Types;
	};
} // namespace ShaderBuilder