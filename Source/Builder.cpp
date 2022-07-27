// Copyright (c) 2022 Dhiraj Wishal

#include "ShaderBuilder/Builder.hpp"

namespace ShaderBuilder
{
	Builder::Builder()
	{
		m_ShaderCode << "#version 450" << std::endl;
	}
} // namespace ShaderBuilder