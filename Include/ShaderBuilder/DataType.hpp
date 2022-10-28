// Copyright (c) 2022 Dhiraj Wishal

#pragma once

#include "TypeTraits.hpp"
#include "SPIRVSource.hpp"

namespace ShaderBuilder
{
	/**
	 * Data type class.
	 * This class is the base class for all the supported types.
	 *
	 * @tparam Derived The derived type.
	 */
	template<class Derived>
	class DataType
	{
	public:
		/**
		 * Explicit constructor.
		 *
		 * @param source The source to record all the instructions to.
		 * @param variableName The name of the variable.
		 */
		explicit DataType(SPIRVSource& source, const std::string& variableName) : m_VariableName(variableName), m_Source(source) {}

		/**
		 * Set a debug name to the data type.
		 * Note: This should be done only once and must have a unique name!
		 *
		 * @param name The name to set.
		 */
		void setDebugName(const std::string& name) { m_Source.insertName(fmt::format("OpName %{} \"{}\"", m_VariableName, name)); }

		/**
		 * Get the name of the variable/ function.
		 *
		 * @return The name.
		 */
		[[nodiscard]] const std::string& getName() const { return m_VariableName; }

		/**
		 * Get the identifier of the variable/ function.
		 *
		 * @return The identifier.
		 */
		[[nodiscard]] std::string getIdentifier() const { return "%" + m_VariableName; }

		/**
		 * Get the source to which the instructions are written to.
		 *
		 * @return The source reference.
		 */
		[[nodiscard]] SPIRVSource& getSource() { return m_Source; }

		/**
		 * Get the source to which the instructions are written to.
		 *
		 * @return The const source reference.
		 */
		[[nodiscard]] const SPIRVSource& getSource() const { return m_Source; }

	protected:
		std::string m_VariableName;
		SPIRVSource& m_Source;
	};

	/**
	 * std::ostream& operator overload.
	 *
	 * @param stream The output stream.
	 * @param dataType The data type to output.
	 * @return The incoming stream.
	 */
	template<class Derived>
	std::ostream& operator<<(std::ostream& stream, const DataType<Derived>& dataType)
	{
		stream << dataType.getName();
		return stream;
	}
} // namespace ShaderBuilder
