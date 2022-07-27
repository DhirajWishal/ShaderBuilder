// Copyright (c) 2022 Dhiraj Wishal

#include "ShaderBuilder/Types/Vec2.hpp"

namespace ShaderBuilder
{
	Vec2::Vec2(std::string&& variableName, CodeStream& codeStream, MetaInformation meta /*= MetaInformation::Variable*/)
		: DataType("vec2", std::move(variableName), codeStream, meta)
	{
		if (meta & MetaInformation::Variable)
			m_CodeStream << ";" << std::endl;
	}

	Vec2& Vec2::operator=(const Vec2& other)
	{
		m_CodeStream << m_VariableName << " = " << other.m_VariableName << ";" << std::endl;
		return *this;
	}

} // namespace ShaderBuilder