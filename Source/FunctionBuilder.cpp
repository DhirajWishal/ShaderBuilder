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
		if (m_IsRecording)
		{
			auto& block = m_Source.getCurrentFunctionBlock();
			block.m_Instructions.insert("OpReturn");
			m_IsComplete = true;
		}
	}

	void FunctionBuilder::toggleRecording()
	{
		m_IsRecording = false;
		m_Source.getCurrentFunctionBlock().disableRecording();
	}

} // namespace ShaderBuilder