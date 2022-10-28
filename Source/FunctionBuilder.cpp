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
			m_Source.getCurrentFunctionBlock().m_Instructions.insert("OpReturn");
			m_Source.finishFunctionBlock();
			m_IsComplete = true;
			m_IsRecording = false;
		}
	}

	void FunctionBuilder::toggleRecording()
	{
		if (m_IsRecording)
		{
			m_IsRecording = false;
			m_Source.getCurrentFunctionBlock().disableRecording();
		}
	}

} // namespace ShaderBuilder