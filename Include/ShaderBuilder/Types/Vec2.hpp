// Copyright (c) 2022 Dhiraj Wishal
#pragma once

#include "DataType.hpp"

namespace ShaderBuilder
{
	/**
	 * Vector 2 data type.
	 *
	 * @tparam Type The value type. Default is float.
	 */
	template<class Type = float>
	class Vec2 final : public DataType<Vec2<Type>>
	{
	public:
		using Traits = TypeTraits<Vec2<Type>>;

	public:
		/**
		 * Explicit constructor.
		 *
		 * @param variableName The name of the variable.
		 * @param codeStream The code stream to store the data in.
		 * @param meta The meta information regarding the type.
		 */
		explicit Vec2(std::string&& variableName) : DataType<Vec2<Type>>(std::move(variableName)) {}

		/**
		 * Assignment operator.
		 *
		 * @param other The other vector.
		 * @return The altered vector reference.
		 */
		Vec2& operator=(const Vec2& other)
		{
			return *this;
		}

	public:
		union
		{
			struct { Type x, y; };
			struct { Type a, b; };
			struct { Type u, v; };
		};
	};

	/**
	 * Vec2 int8_t specialization.
	 */
	template<>
	struct TypeTraits<Vec2<int8_t>>
	{
		using Type = Vec2<int8_t>;
		using ValueTraits = TypeTraits<int8_t>;
		static constexpr std::string_view Identifier = "%vec2_int8";
		static constexpr std::string_view Declaration = "%vec2_int8 = OpTypeVector %int8 2";
	};

	/**
	 * Vec2 uint8_t specialization.
	 */
	template<>
	struct TypeTraits<Vec2<uint8_t>>
	{
		using Type = Vec2<uint8_t>;
		using ValueTraits = TypeTraits<uint8_t>;
		static constexpr std::string_view Identifier = "%vec2_uint8";
		static constexpr std::string_view Declaration = "%vec2_uint8 = OpTypeVector %uint8 2";
	};

	/**
	 * Vec2 int16_t specialization.
	 */
	template<>
	struct TypeTraits<Vec2<int16_t>>
	{
		using Type = Vec2<int16_t>;
		using ValueTraits = TypeTraits<int16_t>;
		static constexpr std::string_view Identifier = "%vec2_int16";
		static constexpr std::string_view Declaration = "%vec2_int16 = OpTypeVector %int16 2";
	};

	/**
	 * Vec2 uint16_t specialization.
	 */
	template<>
	struct TypeTraits<Vec2<uint16_t>>
	{
		using Type = Vec2<uint16_t>;
		using ValueTraits = TypeTraits<uint16_t>;
		static constexpr std::string_view Identifier = "%vec2_uint16";
		static constexpr std::string_view Declaration = "%vec2_uint16 = OpTypeVector %uint16 2";
	};

	/**
	 * Vec2 int32_t specialization.
	 */
	template<>
	struct TypeTraits<Vec2<int32_t>>
	{
		using Type = Vec2<int32_t>;
		using ValueTraits = TypeTraits<int32_t>;
		static constexpr std::string_view Identifier = "%vec2_int32";
		static constexpr std::string_view Declaration = "%vec2_int32 = OpTypeVector %int32 2";
	};

	/**
	 * Vec2 uint32_t specialization.
	 */
	template<>
	struct TypeTraits<Vec2<uint32_t>>
	{
		using Type = Vec2<uint32_t>;
		using ValueTraits = TypeTraits<uint32_t>;
		static constexpr std::string_view Identifier = "%vec2_uint32";
		static constexpr std::string_view Declaration = "%vec2_uint32 = OpTypeVector %uint32 2";
	};

	/**
	 * Vec2 int64_t specialization.
	 */
	template<>
	struct TypeTraits<Vec2<int64_t>>
	{
		using Type = Vec2<int64_t>;
		using ValueTraits = TypeTraits<int64_t>;
		static constexpr std::string_view Identifier = "%vec2_int64";
		static constexpr std::string_view Declaration = "%vec2_int64 = OpTypeVector %int64 2";
	};

	/**
	 * Vec2 uint64_t specialization.
	 */
	template<>
	struct TypeTraits<Vec2<uint64_t>>
	{
		using Type = Vec2<uint64_t>;
		using ValueTraits = TypeTraits<uint64_t>;
		static constexpr std::string_view Identifier = "%vec2_uint64";
		static constexpr std::string_view Declaration = "%vec2_uint64 = OpTypeVector %uint64 2";
	};

	/**
	 * Vec2 float specialization.
	 */
	template<>
	struct TypeTraits<Vec2<float>>
	{
		using Type = Vec2<float>;
		using ValueTraits = TypeTraits<float>;
		static constexpr std::string_view Identifier = "%vec2_float";
		static constexpr std::string_view Declaration = "%vec2_float = OpTypeVector %float 2";
	};

	/**
	 * Vec2 double specialization.
	 */
	template<>
	struct TypeTraits<Vec2<double>>
	{
		using Type = Vec2<double>;
		using ValueTraits = TypeTraits<double>;
		static constexpr std::string_view Identifier = "%vec2_double";
		static constexpr std::string_view Declaration = "%vec2_double = OpTypeVector %double 2";
	};

	/**
	 * Is complex type boolean specialization
	 */
	template<class Type>
	constexpr bool IsCompexType<Vec2<Type>> = true;
} // namespace ShaderBuilder