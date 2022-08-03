// Copyright (c) 2022 Dhiraj Wishal

#include "ShaderBuilder/FunctionBuilder.hpp"

namespace ShaderBuilder
{
	std::string FunctionBuilder::getString() const
	{
		std::stringstream finalTransform;

		finalTransform << m_Variables.getString();
		finalTransform << m_FunctionBlock.getString();

		return finalTransform.str();
	}
} // namespace ShaderBuilder