// Copyright (c) 2022 Dhiraj Wishal
#pragma once

#include <array>
#include <cstdint>
#include <string_view>

namespace ShaderBuilder
{
	/**
	 * Static string concatenation function.
	 *
	 * @tparam Strings The strings to concatenate.
	 * @return The concatenated string.
	 */
	template<std::string_view const&... Strings>
	[[nodiscard]] consteval std::string_view StaticStringConcat()
	{
		constexpr std::size_t len = (Strings.size() + ... + 0);
		std::array<char, len + 1> finalString{};

		auto append = [i = 0, &finalString](auto const& s) mutable { for (auto c : s) finalString[i++] = c; };
		(append(Strings), ...);
		finalString[len] = 0;

		return std::string_view{ finalString.data(), finalString.size() - 1 };
	}

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
		static constexpr std::string_view Identifier = "%void";
		static constexpr std::string_view Declaration = "%void = OpTypeVoid";
	};

	/**
	 * Boolean type traits.
	 */
	template <>
	struct TypeTraits<bool>
	{
		using Type = bool;
		static constexpr std::string_view Identifier = "%bool";
		static constexpr std::string_view Declaration = "%bool = OpTypeBool";
	};

	/**
	 * 8 bit signed integer type traits.
	 */
	template <>
	struct TypeTraits<int8_t>
	{
		using Type = int8_t;
		static constexpr std::string_view Identifier = "%int8";
		static constexpr std::string_view Declaration = "%int8 = OpTypeInt 8 1";
	};

	/**
	 * 8 bit unsigned integer type traits.
	 */
	template <>
	struct TypeTraits<uint8_t>
	{
		using Type = uint8_t;
		static constexpr std::string_view Identifier = "%uint8";
		static constexpr std::string_view Declaration = "%uint8 = OpTypeInt 8 0";
	};

	/**
	 * 16 bit signed integer type traits.
	 */
	template <>
	struct TypeTraits<int16_t>
	{
		using Type = int16_t;
		static constexpr std::string_view Identifier = "%int16";
		static constexpr std::string_view Declaration = "%int16 = OpTypeInt 16 1";
	};

	/**
	 * 16 bit unsigned integer type traits.
	 */
	template <>
	struct TypeTraits<uint16_t>
	{
		using Type = uint16_t;
		static constexpr std::string_view Identifier = "%uint16";
		static constexpr std::string_view Declaration = "%uint16 = OpTypeInt 16 0";
	};

	/**
	 * 32 bit signed integer type traits.
	 */
	template <>
	struct TypeTraits<int32_t>
	{
		using Type = int32_t;
		static constexpr std::string_view Identifier = "%int32";
		static constexpr std::string_view Declaration = "%int32 = OpTypeInt 32 1";
	};

	/**
	 * 32 bit unsigned integer type traits.
	 */
	template <>
	struct TypeTraits<uint32_t>
	{
		using Type = uint32_t;
		static constexpr std::string_view Identifier = "%uint32";
		static constexpr std::string_view Declaration = "%uint32 = OpTypeInt 32 0";
	};

	/**
	 * 64 bit signed integer type traits.
	 */
	template <>
	struct TypeTraits<int64_t>
	{
		using Type = int64_t;
		static constexpr std::string_view Identifier = "%int64";
		static constexpr std::string_view Declaration = "%int64 = OpTypeInt 64 1";
	};

	/**
	 * 64 bit unsigned integer type traits.
	 */
	template <>
	struct TypeTraits<uint64_t>
	{
		using Type = uint64_t;
		static constexpr std::string_view Identifier = "%uint64";
		static constexpr std::string_view Declaration = "%int64 = OpTypeInt 64 0";
	};

	/**
	 * Float type traits.
	 */
	template <>
	struct TypeTraits<float>
	{
		using Type = float;
		static constexpr std::string_view Identifier = "%float";
		static constexpr std::string_view Declaration = "%float = OpTypeFloat 32";
	};

	/**
	 * Double type traits.
	 */
	template <>
	struct TypeTraits<double>
	{
		using Type = double;
		static constexpr std::string_view Identifier = "%double";
		static constexpr std::string_view Declaration = "%double = OpTypeFloat 64";
	};

	/**
	 * Is complex type boolean.
	 */
	template<class Type>
	constexpr bool IsCompexType = false;
} // namespace ShaderBuilder