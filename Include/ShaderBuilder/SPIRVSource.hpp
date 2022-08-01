// Copyright (c) 2022 Dhiraj Wishal

#pragma once

#include <cstdint>
#include <sstream>
#include <vector>

namespace ShaderBuilder
{
	/**
	 * Assembly category enum.
	 */
	enum class AssemblyCategory : uint8_t
	{
		CompatibilityInstructions,
		ExtensionInstructions,
		ExtInstImportInstructions,
		MemoryModel,
		EntryPoints,
		ExecutionModes,
		DebugSources,
		DebugNames,
		DebugModuleProcessed,
		Annotations,
		TypeDeclarations,
		FunctionDeclarations,
		FunctionDefinitions
	};

	/**
	 * SPIR-V Source class.
	 * This contains all the source information provided by the data types and others.
	 */
	class SPIRVSource final
	{
	public:
		/**
		 * Insert a new set of instructions to the source.
		 * Note that this will append an end line instruction at the end so you don't have to provide one.
		 *
		 * @tparam Types The data types.
		 * @param category The instructions category.
		 * @param data The data to enter.
		 */
		template<class...Types>
		void insert(AssemblyCategory category, Types&&... data)
		{
			auto insertData = [this](auto&& member) { m_SourceAssembly << std::move(member); };
			(insertData(std::move(data)), ...);

			m_SourceCategories.emplace_back(category);
			m_SourceAssembly << std::endl;
		}

		/**
		 * Get the source assembly.
		 *
		 * @return The input string stream.
		 */
		[[nodiscard]] std::istringstream getSourceAssembly() const { return std::istringstream(m_SourceAssembly.str()); }

		/**
		 * Get the source categories.
		 *
		 * @return The categories vector.
		 */
		[[nodiscard]] const std::vector<AssemblyCategory>& getSourceCategories() const { return m_SourceCategories; }

		/**
		 * Prepare the source to be assembled.
		 *
		 * @return The prepared string.
		 */
		[[nodsicard]] std::string prepare() const;

	private:
		std::stringstream m_SourceAssembly;
		std::vector<AssemblyCategory> m_SourceCategories;
	};
} // namespace ShaderBuilder