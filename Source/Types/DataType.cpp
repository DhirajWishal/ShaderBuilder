// Copyright (c) 2022 Dhiraj Wishal

#include "ShaderBuilder/Types/DataType.hpp"

namespace ShaderBuilder
{
	DataType::DataType(std::string_view typeName, std::string&& variableName, CodeStream& codeStream)
		: m_TypeName(typeName), m_VariableName(std::move(variableName)), m_CodeStream(codeStream)
	{
		m_CodeStream << m_TypeName << " " << m_VariableName;
	}
} // namespace ShaderBuilder
