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
			const auto identifier = GetConstantIdentifier<uint64_t>(value);

			// Setup the values.
			source.insertType(std::format("%{} = OpConstant {} {}", identifier, TypeTraits<Type>::Identifier, value));

			// Setup the vector constant.
			const Type vector[] = { value, value, value };
			const auto hash = GenerateHash(vector, sizeof(vector));
			source.insertType(std::format("%composite_{} = OpConstantComposite {} %{} %{} %{}", hash, Traits::Identifier
				, identifier
				, identifier
				, identifier
			));

			// Initialization happens only within function definitions, so we can simply assign it there.
			source.getCurrentFunctionBlock().m_Instructions.insert(std::format("OpStore %{} %composite_{}", variableName, hash));
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
			const auto xIdentifier = GetConstantIdentifier<uint64_t>(x);
			const auto yIdentifier = GetConstantIdentifier<uint64_t>(y);
			const auto zIdentifier = GetConstantIdentifier<uint64_t>(z);

			// Setup the values.
			source.insertType(std::format("%{} = OpConstant {} {}", xIdentifier, TypeTraits<Type>::Identifier, x));
			source.insertType(std::format("%{} = OpConstant {} {}", yIdentifier, TypeTraits<Type>::Identifier, y));
			source.insertType(std::format("%{} = OpConstant {} {}", zIdentifier, TypeTraits<Type>::Identifier, z));

			// Setup the vector constant.
			const Type vector[] = { x, y, z };
			const auto hash = GenerateHash(vector, sizeof(vector));
			source.insertType(std::format("%composite_{} = OpConstantComposite {} %{} %{} %{}", hash, Traits::Identifier
				, xIdentifier
				, yIdentifier
				, zIdentifier
			));

			// Initialization happens only within function definitions, so we can simply assign it there.
			source.getCurrentFunctionBlock().m_Instructions.insert(std::format("OpStore %{} %composite_{}", variableName, hash));
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
			const auto zIdentifier = GetConstantIdentifier<uint64_t>(z);

			// Setup the values.
			source.insertType(std::format("%{} = OpConstant {} {}", zIdentifier, TypeTraits<Type>::Identifier, z));

			// Load the memory.
			const auto variableIdentifier = std::format("%{}", source.getUniqueIdentifier());
			source.getCurrentFunctionBlock().m_Instructions.insert(std::format("{} = OpLoad {} %{}", variableIdentifier, TypeTraits<Vec2<Type>>::Identifier, vec.getName()));

			const auto xIdentifier = std::format("%{}", DataType<Vec3<Type>>::m_Source.getUniqueIdentifier());
			const auto yIdentifier = std::format("%{}", DataType<Vec3<Type>>::m_Source.getUniqueIdentifier());

			source.getCurrentFunctionBlock().m_Instructions.insert(std::format("{} = OpCompositeExtract {} {} 0", xIdentifier, TypeTraits<Type>::Identifier, variableIdentifier));
			source.getCurrentFunctionBlock().m_Instructions.insert(std::format("{} = OpCompositeExtract {} {} 1", yIdentifier, TypeTraits<Type>::Identifier, variableIdentifier));

			// Create the composite.
			const auto compositeIdentifier = std::format("%{}", source.getUniqueIdentifier());
			source.getCurrentFunctionBlock().m_Instructions.insert(std::format("{} = OpCompositeConstruct {} {} {} %{}", compositeIdentifier, Traits::Identifier, xIdentifier, yIdentifier, zIdentifier));

			// Store it.
			source.getCurrentFunctionBlock().m_Instructions.insert(std::format("OpStore %{} {}", variableName, compositeIdentifier));
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
			const auto xIdentifier = GetConstantIdentifier<uint64_t>(x);

			// Setup the values.
			source.insertType(std::format("%{} = OpConstant {} {}", xIdentifier, TypeTraits<Type>::Identifier, x));

			// Load the memory.
			const auto variableIdentifier = std::format("%{}", source.getUniqueIdentifier());
			source.getCurrentFunctionBlock().m_Instructions.insert(std::format("{} = OpLoad {} %{}", variableIdentifier, TypeTraits<Vec2<Type>>::Identifier, vec.getName()));

			const auto yIdentifier = std::format("%{}", DataType<Vec3<Type>>::m_Source.getUniqueIdentifier());
			const auto zIdentifier = std::format("%{}", DataType<Vec3<Type>>::m_Source.getUniqueIdentifier());

			source.getCurrentFunctionBlock().m_Instructions.insert(std::format("{} = OpCompositeExtract {} {} 0", yIdentifier, TypeTraits<Type>::Identifier, variableIdentifier));
			source.getCurrentFunctionBlock().m_Instructions.insert(std::format("{} = OpCompositeExtract {} {} 1", zIdentifier, TypeTraits<Type>::Identifier, variableIdentifier));

			// Create the composite.
			const auto compositeIdentifier = std::format("%{}", source.getUniqueIdentifier());
			source.getCurrentFunctionBlock().m_Instructions.insert(std::format("{} = OpCompositeConstruct {} %{} {} {}", compositeIdentifier, Traits::Identifier, xIdentifier, yIdentifier, zIdentifier));

			// Store it.
			source.getCurrentFunctionBlock().m_Instructions.insert(std::format("OpStore %{} {}", variableName, compositeIdentifier));
		}

		/**
		 * Assignment operator.
		 *
		 * @param other The other vector.
		 * @return The altered vector reference.
		 */
		Vec3& operator=(const Vec3& other)
		{
			DataType<Vec2<Type>>::m_Source.getCurrentFunctionBlock().m_Instructions.insert(std::format("OpCopyMemory %{} %{}", DataType<Vec3<Type>>::m_VariableName, other.getName()));

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