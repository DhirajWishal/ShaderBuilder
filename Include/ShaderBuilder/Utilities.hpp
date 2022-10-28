// Copyright (c) 2022 Dhiraj Wishal

#pragma once

#include "TypeTraits.hpp"

#include <fmt/format.h>

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

	/**
	 * Get the constant value's identifier.
	 * Make sure that the type is registered.
	 *
	 * @tparam Type The type of the value.
	 * @param value The constant value.
	 * @return The identifier string.
	 */
	template<class Type>
	[[nodiscard]] static std::string GetConstantIdentifier(const Type& value)
	{
		return fmt::format("const_{}_{}", TypeTraits<Type>::RawIdentifier, value);
	}
} // namespace ShaderBuilder