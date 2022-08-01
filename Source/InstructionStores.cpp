// Copyright (c) 2022 Dhiraj Wishal

#include "ShaderBuilder/InstructionStores.hpp"

#define XXH_INLINE_ALL
#include <xxhash.h>

namespace ShaderBuilder
{
	void InstructionStore::insert(std::string&& line)
	{
		m_Instructions << line << std::endl;
	}

	void TypeDeclarationInstructions::insert(std::string&& line)
	{
		if (m_AvailabilityMap.insert(static_cast<uint64_t>(XXH64(line.data(), line.size(), 0))).second)
			m_Instructions << std::move(line) << std::endl;
	}
} // namespace ShaderBuilder