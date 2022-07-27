// Copyright (c) 2022 Dhiraj Wishal

#include "ShaderBuilder/Types/Vec3.hpp"

namespace ShaderBuilder
{
	Vec3::Vec3(std::string&& variableName, CodeStream& codeStream)
		: DataType("vec3", std::move(variableName), codeStream)
	{
		m_CodeStream << ";" << std::endl;
	}

} // namespace ShaderBuilder