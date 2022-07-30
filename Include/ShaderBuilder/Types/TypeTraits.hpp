// Copyright (c) 2022 Dhiraj Wishal

#pragma once

#include <cstdint>

namespace ShaderBuilder
{
	/**
	 * Type traits structure.
	 */
	template <class Type>
	struct TypeTraits;

	/**
	 * Void type traits.
	 */
	template <>
	struct TypeTraits<void>
	{
		using Type = void;
		static constexpr const char Identifier[] = "%void";
		static constexpr const char Declaration[] = "%void = OpTypeVoid";
		static constexpr uint8_t Size = 0;
	};

	/**
	 * Boolean type traits.
	 */
	template <>
	struct TypeTraits<bool>
	{
		using Type = bool;
		static constexpr const char Identifier[] = "%bool";
		static constexpr const char Declaration[] = "%bool = OpTypeBool";
		static constexpr uint8_t Size = sizeof(Type);
	};

	/**
	 * 8 bit signed integer type traits.
	 */
	template <>
	struct TypeTraits<int8_t>
	{
		using Type = int8_t;
		static constexpr const char Identifier[] = "%int8";
		static constexpr const char Declaration[] = "%int8 = OpTypeInt 8 1";
		static constexpr uint8_t Size = sizeof(Type);
	};

	/**
	 * 8 bit unsigned integer type traits.
	 */
	template <>
	struct TypeTraits<uint8_t>
	{
		using Type = uint8_t;
		static constexpr const char Identifier[] = "%uint8";
		static constexpr const char Declaration[] = "%uint8 = OpTypeInt 8 0";
		static constexpr uint8_t Size = sizeof(Type);
	};

	/**
	 * 16 bit signed integer type traits.
	 */
	template <>
	struct TypeTraits<int16_t>
	{
		using Type = int16_t;
		static constexpr const char Identifier[] = "%int16";
		static constexpr const char Declaration[] = "%int16 = OpTypeInt 16 1";
		static constexpr uint8_t Size = sizeof(Type);
	};

	/**
	 * 16 bit unsigned integer type traits.
	 */
	template <>
	struct TypeTraits<uint16_t>
	{
		using Type = uint16_t;
		static constexpr const char Identifier[] = "%uint16";
		static constexpr const char Declaration[] = "%uint16 = OpTypeInt 16 0";
		static constexpr uint8_t Size = sizeof(Type);
	};

	/**
	 * 32 bit signed integer type traits.
	 */
	template <>
	struct TypeTraits<int32_t>
	{
		using Type = int32_t;
		static constexpr const char Identifier[] = "%int32";
		static constexpr const char Declaration[] = "%int32 = OpTypeInt 32 1";
		static constexpr uint8_t Size = sizeof(Type);
	};

	/**
	 * 32 bit unsigned integer type traits.
	 */
	template <>
	struct TypeTraits<uint32_t>
	{
		using Type = uint32_t;
		static constexpr const char Identifier[] = "%uint32";
		static constexpr const char Declaration[] = "%uint32 = OpTypeInt 32 0";
		static constexpr uint8_t Size = sizeof(Type);
	};

	/**
	 * 64 bit signed integer type traits.
	 */
	template <>
	struct TypeTraits<int64_t>
	{
		using Type = int64_t;
		static constexpr const char Identifier[] = "%int64";
		static constexpr const char Declaration[] = "%int64 = OpTypeInt 64 1";
		static constexpr uint8_t Size = sizeof(Type);
	};

	/**
	 * 64 bit unsigned integer type traits.
	 */
	template <>
	struct TypeTraits<uint64_t>
	{
		using Type = uint64_t;
		static constexpr const char Identifier[] = "%uint64";
		static constexpr const char Declaration[] = "%int64 = OpTypeInt 64 0";
		static constexpr uint8_t Size = sizeof(Type);
	};

	/**
	 * Float type traits.
	 */
	template <>
	struct TypeTraits<float>
	{
		using Type = float;
		static constexpr const char Identifier[] = "%float";
		static constexpr const char Declaration[] = "%float = OpTypeFloat 32";
		static constexpr uint8_t Size = sizeof(Type);
	};

	/**
	 * Double type traits.
	 */
	template <>
	struct TypeTraits<double>
	{
		using Type = double;
		static constexpr const char Identifier[] = "%double";
		static constexpr const char Declaration[] = "%double = OpTypeFloat 64";
		static constexpr uint8_t Size = sizeof(Type);
	};

	/**
	 * Is complex type boolean.
	 */
	template<class Type>
	constexpr bool IsCompexType = false;
} // namespace ShaderBuilder