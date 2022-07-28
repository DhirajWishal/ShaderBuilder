// Copyright (c) 2022 Dhiraj Wishal

#include "ShaderBuilder/Builder.hpp"

namespace ShaderBuilder
{
	Builder::Builder()
		: m_BuiltIns(m_ShaderCode)
	{
		m_ShaderCode << "#version 450" << std::endl;
	}

	Builder::BuiltIns::BuiltIns(CodeStream& stream)
		: gl_Position(Vec4("gl_Position", stream, MetaInformation::BuiltIn))
	{
	}

} // namespace ShaderBuilder