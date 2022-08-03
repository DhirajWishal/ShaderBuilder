// Copyright (c) 2022 Dhiraj Wishal

#include "ShaderBuilder/Utilities.hpp"

#define XXH_INLINE_ALL
#include <xxhash.h>

namespace ShaderBuilder
{
	uint64_t GenerateHash(const void* pDataStore, uint64_t size)
	{
		return static_cast<uint64_t>(XXH64(pDataStore, size, 0));
	}
} // namespace ShaderBuilder