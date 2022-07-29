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
		static constexpr const char OpType[] = "OpTypeVoid";
	};

	/**
	 * Boolean type traits.
	 */
	template <>
	struct TypeTraits<bool>
	{
		using Type = bool;
		static constexpr const char Identifier[] = "%bool";
		static constexpr const char OpType[] = "OpTypeBool";
	};

	/**
	 * 8 bit signed integer type traits.
	 */
	template <>
	struct TypeTraits<int8_t>
	{
		using Type = int8_t;
		static constexpr const char Identifier[] = "%int8";
		static constexpr const char OpType[] = "OpTypeInt";
		static constexpr uint8_t Width = sizeof(Type);
		static constexpr bool IsSigned = true;
	};

	/**
	 * 8 bit unsigned integer type traits.
	 */
	template <>
	struct TypeTraits<uint8_t>
	{
		using Type = uint8_t;
		static constexpr const char Identifier[] = "%uint8";
		static constexpr const char OpType[] = "OpTypeInt";
		static constexpr uint8_t Width = sizeof(Type);
		static constexpr bool IsSigned = false;
	};

	/**
	 * 16 bit signed integer type traits.
	 */
	template <>
	struct TypeTraits<int16_t>
	{
		using Type = int16_t;
		static constexpr const char Identifier[] = "%int16";
		static constexpr const char OpType[] = "OpTypeInt";
		static constexpr uint8_t Width = sizeof(Type);
		static constexpr bool IsSigned = true;
	};

	/**
	 * 16 bit unsigned integer type traits.
	 */
	template <>
	struct TypeTraits<uint16_t>
	{
		using Type = uint16_t;
		static constexpr const char Identifier[] = "%uint16";
		static constexpr const char OpType[] = "OpTypeInt";
		static constexpr uint8_t Width = sizeof(Type);
		static constexpr bool IsSigned = false;
	};

	/**
	 * 32 bit signed integer type traits.
	 */
	template <>
	struct TypeTraits<int32_t>
	{
		using Type = int32_t;
		static constexpr const char Identifier[] = "%int32";
		static constexpr const char OpType[] = "OpTypeInt";
		static constexpr uint8_t Width = sizeof(Type);
		static constexpr bool IsSigned = true;
	};

	/**
	 * 32 bit unsigned integer type traits.
	 */
	template <>
	struct TypeTraits<uint32_t>
	{
		using Type = uint32_t;
		static constexpr const char Identifier[] = "%uint32";
		static constexpr const char OpType[] = "OpTypeInt";
		static constexpr uint8_t Width = sizeof(Type);
		static constexpr bool IsSigned = false;
	};

	/**
	 * 64 bit signed integer type traits.
	 */
	template <>
	struct TypeTraits<int64_t>
	{
		using Type = int64_t;
		static constexpr const char Identifier[] = "%int64";
		static constexpr const char OpType[] = "OpTypeInt";
		static constexpr uint8_t Width = sizeof(Type);
		static constexpr bool IsSigned = true;
	};

	/**
	 * 64 bit unsigned integer type traits.
	 */
	template <>
	struct TypeTraits<uint64_t>
	{
		using Type = uint64_t;
		static constexpr const char Identifier[] = "%uint64";
		static constexpr const char OpType[] = "OpTypeInt";
		static constexpr uint8_t Width = sizeof(Type);
		static constexpr bool IsSigned = false;
	};

	/**
	 * Float type traits.
	 */
	template <>
	struct TypeTraits<float>
	{
		using Type = float;
		static constexpr const char Identifier[] = "%float";
		static constexpr const char OpType[] = "OpTypeFloat";
		static constexpr uint8_t Width = sizeof(Type);
	};

	/**
	 * Double type traits.
	 */
	template <>
	struct TypeTraits<double>
	{
		using Type = double;
		static constexpr const char Identifier[] = "%double";
		static constexpr const char OpType[] = "OpTypeFloat";
		static constexpr uint8_t Width = sizeof(Type);
	};
} // namespace ShaderBuilder