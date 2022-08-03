// Copyright (c) 2022 Dhiraj Wishal

#pragma once

#include <nlohmann/json.hpp>

namespace ShaderBuilder
{
	using Json = nlohmann::ordered_json;

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
		 * Insert a new shader compatibility.
		 *
		 * @param compatibility The shader compatibility.
		 */
		void insertCompatibility(const std::string& compatibility);

		/**
		 * Insert a new extension.
		 *
		 * @param extension The extension to insert.
		 */
		void insertExtension(const std::string& extension);

		/**
		 * Insert a new extended instruction set.
		 *
		 * @param extensionName The extension name.
		 * @param extension The instruction set.
		 */
		void insertExtendedInstructionSet(const std::string& extensionName, const std::string& extension);

		/**
		 * Set the memory model.
		 *
		 * @param addressingModel The addressing model to set.
		 * @param memoryModel The memory model to set.
		 */
		void setMemoryModel(const std::string& addressingModel, const std::string& memoryModel);

		/**
		 * Insert an entry point.
		 *
		 * @param executionModel The execution model.
		 * @param entryPointIdentifier The entry point identifier.
		 * @param name The name of the entry point.
		 * @param attributes The entry point's inputs and outputs.
		 */
		void insertEntryPoint(const std::string& executionModel, const std::string& entryPointIdentifier, const std::string& name, const std::vector<std::string>& attributes);

		/**
		 * Insert a new execution mode.
		 *
		 * @param mode The execution model
		 */
		void insertExecutionMode(const std::string& mode);

		/**
		 * Insert a new debug name.
		 *
		 * @param type The type of the name.
		 * @param name The name to insert.
		 */
		void insertName(const std::string& type, const std::string& name);

		/**
		 * Insert a new debug member name.
		 *
		 * @param type The type of the name.
		 * @param index The index of the variable in the structure.
		 * @param name The name to insert.
		 */
		void insertMemberName(const std::string& type, uint32_t index, const std::string& name);

		/**
		 * Insert a new annotation.
		 *
		 * @param annotation The annotation to insert.
		 */
		void insertAnnotation(const std::string& annotation);

		/**
		 * Insert a new type.
		 *
		 * @param name The name of the type.
		 * @param declaration The type declaration.
		 */
		void insertType(const std::string& name, const std::string& declaration);

		/**
		 * Insert a new function definition.
		 *
		 * @param object The function definition.
		 */
		void insertFunctionDefinition(Json&& object);

	public:
		/**
		 * Get the source assembly.
		 *
		 * @return The assembly string
		 */
		[[nodiscard]] std::string getSourceAssembly() const;

		/**
		 * Get the internal JSON representation.
		 *
		 * @return The JSON representation as a string.
		 */
		[[nodiscard]] std::string getJSON() const;

		/**
		 * Get a unique ID.
		 *
		 * @return The unique ID.
		 */
		[[nodsicard]] uint64_t getUniqueID() { return m_UniqueID++; }

	private:
		Json m_SourceJSON;
		uint64_t m_UniqueID = 1;
	};
} // namespace ShaderBuilder