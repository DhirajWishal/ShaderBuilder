// Copyright (c) 2022 Dhiraj Wishal

#include "ShaderBuilder/InstructionStores.hpp"
#include "ShaderBuilder/Utilities.hpp"

namespace ShaderBuilder
{
	void InstructionStore::insert(std::string&& line)
	{
		m_Instructions << line << std::endl;
	}

	void TypeDeclarationInstructions::insert(std::string&& line)
	{
		if (m_AvailabilityMap.insert(GenerateHash(line.data(), line.size())).second)
			m_Instructions << std::move(line) << std::endl;
	}
} // namespace ShaderBuilder