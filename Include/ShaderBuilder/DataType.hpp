// Copyright (c) 2022 Dhiraj Wishal

#pragma once

#include "TypeTraits.hpp"

#include <string>

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
		explicit DataType(std::string&& variableName) : m_VariableName(std::move(variableName)) {}

		/**
		 * Get the name of the variable/ function.
		 *
		 * @return The name.
		 */
		[[nodsicard]] std::string getName() const { return m_VariableName; }

	protected:
		std::string m_VariableName;
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
