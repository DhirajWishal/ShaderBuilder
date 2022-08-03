// Copyright (c) 2022 Dhiraj Wishal

#pragma once

#include <cstdint>

namespace ShaderBuilder
{
	/**
	 * Generate hash for a block of memory.
	 *
	 * @param pDataStore The memory to generate hash.
	 * @param size The size of the memory.
	 * @return The generated hash.
	 */
	[[nodiscard]] uint64_t GenerateHash(const void* pDataStore, uint64_t size);
} // namespace ShaderBuilder