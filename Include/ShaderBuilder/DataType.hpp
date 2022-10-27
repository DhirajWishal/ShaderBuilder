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
				m_Source.insertType(std::format("%input_{} = OpTypePointer Input {}", variableName, TypeTraits<Derived>::Identifier));
				m_Source.insertType(std::format("%{} = OpVariable %input_{} Input", variableName, variableName));
			}
			else
			{
				m_Source.insertType(std::format("%output_{} = OpTypePointer Output {}", variableName, TypeTraits<Derived>::Identifier));
				m_Source.insertType(std::format("%{} = OpVariable %output_{} Output", variableName, variableName));
			}

			m_Source.insertName(std::format("OpName %{} \"{}\"", variableName, variableName));
			m_Source.insertAnnotation(std::format("OpDecorate %{} Location {}", variableName, location));
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
