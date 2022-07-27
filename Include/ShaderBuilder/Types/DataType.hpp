// Copyright (c) 2022 Dhiraj Wishal
#pragma once

#include <string>
#include <sstream>

namespace ShaderBuilder
{
	using CodeStream = std::stringstream;

	/**
	 * Data type class.
	 * This class is the base class for all the supported types.
	 */
	class DataType
	{
	public:
		/**
		 * Explicit constructor.
		 *
		 * @param typeName The type name of the data type.
		 * @param variableName The name of the variable.
		 * @param codeStream The code stream to register the code to.
		 */
		explicit DataType(std::string_view typeName, std::string&& variableName, CodeStream& codeStream);

	protected:
		std::string_view m_TypeName;
		std::string m_VariableName;
		CodeStream& m_CodeStream;
	};
} // namespace ShaderBuilder
