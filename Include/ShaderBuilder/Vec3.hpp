// Copyright (c) 2022 Dhiraj Wishal

#pragma once

#include "DataType.hpp"

namespace ShaderBuilder
{
	/**
	 * Vector 3 data type.
	 * 
	 * @tparam Type The value type. Default is float.
	 */
	template<class Type = float>
	class Vec3 final : public DataType<Vec3<Type>>
	{
	public:
		using Traits = TypeTraits<Vec3<Type>>;

	public:
		/**
		 * Explicit constructor.
		 *
		 * @param variableName The name of the variable.
		 */
		explicit Vec3(std::string&& variableName) : DataType<Vec3<Type>>(std::move(variableName)) {}

	public:
		union
		{
			struct { Type x, y, z; };
			struct { Type r, g, b; };
			struct { Type u, v, w; };
		};
	};

	/**
	 * Vec3 int8_t specialization.
	 */
	template<>
	struct TypeTraits<Vec3<int8_t>>
	{
		using Type = Vec3<int8_t>;
		using ValueTraits = TypeTraits<int8_t>;
		static constexpr const char Identifier[] = "%vec3_int8";
		static constexpr const char Declaration[] = "%vec3_int8 = OpTypeVector %int8 3";
		static constexpr uint8_t Size = sizeof(ValueTraits::Type) * 3;
	};

	/**
	 * Vec3 uint8_t specialization.
	 */
	template<>
	struct TypeTraits<Vec3<uint8_t>>
	{
		using Type = Vec3<uint8_t>;
		using ValueTraits = TypeTraits<uint8_t>;
		static constexpr const char Identifier[] = "%vec3_uint8";
		static constexpr const char Declaration[] = "%vec3_uint8 = OpTypeVector %uint8 3";
		static constexpr uint8_t Size = sizeof(ValueTraits::Type) * 3;
	};

	/**
	 * Vec3 int16_t specialization.
	 */
	template<>
	struct TypeTraits<Vec3<int16_t>>
	{
		using Type = Vec3<int16_t>;
		using ValueTraits = TypeTraits<int16_t>;
		static constexpr const char Identifier[] = "%vec3_int16";
		static constexpr const char Declaration[] = "%vec3_int16 = OpTypeVector %int16 3";
		static constexpr uint8_t Size = sizeof(ValueTraits::Type) * 3;
	};

	/**
	 * Vec3 uint16_t specialization.
	 */
	template<>
	struct TypeTraits<Vec3<uint16_t>>
	{
		using Type = Vec3<uint16_t>;
		using ValueTraits = TypeTraits<uint16_t>;
		static constexpr const char Identifier[] = "%vec3_uint16";
		static constexpr const char Declaration[] = "%vec3_uint16 = OpTypeVector %uint16 3";
		static constexpr uint8_t Size = sizeof(ValueTraits::Type) * 3;
	};

	/**
	 * Vec3 int32_t specialization.
	 */
	template<>
	struct TypeTraits<Vec3<int32_t>>
	{
		using Type = Vec3<int32_t>;
		using ValueTraits = TypeTraits<int32_t>;
		static constexpr const char Identifier[] = "%vec3_int32";
		static constexpr const char Declaration[] = "%vec3_int32 = OpTypeVector %int32 3";
		static constexpr uint8_t Size = sizeof(ValueTraits::Type) * 3;
	};

	/**
	 * Vec3 uint32_t specialization.
	 */
	template<>
	struct TypeTraits<Vec3<uint32_t>>
	{
		using Type = Vec3<uint32_t>;
		using ValueTraits = TypeTraits<uint32_t>;
		static constexpr const char Identifier[] = "%vec3_uint32";
		static constexpr const char Declaration[] = "%vec3_uint32 = OpTypeVector %uint32 3";
		static constexpr uint8_t Size = sizeof(ValueTraits::Type) * 3;
	};

	/**
	 * Vec3 int64_t specialization.
	 */
	template<>
	struct TypeTraits<Vec3<int64_t>>
	{
		using Type = Vec3<int64_t>;
		using ValueTraits = TypeTraits<int64_t>;
		static constexpr const char Identifier[] = "%vec3_int64";
		static constexpr const char Declaration[] = "%vec3_int64 = OpTypeVector %int64 3";
		static constexpr uint8_t Size = sizeof(ValueTraits::Type) * 3;
	};

	/**
	 * Vec3 uint64_t specialization.
	 */
	template<>
	struct TypeTraits<Vec3<uint64_t>>
	{
		using Type = Vec3<uint64_t>;
		using ValueTraits = TypeTraits<uint64_t>;
		static constexpr const char Identifier[] = "%vec3_uint64";
		static constexpr const char Declaration[] = "%vec3_uint64 = OpTypeVector %uint64 3";
		static constexpr uint8_t Size = sizeof(ValueTraits::Type) * 3;
	};

	/**
	 * Vec3 float specialization.
	 */
	template<>
	struct TypeTraits<Vec3<float>>
	{
		using Type = Vec3<float>;
		using ValueTraits = TypeTraits<float>;
		static constexpr const char Identifier[] = "%vec3_float";
		static constexpr const char Declaration[] = "%vec3_float = OpTypeVector %float 3";
		static constexpr uint8_t Size = sizeof(ValueTraits::Type) * 3;
	};

	/**
	 * Vec3 double specialization.
	 */
	template<>
	struct TypeTraits<Vec3<double>>
	{
		using Type = Vec3<double>;
		using ValueTraits = TypeTraits<double>;
		static constexpr const char Identifier[] = "%vec3_double";
		static constexpr const char Declaration[] = "%vec3_double = OpTypeVector %double 3";
		static constexpr uint8_t Size = sizeof(ValueTraits::Type) * 3;
	};

	/**
	 * Is complex type boolean specialization
	 */
	template<class Type>
	constexpr bool IsCompexType<Vec3<Type>> = true;
} // namespace ShaderBuilder