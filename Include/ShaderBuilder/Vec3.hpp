// Copyright (c) 2022 Dhiraj Wishal

#pragma once

#include "Vec2.hpp"

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
		 * @param location The location of the attribute.
		 * @param isInput Whether or not the attribute is input or not.
		 * @param source The source to record all the instructions to.
		 * @param variableName The name of the variable.
		 */
		explicit Vec3(uint32_t location, bool isInput, SPIRVSource& source, const std::string& variableName) : DataType<Vec3<Type>>(location, isInput, source, variableName), x(0), y(0), z(0) {}

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
			source.insertType("%" + GetConstantIdentifier<uint64_t>(value) + " = OpConstant " + TypeTraits<Type>::Identifier + " " + std::to_string(value));

			// Setup the vector constant.
			const Type vector[] = { value, value, value };
			const auto hash = GenerateHash(vector, sizeof(vector));
			source.insertType("%composite_" + std::to_string(hash) + " = OpConstantComposite " + Traits::Identifier
				+ " %" + GetConstantIdentifier<uint64_t>(value)
				+ " %" + GetConstantIdentifier<uint64_t>(value)
				+ " %" + GetConstantIdentifier<uint64_t>(value));

			// Initialization happens only within function definitions, so we can simply assign it there.
			source.getCurrentFunctionBlock().m_Instructions.insert("OpStore %" + variableName + " %composite_" + hash);
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
			source.insertType("%" + GetConstantIdentifier<uint64_t>(x) + " = OpConstant " + TypeTraits<Type>::Identifier + " " + std::to_string(x));
			source.insertType("%" + GetConstantIdentifier<uint64_t>(y) + " = OpConstant " + TypeTraits<Type>::Identifier + " " + std::to_string(y));
			source.insertType("%" + GetConstantIdentifier<uint64_t>(z) + " = OpConstant " + TypeTraits<Type>::Identifier + " " + std::to_string(z));

			// Setup the vector constant.
			const Type vector[] = { x, y, z };
			const auto hash = GenerateHash(vector, sizeof(vector));
			source.insertType("%composite_" + std::to_string(hash) + " = OpConstantComposite " + Traits::Identifier
				+ " %" + GetConstantIdentifier<uint64_t>(x)
				+ " %" + GetConstantIdentifier<uint64_t>(y)
				+ " %" + GetConstantIdentifier<uint64_t>(z));

			// Initialization happens only within function definitions, so we can simply assign it there.
			source.getCurrentFunctionBlock().m_Instructions.insert("OpStore %" + variableName + " %composite_" + std::to_string(hash));
		}

		/**
		 * Explicit constructor.
		 *
		 * @param source The source to insert the instructions to.
		 * @param variableName The name of the variable.
		 * @param vec The vec2 to initialize vec3.
		 * @param z The z to initialize the z member with.
		 */
		explicit Vec3(SPIRVSource& source, const std::string& variableName, const Vec2<Type>& vec, Type z) : DataType<Vec3<Type>>(source, variableName), x(vec.x), y(vec.y), z(z)
		{
			// Setup the values.
			source.insertType("%" + GetConstantIdentifier<uint64_t>(z) + " = OpConstant " + TypeTraits<Type>::Identifier + " " + std::to_string(z));

			// Load the memory.
			const auto variableIdentifier = "%" + source.getUniqueIdentifier();
			source.getCurrentFunctionBlock().m_Instructions.insert(variableIdentifier + " = OpLoad " + TypeTraits<Vec2<Type>>::Identifier + " %" + vec.getName());

			const auto xIdentifier = "%" + DataType<Vec3<Type>>::m_Source.getUniqueIdentifier();
			const auto yIdentifier = "%" + DataType<Vec3<Type>>::m_Source.getUniqueIdentifier();

			source.getCurrentFunctionBlock().m_Instructions.insert(xIdentifier + " = OpCompositeExtract " + TypeTraits<Type>::Identifier + " " + variableIdentifier + " " + std::to_string(0));
			source.getCurrentFunctionBlock().m_Instructions.insert(yIdentifier + " = OpCompositeExtract " + TypeTraits<Type>::Identifier + " " + variableIdentifier + " " + std::to_string(1));

			// Create the composite.
			const auto compositeIdentifier = "%" + source.getUniqueIdentifier();
			source.getCurrentFunctionBlock().m_Instructions.insert(compositeIdentifier + " = OpCompositeConstruct " + Traits::Identifier
				+ " " + xIdentifier
				+ " " + yIdentifier
				+ " %" + GetConstantIdentifier<uint64_t>(z)
			);

			// Store it.
			source.getCurrentFunctionBlock().m_Instructions.insert("OpStore %" + variableName + " " + compositeIdentifier);
		}

		/**
		 * Explicit constructor.
		 *
		 * @param source The source to insert the instructions to.
		 * @param variableName The name of the variable.
		 * @param x The x to initialize the x member with.
		 * @param vec The vec2 to initialize vec3.
		 */
		explicit Vec3(SPIRVSource& source, const std::string& variableName, Type x, const Vec2<Type>& vec) : DataType<Vec3<Type>>(source, variableName), x(x), y(vec.x), z(vec.y)
		{
			// Setup the values.
			source.insertType("%" + GetConstantIdentifier<uint64_t>(x) + " = OpConstant " + TypeTraits<Type>::Identifier + " " + std::to_string(x));

			// Load the memory.
			const auto variableIdentifier = "%" + source.getUniqueIdentifier();
			source.getCurrentFunctionBlock().m_Instructions.insert(variableIdentifier + " = OpLoad " + TypeTraits<Vec2<Type>>::Identifier + " %" + vec.getName());

			const auto yIdentifier = "%" + DataType<Vec3<Type>>::m_Source.getUniqueIdentifier();
			const auto zIdentifier = "%" + DataType<Vec3<Type>>::m_Source.getUniqueIdentifier();

			source.getCurrentFunctionBlock().m_Instructions.insert(yIdentifier + " = OpCompositeExtract " + TypeTraits<Type>::Identifier + " " + variableIdentifier + " " + std::to_string(0));
			source.getCurrentFunctionBlock().m_Instructions.insert(zIdentifier + " = OpCompositeExtract " + TypeTraits<Type>::Identifier + " " + variableIdentifier + " " + std::to_string(1));

			// Create the composite.
			const auto compositeIdentifier = "%" + source.getUniqueIdentifier();
			source.getCurrentFunctionBlock().m_Instructions.insert(compositeIdentifier + " = OpCompositeConstruct " + Traits::Identifier
				+ " %" + GetConstantIdentifier<uint64_t>(x)
				+ " " + yIdentifier
				+ " " + zIdentifier
			);

			// Store it.
			source.getCurrentFunctionBlock().m_Instructions.insert("OpStore %" + variableName + " " + compositeIdentifier);
		}

		/**
		 * Assignment operator.
		 *
		 * @param other The other vector.
		 * @return The altered vector reference.
		 */
		Vec3& operator=(const Vec3& other)
		{
			DataType<Vec3<Type>>::m_Source.getCurrentFunctionBlock().m_Instructions.insert("OpCopyMemory %" + DataType<Vec3<Type>>::m_VariableName + " %" + other.getName());

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
		static constexpr const char Declaration[] = "OpTypeVector %int8 3";
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
		static constexpr const char Declaration[] = "OpTypeVector %uint8 3";
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
		static constexpr const char Declaration[] = "OpTypeVector %int16 3";
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
		static constexpr const char Declaration[] = "OpTypeVector %uint16 3";
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
		static constexpr const char Declaration[] = "OpTypeVector %int32 3";
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
		static constexpr const char Declaration[] = "OpTypeVector %uint32 3";
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
		static constexpr const char Declaration[] = "OpTypeVector %int64 3";
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
		static constexpr const char Declaration[] = "OpTypeVector %uint64 3";
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
		static constexpr const char Declaration[] = "OpTypeVector %float 3";
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
		static constexpr const char Declaration[] = "OpTypeVector %double 3";
		static constexpr uint8_t Size = sizeof(ValueTraits::Type) * 3;
	};

	/**
	 * Is complex type boolean specialization
	 */
	template<class Type>
	constexpr bool IsCompexType<Vec3<Type>> = true;
} // namespace ShaderBuilder