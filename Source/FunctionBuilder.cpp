// Copyright (c) 2022 Dhiraj Wishal

#include "ShaderBuilder/FunctionBuilder.hpp"

namespace ShaderBuilder
{
	FunctionBuilder::~FunctionBuilder()
	{
		if (!m_IsComplete)
			exit();
	}

	void FunctionBuilder::exit()
	{
		auto& block = m_Source.getCurrentFunctionBlock();
		block.m_Instructions.insert("OpReturn");
		m_IsComplete = true;
	}

} // namespace ShaderBuilder