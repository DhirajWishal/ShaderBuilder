// Copyright (c) 2022 Dhiraj Wishal

#pragma once

#include "SPIRVSource.hpp"

namespace ShaderBuilder
{
	/**
	 * Attribute class.
	 * This is the base class for the input and output attributes.
	 *
	 * @tparam Type The attribute type.
	 */
	template<class Type>
	class Attribute
	{
	public:
		/**
		 * Explicit constructor.
		 *
		 * @param source The source to record all the instructions to.
		 * @param location The input location.
		 */
		explicit Attribute(SPIRVSource& source, uint32_t location) : m_Source(source), m_Location(location), m_Data(source, source.getUniqueIdentifier())
		{
			m_Source.registerType<Type>();
			m_Source.insertAnnotation(fmt::format("OpDecorate %{} Location {}", m_Data.getName(), location));
		}

		/**
		 * Get the variable's name.
		 *
		 * @return The attribute's name.
		 */
		[[nodiscard]] const std::string getName() const { return m_Data.getName(); }

		/**
		 * Get the source reference.
		 *
		 * @return The SPIRV source reference.
		 */
		[[nodiscard]] SPIRVSource& getSource() { return m_Source; }

		/**
		 * Get the source reference.
		 *
		 * @return The const SPIRV source reference.
		 */
		[[nodiscard]] const SPIRVSource& getSource() const { return m_Source; }

	protected:
		/**
		 * Get the internally stored data.
		 *
		 * @return The data reference.
		 */
		[[nodiscard]] Type& getData() { return m_Data; }

		/**
		 * Get the internally stored data.
		 *
		 * @return The const data reference.
		 */
		[[nodiscard]] const Type& getData() const { return m_Data; }

	protected:
		Type m_Data;

		SPIRVSource& m_Source;
		uint32_t m_Location;
	};
}