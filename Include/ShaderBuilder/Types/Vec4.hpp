// Copyright (c) 2022 Dhiraj Wishal
#pragma once

#include "DataType.hpp"

namespace ShaderBuilder
{
	/**
	 * Vector 4 data type.
	 *
	 * @tparam Type The value type. Default is float.
	 */
	template<class Type = float>
	class Vec4 final : public DataType<Vec4<Type>>
	{
	public:
		using Traits = TypeTraits<Vec4<Type>>;

	public:
		/**
		 * Explicit constructor.
		 *
		 * @param variableName The name of the variable.
		 */
		explicit Vec4(std::string&& variableName) : DataType<Vec4<Type>>(std::move(variableName)) {}

	public:
		union
		{
			struct { Type x, y, z, w; };
			struct { Type r, g, b, a; };
		};
	};

	/**
	 * Vec4 int8_t specialization.
	 */
	template<>
	struct TypeTraits<Vec4<int8_t>>
	{
		using Type = Vec4<int8_t>;
		using ValueTraits = TypeTraits<int8_t>;
		static constexpr std::string_view Identifier = "%vec4_int8";
		static constexpr std::string_view Declaration = "%vec4_int8 = OpTypeVector %int8 4";
	};

	/**
	 * Vec4 uint8_t specialization.
	 */
	template<>
	struct TypeTraits<Vec4<uint8_t>>
	{
		using Type = Vec4<uint8_t>;
		using ValueTraits = TypeTraits<uint8_t>;
		static constexpr std::string_view Identifier = "%vec4_uint8";
		static constexpr std::string_view Declaration = "%vec4_uint8 = OpTypeVector %uint8 4";
	};

	/**
	 * Vec4 int16_t specialization.
	 */
	template<>
	struct TypeTraits<Vec4<int16_t>>
	{
		using Type = Vec4<int16_t>;
		using ValueTraits = TypeTraits<int16_t>;
		static constexpr std::string_view Identifier = "%vec4_int16";
		static constexpr std::string_view Declaration = "%vec4_int16 = OpTypeVector %int16 4";
	};

	/**
	 * Vec4 uint16_t specialization.
	 */
	template<>
	struct TypeTraits<Vec4<uint16_t>>
	{
		using Type = Vec4<uint16_t>;
		using ValueTraits = TypeTraits<uint16_t>;
		static constexpr std::string_view Identifier = "%vec4_uint16";
		static constexpr std::string_view Declaration = "%vec4_uint16 = OpTypeVector %uint16 4";
	};

	/**
	 * Vec4 int32_t specialization.
	 */
	template<>
	struct TypeTraits<Vec4<int32_t>>
	{
		using Type = Vec4<int32_t>;
		using ValueTraits = TypeTraits<int32_t>;
		static constexpr std::string_view Identifier = "%vec4_int32";
		static constexpr std::string_view Declaration = "%vec4_int32 = OpTypeVector %int32 4";
	};

	/**
	 * Vec4 uint32_t specialization.
	 */
	template<>
	struct TypeTraits<Vec4<uint32_t>>
	{
		using Type = Vec4<uint32_t>;
		using ValueTraits = TypeTraits<uint32_t>;
		static constexpr std::string_view Identifier = "%vec4_uint32";
		static constexpr std::string_view Declaration = "%vec4_uint32 = OpTypeVector %uint32 4";
	};

	/**
	 * Vec4 int64_t specialization.
	 */
	template<>
	struct TypeTraits<Vec4<int64_t>>
	{
		using Type = Vec4<int64_t>;
		using ValueTraits = TypeTraits<int64_t>;
		static constexpr std::string_view Identifier = "%vec4_int64";
		static constexpr std::string_view Declaration = "%vec4_int64 = OpTypeVector %int64 4";
	};

	/**
	 * Vec4 uint64_t specialization.
	 */
	template<>
	struct TypeTraits<Vec4<uint64_t>>
	{
		using Type = Vec4<uint64_t>;
		using ValueTraits = TypeTraits<uint64_t>;
		static constexpr std::string_view Identifier = "%vec4_uint64";
		static constexpr std::string_view Declaration = "%vec4_uint64 = OpTypeVector %uint64 4";
	};

	/**
	 * Vec4 float specialization.
	 */
	template<>
	struct TypeTraits<Vec4<float>>
	{
		using Type = Vec4<float>;
		using ValueTraits = TypeTraits<float>;
		static constexpr std::string_view Identifier = "%vec4_float";
		static constexpr std::string_view Declaration = "%vec4_float = OpTypeVector %float 4";
	};

	/**
	 * Vec4 double specialization.
	 */
	template<>
	struct TypeTraits<Vec4<double>>
	{
		using Type = Vec4<double>;
		using ValueTraits = TypeTraits<double>;
		static constexpr std::string_view Identifier = "%vec4_double";
		static constexpr std::string_view Declaration = "%vec4_double = OpTypeVector %double 4";
	};

	/**
	 * Is complex type boolean specialization
	 */
	template<class Type>
	constexpr bool IsCompexType<Vec4<Type>> = true;
} // namespace ShaderBuilder