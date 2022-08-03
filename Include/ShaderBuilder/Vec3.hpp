// Copyright (c) 2022 Dhiraj Wishal

#pragma once

#include "DataType.hpp"
#include "Utilities.hpp"

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
		 * @param source The source to insert the instructions to.
		 * @param variableName The name of the variable.
		 */
		explicit Vec3(SPIRVSource& source, const std::string& variableName) : DataType<Vec3<Type>>(source, variableName), x(0), y(0), z(0) {}

		/**
		 * Explicit constructor.
		 *
		 * @param source The source to insert the instructions to.
		 * @param variableName The name of the variable.
		 * @param value The value to initialize the type with.
		 */
		explicit Vec3(SPIRVSource& source, const std::string& variableName, Type value) : DataType<Vec3<Type>>(source, variableName), x(value), y(value), z(value)
		{
			// Setup the values.
			source.insertTypeDeclaration("%const_", static_cast<uint64_t>(value), " = OpConstant ", TypeTraits<Type>::Identifier, " ", value);

			// Setup the vector constant.
			const Type vector[] = { value, value, value };
			const auto hash = GenerateHash(vector, sizeof(vector));
			source.insertTypeDeclaration("%composite_", hash, " = OpConstantComposite ", Traits::Identifier, " %const_", value, " %const_", value, " %const_", value);

			// Initialization happens only within function definitions, so we can simply assign it there.
			source.insertFunctionDefinition("OpStore %", variableName, " %composite_", hash);
		}

		/**
		 * Explicit constructor.
		 *
		 * @param source The source to insert the instructions to.
		 * @param variableName The name of the variable.
		 * @param x The x to initialize the x member with.
		 * @param y The y to initialize the y member with.
		 * @param z The z to initialize the z member with.
		 */
		explicit Vec3(SPIRVSource& source, const std::string& variableName, Type x, Type y, Type z) : DataType<Vec3<Type>>(source, variableName), x(x), y(y), z(z)
		{
			// Setup the values.
			source.insertTypeDeclaration("%const_", static_cast<uint64_t>(x), " = OpConstant ", TypeTraits<Type>::Identifier, " ", x);
			source.insertTypeDeclaration("%const_", static_cast<uint64_t>(y), " = OpConstant ", TypeTraits<Type>::Identifier, " ", y);
			source.insertTypeDeclaration("%const_", static_cast<uint64_t>(z), " = OpConstant ", TypeTraits<Type>::Identifier, " ", z);

			// Setup the vector constant.
			const Type vector[] = { x, y, z };
			const auto hash = GenerateHash(vector, sizeof(vector));
			source.insertTypeDeclaration("%composite_", hash, " = OpConstantComposite ", Traits::Identifier, " %const_", x, " %const_", y, " %const_", z);

			// Initialization happens only within function definitions, so we can simply assign it there.
			source.insertFunctionDefinition("OpStore %", variableName, " %composite_", hash);
		}

		/**
		 * Assignment operator.
		 *
		 * @param other The other vector.
		 * @return The altered vector reference.
		 */
		Vec3& operator=(const Vec3& other)
		{
			DataType<Vec3<Type>>::m_Source.insertFunctionDefinition("OpCopyMemory %", DataType<Vec3<Type>>::m_VariableName, " %", other.getName());

			x = other.x;
			y = other.y;
			z = other.z;

			return *this;
		}

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
		static constexpr const char RawIdentifier[] = "vec3_int8";
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
		static constexpr const char RawIdentifier[] = "vec3_uint8";
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
		static constexpr const char RawIdentifier[] = "vec3_int16";
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
		static constexpr const char RawIdentifier[] = "vec3_uint16";
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
		static constexpr const char RawIdentifier[] = "vec3_int32";
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
		static constexpr const char RawIdentifier[] = "vec3_uint32";
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
		static constexpr const char RawIdentifier[] = "vec3_int64";
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
		static constexpr const char RawIdentifier[] = "vec3_uint64";
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
		static constexpr const char RawIdentifier[] = "vec3_float";
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
		static constexpr const char RawIdentifier[] = "vec3_double";
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