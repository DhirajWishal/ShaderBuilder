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
		 * @param variableName The name of the variable.
		 */
		explicit DataType(std::string&& variableName) : m_VariableName(std::move(variableName))
		{
			// Validate the name.
			if (m_VariableName.find('%') != std::string::npos)
			{
				if (m_VariableName.find('%') > 0)
				{
					// Throw an error for invalid name.
				}
			}
			else
			{
				m_VariableName = "%" + m_VariableName;
			}
		}

		/**
		 * Get the name of the variable/ function.
		 *
		 * @return The name.
		 */
		[[nodsicard]] std::string getName() const { return m_VariableName; }

	protected:
		std::string m_VariableName;
	};
} // namespace ShaderBuilder
