// Copyright (c) 2022 Dhiraj Wishal

#pragma once

#include "TypeTraits.hpp"
#include "SPIRVSource.hpp"

namespace ShaderBuilder
{
	/**
	 * Data type class.
	 * This class is the base class for all the supported types.
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
		 * Explicit constructor.
		 *
		 * @param location The location of the attribute.
		 * @param isInput Whether or not the attribute is input or not.
		 * @param source The source to record all the instructions to.
		 * @param variableName The name of the variable.
		 */
		explicit DataType(uint32_t location, bool isInput, SPIRVSource& source, const std::string& variableName) : m_VariableName(variableName), m_Source(source)
		{
			if (isInput)
			{
				m_Source.insertType("%input_" + variableName + " = OpTypePointer Input " + TypeTraits<Derived>::Identifier);
				m_Source.insertType("%" + variableName + " = OpVariable %input_" + variableName + " Input");
			}
			else
			{
				m_Source.insertType("%output_" + variableName + " = OpTypePointer Output " + TypeTraits<Derived>::Identifier);
				m_Source.insertType("%" + variableName + " = OpVariable %output_" + variableName + " Output");
			}

			m_Source.insertName("OpName %" + variableName + " \"" + variableName + "\"");
			m_Source.insertAnnotation("OpDecorate %" + variableName + " Location " + std::to_string(location));
		}

		/**
		 * Get the name of the variable/ function.
		 *
		 * @return The name.
		 */
		[[nodiscard]] std::string getName() const { return m_VariableName; }

		/**
		 * Get the identifier of the variable/ function.
		 *
		 * @return The identifier.
		 */
		[[nodiscard]] std::string getIdentifier() const { return "%" + m_VariableName; }

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
