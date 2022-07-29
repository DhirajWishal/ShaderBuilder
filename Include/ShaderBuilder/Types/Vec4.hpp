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
		static constexpr const char Identifier[] = "%vec4_int8";
		static constexpr const char OpType[] = "OpTypeVector";
		static constexpr uint8_t ComponentCount = 4;
	};

	/**
	 * Vec4 uint8_t specialization.
	 */
	template<>
	struct TypeTraits<Vec4<uint8_t>>
	{
		using Type = Vec4<uint8_t>;
		using ValueTraits = TypeTraits<uint8_t>;
		static constexpr const char Identifier[] = "%vec4_uint8";
		static constexpr const char OpType[] = "OpTypeVector";
		static constexpr uint8_t ComponentCount = 4;
	};

	/**
	 * Vec4 int16_t specialization.
	 */
	template<>
	struct TypeTraits<Vec4<int16_t>>
	{
		using Type = Vec4<int16_t>;
		using ValueTraits = TypeTraits<int16_t>;
		static constexpr const char Identifier[] = "%vec4_int16";
		static constexpr const char OpType[] = "OpTypeVector";
		static constexpr uint8_t ComponentCount = 4;
	};

	/**
	 * Vec4 uint16_t specialization.
	 */
	template<>
	struct TypeTraits<Vec4<uint16_t>>
	{
		using Type = Vec4<uint16_t>;
		using ValueTraits = TypeTraits<uint16_t>;
		static constexpr const char Identifier[] = "%vec4_uint16";
		static constexpr const char OpType[] = "OpTypeVector";
		static constexpr uint8_t ComponentCount = 4;
	};

	/**
	 * Vec4 int32_t specialization.
	 */
	template<>
	struct TypeTraits<Vec4<int32_t>>
	{
		using Type = Vec4<int32_t>;
		using ValueTraits = TypeTraits<int32_t>;
		static constexpr const char Identifier[] = "%vec4_int32";
		static constexpr const char OpType[] = "OpTypeVector";
		static constexpr uint8_t ComponentCount = 4;
	};

	/**
	 * Vec4 uint32_t specialization.
	 */
	template<>
	struct TypeTraits<Vec4<uint32_t>>
	{
		using Type = Vec4<uint32_t>;
		using ValueTraits = TypeTraits<uint32_t>;
		static constexpr const char Identifier[] = "%vec4_uint32";
		static constexpr const char OpType[] = "OpTypeVector";
		static constexpr uint8_t ComponentCount = 4;
	};

	/**
	 * Vec4 int64_t specialization.
	 */
	template<>
	struct TypeTraits<Vec4<int64_t>>
	{
		using Type = Vec4<int64_t>;
		using ValueTraits = TypeTraits<int64_t>;
		static constexpr const char Identifier[] = "%vec4_int64";
		static constexpr const char OpType[] = "OpTypeVector";
		static constexpr uint8_t ComponentCount = 4;
	};

	/**
	 * Vec4 uint64_t specialization.
	 */
	template<>
	struct TypeTraits<Vec4<uint64_t>>
	{
		using Type = Vec4<uint64_t>;
		using ValueTraits = TypeTraits<uint64_t>;
		static constexpr const char Identifier[] = "%vec4_uint64";
		static constexpr const char OpType[] = "OpTypeVector";
		static constexpr uint8_t ComponentCount = 4;
	};

	/**
	 * Vec4 float specialization.
	 */
	template<>
	struct TypeTraits<Vec4<float>>
	{
		using Type = Vec4<float>;
		using ValueTraits = TypeTraits<float>;
		static constexpr const char Identifier[] = "%vec4_float";
		static constexpr const char OpType[] = "OpTypeVector";
		static constexpr uint8_t ComponentCount = 4;
	};

	/**
	 * Vec4 double specialization.
	 */
	template<>
	struct TypeTraits<Vec4<double>>
	{
		using Type = Vec4<double>;
		using ValueTraits = TypeTraits<double>;
		static constexpr const char Identifier[] = "%vec4_double";
		static constexpr const char OpType[] = "OpTypeVector";
		static constexpr uint8_t ComponentCount = 4;
	};
} // namespace ShaderBuilder