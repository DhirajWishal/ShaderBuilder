// Copyright (c) 2022 Dhiraj Wishal

#include "ShaderBuilder/FunctionBuilder.hpp"

namespace ShaderBuilder
{
	FunctionBuilder::~FunctionBuilder()
	{
		m_Source.insertFunctionDefinition(std::move(m_FunctionJSON));
	}
} // namespace ShaderBuilder