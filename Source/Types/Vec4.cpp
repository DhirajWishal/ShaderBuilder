// Copyright (c) 2022 Dhiraj Wishal

#include "ShaderBuilder/Types/Vec4.hpp"

namespace ShaderBuilder
{
	Vec4::Vec4(std::string&& variableName, CodeStream& codeStream, MetaInformation meta /*= MetaInformation::Variable*/)
		: DataType("vec4", std::move(variableName), codeStream, meta)
	{
		if (meta & MetaInformation::Variable)
			m_CodeStream << ";" << std::endl;
	}

	Vec4::Vec4(float r, float g, float b, float a, std::string&& variableName, CodeStream& codeStream, MetaInformation meta /*= MetaInformation::Variable*/)
		: DataType("vec4", std::move(variableName), codeStream, meta)
		, r(r), g(g), b(b), a(a)
	{
		if (meta & MetaInformation::Variable)
			m_CodeStream << " = vec4(" << r << ", " << g << ", " << b << ", " << a << ");" << std::endl;
	}

	ShaderBuilder::Vec4& Vec4::operator=(const Vec4& other)
	{
		m_CodeStream << m_VariableName << " = " << other.m_VariableName << ";" << std::endl;
		return *this;
	}
} // namespace ShaderBuilder