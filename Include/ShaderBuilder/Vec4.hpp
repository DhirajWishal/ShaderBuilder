// Copyright (c) 2022 Dhiraj Wishal

#pragma once

#include "Vec3.hpp"

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
		 * @param source The source to insert the instructions to.
		 * @param variableName The name of the variable.
		 */
		explicit Vec4(SPIRVSource& source, const std::string& variableName) : DataType<Vec4<Type>>(source, variableName), x(0), y(0), z(0), w(0) {}

		/**
		 * Explicit constructor.
		 *
		 * @param location The location of the attribute.
		 * @param isInput Whether or not the attribute is input or not.
		 * @param source The source to record all the instructions to.
		 * @param variableName The name of the variable.
		 */
		explicit Vec4(uint32_t location, bool isInput, SPIRVSource& source, const std::string& variableName) : DataType<Vec4<Type>>(location, isInput, source, variableName), x(0), y(0), z(0), w(0) {}

		/**
		 * Explicit constructor.
		 *
		 * @param source The source to insert the instructions to.
		 * @param variableName The name of the variable.
		 * @param value The value to initialize the type with.
		 */
		explicit Vec4(SPIRVSource& source, const std::string& variableName, Type value) : DataType<Vec4<Type>>(source, variableName), x(value), y(value), z(value), w(value)
		{
			const auto identifier = GetConstantIdentifier<uint64_t>(value);

			// Setup the values.
			source.insertType(std::format("%{} = OpConstant {} {}", identifier, TypeTraits<Type>::Identifier, value));

			// Setup the vector constant.
			const Type vector[] = { value, value, value, value };
			const auto hash = GenerateHash(vector, sizeof(vector));
			source.insertType(std::format("%composite_{} = OpConstantComposite {} %{} %{} %{} %{}", hash, Traits::Identifier
				, identifier
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
		 * @param w The w to initialize the w member with.
		 */
		explicit Vec4(SPIRVSource& source, const std::string& variableName, Type x, Type y, Type z, Type w) : DataType<Vec4<Type>>(source, variableName), x(x), y(y), z(z), w(w)
		{
			const auto xIdentifier = GetConstantIdentifier<uint64_t>(x);
			const auto yIdentifier = GetConstantIdentifier<uint64_t>(y);
			const auto zIdentifier = GetConstantIdentifier<uint64_t>(z);
			const auto wIdentifier = GetConstantIdentifier<uint64_t>(w);

			// Setup the values.
			source.insertType(std::format("%{} = OpConstant {} {}", xIdentifier, TypeTraits<Type>::Identifier, x));
			source.insertType(std::format("%{} = OpConstant {} {}", yIdentifier, TypeTraits<Type>::Identifier, y));
			source.insertType(std::format("%{} = OpConstant {} {}", zIdentifier, TypeTraits<Type>::Identifier, z));
			source.insertType(std::format("%{} = OpConstant {} {}", wIdentifier, TypeTraits<Type>::Identifier, w));

			// Setup the vector constant.
			const Type vector[] = { x, y, z, w };
			const auto hash = GenerateHash(vector, sizeof(vector));
			source.insertType(std::format("%composite_{} = OpConstantComposite {} %{} %{} %{} %{}", hash, Traits::Identifier
				, xIdentifier
				, yIdentifier
				, zIdentifier
				, wIdentifier
			));

			// Initialization happens only within function definitions, so we can simply assign it there.
			source.getCurrentFunctionBlock().m_Instructions.insert(std::format("OpStore %{} %composite_{}", variableName, hash));
		}

		/**
		 * Explicit constructor.
		 *
		 * @param source The source to insert the instructions to.
		 * @param variableName The name of the variable.
		 * @param vec The vec2 to initialize vec4.
		 * @param z The z to initialize the z member with.
		 * @param w The w to initialize the w member with.
		 */
		explicit Vec4(SPIRVSource& source, const std::string& variableName, const Vec2<Type>& vec, Type z, Type w) : DataType<Vec4<Type>>(source, variableName), x(vec.x), y(vec.y), z(z), w(w)
		{
			const auto zIdentifier = GetConstantIdentifier<uint64_t>(z);
			const auto wIdentifier = GetConstantIdentifier<uint64_t>(w);

			// Setup the values.
			source.insertType(std::format("%{} = OpConstant {} {}", zIdentifier, TypeTraits<Type>::Identifier, z));
			source.insertType(std::format("%{} = OpConstant {} {}", wIdentifier, TypeTraits<Type>::Identifier, w));

			// Load the memory.
			const auto variableIdentifier = std::format("%{}", source.getUniqueIdentifier());
			source.getCurrentFunctionBlock().m_Instructions.insert(std::format("{} = OpLoad {} %{}", variableIdentifier, TypeTraits<Vec2<Type>>::Identifier, vec.getName()));

			const auto xIdentifier = std::format("%{}", DataType<Vec4<Type>>::m_Source.getUniqueIdentifier());
			const auto yIdentifier = std::format("%{}", DataType<Vec4<Type>>::m_Source.getUniqueIdentifier());

			source.getCurrentFunctionBlock().m_Instructions.insert(std::format("{} = OpCompositeExtract {} {} 0", xIdentifier, TypeTraits<Type>::Identifier, variableIdentifier));
			source.getCurrentFunctionBlock().m_Instructions.insert(std::format("{} = OpCompositeExtract {} {} 1", yIdentifier, TypeTraits<Type>::Identifier, variableIdentifier));

			// Create the composite.
			const auto compositeIdentifier = std::format("%{}", source.getUniqueIdentifier());
			source.getCurrentFunctionBlock().m_Instructions.insert(std::format("{} = OpCompositeConstruct {} {} {} %{} %{}", compositeIdentifier, Traits::Identifier
				, xIdentifier
				, yIdentifier
				, zIdentifier
				, wIdentifier
			));

			// Store it.
			source.getCurrentFunctionBlock().m_Instructions.insert(std::format("OpStore %{} {}", variableName, compositeIdentifier));
		}

		/**
		 * Explicit constructor.
		 *
		 * @param source The source to insert the instructions to.
		 * @param variableName The name of the variable.
		 * @param x The x to initialize the x member with.
		 * @param vec The vec2 to initialize vec4.
		 * @param w The w to initialize the w member with.
		 */
		explicit Vec4(SPIRVSource& source, const std::string& variableName, Type x, const Vec2<Type>& vec, Type w) : DataType<Vec4<Type>>(source, variableName), x(x), y(vec.x), z(vec.y), w(w)
		{
			const auto xIdentifier = GetConstantIdentifier<uint64_t>(x);
			const auto wIdentifier = GetConstantIdentifier<uint64_t>(w);

			// Setup the values.
			source.insertType(std::format("%{} = OpConstant {} {}", xIdentifier, TypeTraits<Type>::Identifier, x));
			source.insertType(std::format("%{} = OpConstant {} {}", wIdentifier, TypeTraits<Type>::Identifier, w));

			// Load the memory.
			const auto variableIdentifier = std::format("%{}", source.getUniqueIdentifier());
			source.getCurrentFunctionBlock().m_Instructions.insert(std::format("{} = OpLoad {} %{}", variableIdentifier, TypeTraits<Vec2<Type>>::Identifier, vec.getName()));

			const auto yIdentifier = std::format("%{}", DataType<Vec4<Type>>::m_Source.getUniqueIdentifier());
			const auto zIdentifier = std::format("%{}", DataType<Vec4<Type>>::m_Source.getUniqueIdentifier());

			source.getCurrentFunctionBlock().m_Instructions.insert(std::format("{} = OpCompositeExtract {} {} 0", yIdentifier, TypeTraits<Type>::Identifier, variableIdentifier));
			source.getCurrentFunctionBlock().m_Instructions.insert(std::format("{} = OpCompositeExtract {} {} 1", zIdentifier, TypeTraits<Type>::Identifier, variableIdentifier));

			// Create the composite.
			const auto compositeIdentifier = std::format("%{}", source.getUniqueIdentifier());
			source.getCurrentFunctionBlock().m_Instructions.insert(std::format("{} = OpCompositeConstruct {} %{} {} {} %{}", compositeIdentifier, Traits::Identifier
				, xIdentifier
				, yIdentifier
				, zIdentifier
				, wIdentifier
			));

			// Store it.
			source.getCurrentFunctionBlock().m_Instructions.insert(std::format("OpStore %{} {}", variableName, compositeIdentifier));
		}

		/**
		 * Explicit constructor.
		 *
		 * @param source The source to insert the instructions to.
		 * @param variableName The name of the variable.
		 * @param x The x to initialize the x member with.
		 * @param y The y to initialize the y member with.
		 * @param vec The vec2 to initialize vec3.
		 */
		explicit Vec4(SPIRVSource& source, const std::string& variableName, Type x, Type y, const Vec2<Type>& vec) : DataType<Vec4<Type>>(source, variableName), x(x), y(y), z(vec.x), w(vec.y)
		{
			const auto xIdentifier = GetConstantIdentifier<uint64_t>(x);
			const auto yIdentifier = GetConstantIdentifier<uint64_t>(y);

			// Setup the values.
			source.insertType(std::format("%{} = OpConstant {} {}", xIdentifier, TypeTraits<Type>::Identifier, x));
			source.insertType(std::format("%{} = OpConstant {} {}", yIdentifier, TypeTraits<Type>::Identifier, y));

			// Load the memory.
			const auto variableIdentifier = std::format("%{}", source.getUniqueIdentifier());
			source.getCurrentFunctionBlock().m_Instructions.insert(std::format("{} = OpLoad {} %{}", variableIdentifier, TypeTraits<Vec2<Type>>::Identifier, vec.getName()));

			const auto zIdentifier = std::format("%{}", DataType<Vec4<Type>>::m_Source.getUniqueIdentifier());
			const auto wIdentifier = std::format("%{}", DataType<Vec4<Type>>::m_Source.getUniqueIdentifier());

			source.getCurrentFunctionBlock().m_Instructions.insert(std::format("{} = OpCompositeExtract {} {} 0", zIdentifier, TypeTraits<Type>::Identifier, variableIdentifier));
			source.getCurrentFunctionBlock().m_Instructions.insert(std::format("{} = OpCompositeExtract {} {} 1", wIdentifier, TypeTraits<Type>::Identifier, variableIdentifier));

			// Create the composite.
			const auto compositeIdentifier = std::format("%{}", source.getUniqueIdentifier());
			source.getCurrentFunctionBlock().m_Instructions.insert(std::format("{} = OpCompositeConstruct {} %{} %{} {} {}", compositeIdentifier, Traits::Identifier
				, xIdentifier
				, yIdentifier
				, zIdentifier
				, wIdentifier
			));

			// Store it.
			source.getCurrentFunctionBlock().m_Instructions.insert(std::format("OpStore %{} {}", variableName, compositeIdentifier));
		}

		/**
		 * Explicit constructor.
		 *
		 * @param source The source to insert the instructions to.
		 * @param variableName The name of the variable.
		 * @param vec The vec3 to initialize vec4.
		 * @param w The w to initialize the w member with.
		 */
		explicit Vec4(SPIRVSource& source, const std::string& variableName, const Vec3<Type>& vec, Type w) : DataType<Vec4<Type>>(source, variableName), x(vec.x), y(vec.y), z(vec.z), w(w)
		{
			const auto wIdentifier = GetConstantIdentifier<uint64_t>(w);

			// Setup the values.
			source.insertType(std::format("%{} = OpConstant {} {}", wIdentifier, TypeTraits<Type>::Identifier, w));

			// Load the memory.
			const auto variableIdentifier = std::format("%{}", source.getUniqueIdentifier());
			source.getCurrentFunctionBlock().m_Instructions.insert(std::format("{} = OpLoad {} %{}", variableIdentifier, TypeTraits<Vec3<Type>>::Identifier, vec.getName()));

			const auto xIdentifier = std::format("%{}", DataType<Vec4<Type>>::m_Source.getUniqueIdentifier());
			const auto yIdentifier = std::format("%{}", DataType<Vec4<Type>>::m_Source.getUniqueIdentifier());
			const auto zIdentifier = std::format("%{}", DataType<Vec4<Type>>::m_Source.getUniqueIdentifier());

			source.getCurrentFunctionBlock().m_Instructions.insert(std::format("{} = OpCompositeExtract {} {} 0", xIdentifier, TypeTraits<Type>::Identifier, variableIdentifier));
			source.getCurrentFunctionBlock().m_Instructions.insert(std::format("{} = OpCompositeExtract {} {} 1", yIdentifier, TypeTraits<Type>::Identifier, variableIdentifier));
			source.getCurrentFunctionBlock().m_Instructions.insert(std::format("{} = OpCompositeExtract {} {} 2", zIdentifier, TypeTraits<Type>::Identifier, variableIdentifier));

			// Create the composite.
			const auto compositeIdentifier = std::format("%{}", source.getUniqueIdentifier());
			source.getCurrentFunctionBlock().m_Instructions.insert(std::format("{} = OpCompositeConstruct {} {} {} {} %{}", compositeIdentifier, Traits::Identifier
				, xIdentifier
				, yIdentifier
				, zIdentifier
				, wIdentifier
			));

			// Store it.
			source.getCurrentFunctionBlock().m_Instructions.insert(std::format("OpStore %{} {}", variableName, compositeIdentifier));
		}

		/**
		 * Explicit constructor.
		 *
		 * @param source The source to insert the instructions to.
		 * @param variableName The name of the variable.
		 * @param x The x to initialize the x member with.
		 * @param vec The vec3 to initialize vec4.
		 */
		explicit Vec4(SPIRVSource& source, const std::string& variableName, Type x, const Vec3<Type>& vec) : DataType<Vec4<Type>>(source, variableName), x(x), y(vec.x), z(vec.y), w(vec.z)
		{
			const auto xIdentifier = GetConstantIdentifier<uint64_t>(x);

			// Setup the values.
			source.insertType(std::format("%{} = OpConstant {} {}", xIdentifier, TypeTraits<Type>::Identifier, x));

			// Load the memory.
			const auto variableIdentifier = std::format("%{}", source.getUniqueIdentifier());
			source.getCurrentFunctionBlock().m_Instructions.insert(std::format("{} = OpLoad {} %{}", variableIdentifier, TypeTraits<Vec3<Type>>::Identifier, vec.getName()));

			const auto yIdentifier = std::format("%{}", DataType<Vec4<Type>>::m_Source.getUniqueIdentifier());
			const auto zIdentifier = std::format("%{}", DataType<Vec4<Type>>::m_Source.getUniqueIdentifier());
			const auto wIdentifier = std::format("%{}", DataType<Vec4<Type>>::m_Source.getUniqueIdentifier());

			source.getCurrentFunctionBlock().m_Instructions.insert(std::format("{} = OpCompositeExtract {} {} 0", yIdentifier, TypeTraits<Type>::Identifier, variableIdentifier));
			source.getCurrentFunctionBlock().m_Instructions.insert(std::format("{} = OpCompositeExtract {} {} 1", zIdentifier, TypeTraits<Type>::Identifier, variableIdentifier));
			source.getCurrentFunctionBlock().m_Instructions.insert(std::format("{} = OpCompositeExtract {} {} 2", wIdentifier, TypeTraits<Type>::Identifier, variableIdentifier));

			// Create the composite.
			const auto compositeIdentifier = std::format("%{}", source.getUniqueIdentifier());
			source.getCurrentFunctionBlock().m_Instructions.insert(std::format("{} = OpCompositeConstruct {} %{} {} {} {}", compositeIdentifier, Traits::Identifier
				, xIdentifier
				, yIdentifier
				, zIdentifier
				, wIdentifier
			));

			// Store it.
			source.getCurrentFunctionBlock().m_Instructions.insert(std::format("OpStore %{} {}", variableName, compositeIdentifier));
		}

		/**
		 * Assignment operator.
		 *
		 * @param other The other vector.
		 * @return The altered vector reference.
		 */
		Vec4& operator=(const Vec4& other)
		{
			DataType<Vec2<Type>>::m_Source.getCurrentFunctionBlock().m_Instructions.insert(std::format("OpCopyMemory %{} %{}", DataType<Vec4<Type>>::m_VariableName, other.getName()));

			x = other.x;
			y = other.y;
			z = other.z;
			w = other.w;

			return *this;
		}

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
		static constexpr const char RawIdentifier[] = "vec4_int8";
		static constexpr const char Identifier[] = "%vec4_int8";
		static constexpr const char Declaration[] = "OpTypeVector %int8 4";
		static constexpr uint8_t Size = sizeof(ValueTraits::Type) * 4;
	};

	/**
	 * Vec4 uint8_t specialization.
	 */
	template<>
	struct TypeTraits<Vec4<uint8_t>>
	{
		using Type = Vec4<uint8_t>;
		using ValueTraits = TypeTraits<uint8_t>;
		static constexpr const char RawIdentifier[] = "vec4_uint8";
		static constexpr const char Identifier[] = "%vec4_uint8";
		static constexpr const char Declaration[] = "OpTypeVector %uint8 4";
		static constexpr uint8_t Size = sizeof(ValueTraits::Type) * 4;
	};

	/**
	 * Vec4 int16_t specialization.
	 */
	template<>
	struct TypeTraits<Vec4<int16_t>>
	{
		using Type = Vec4<int16_t>;
		using ValueTraits = TypeTraits<int16_t>;
		static constexpr const char RawIdentifier[] = "vec4_int16";
		static constexpr const char Identifier[] = "%vec4_int16";
		static constexpr const char Declaration[] = "OpTypeVector %int16 4";
		static constexpr uint8_t Size = sizeof(ValueTraits::Type) * 4;
	};

	/**
	 * Vec4 uint16_t specialization.
	 */
	template<>
	struct TypeTraits<Vec4<uint16_t>>
	{
		using Type = Vec4<uint16_t>;
		using ValueTraits = TypeTraits<uint16_t>;
		static constexpr const char RawIdentifier[] = "vec4_uint16";
		static constexpr const char Identifier[] = "%vec4_uint16";
		static constexpr const char Declaration[] = "OpTypeVector %uint16 4";
		static constexpr uint8_t Size = sizeof(ValueTraits::Type) * 4;
	};

	/**
	 * Vec4 int32_t specialization.
	 */
	template<>
	struct TypeTraits<Vec4<int32_t>>
	{
		using Type = Vec4<int32_t>;
		using ValueTraits = TypeTraits<int32_t>;
		static constexpr const char RawIdentifier[] = "vec4_int32";
		static constexpr const char Identifier[] = "%vec4_int32";
		static constexpr const char Declaration[] = "OpTypeVector %int32 4";
		static constexpr uint8_t Size = sizeof(ValueTraits::Type) * 4;
	};

	/**
	 * Vec4 uint32_t specialization.
	 */
	template<>
	struct TypeTraits<Vec4<uint32_t>>
	{
		using Type = Vec4<uint32_t>;
		using ValueTraits = TypeTraits<uint32_t>;
		static constexpr const char RawIdentifier[] = "vec4_uint32";
		static constexpr const char Identifier[] = "%vec4_uint32";
		static constexpr const char Declaration[] = "OpTypeVector %uint32 4";
		static constexpr uint8_t Size = sizeof(ValueTraits::Type) * 4;
	};

	/**
	 * Vec4 int64_t specialization.
	 */
	template<>
	struct TypeTraits<Vec4<int64_t>>
	{
		using Type = Vec4<int64_t>;
		using ValueTraits = TypeTraits<int64_t>;
		static constexpr const char RawIdentifier[] = "vec4_int64";
		static constexpr const char Identifier[] = "%vec4_int64";
		static constexpr const char Declaration[] = "OpTypeVector %int64 4";
		static constexpr uint8_t Size = sizeof(ValueTraits::Type) * 4;
	};

	/**
	 * Vec4 uint64_t specialization.
	 */
	template<>
	struct TypeTraits<Vec4<uint64_t>>
	{
		using Type = Vec4<uint64_t>;
		using ValueTraits = TypeTraits<uint64_t>;
		static constexpr const char RawIdentifier[] = "vec4_uint64";
		static constexpr const char Identifier[] = "%vec4_uint64";
		static constexpr const char Declaration[] = "OpTypeVector %uint64 4";
		static constexpr uint8_t Size = sizeof(ValueTraits::Type) * 4;
	};

	/**
	 * Vec4 float specialization.
	 */
	template<>
	struct TypeTraits<Vec4<float>>
	{
		using Type = Vec4<float>;
		using ValueTraits = TypeTraits<float>;
		static constexpr const char RawIdentifier[] = "vec4_float";
		static constexpr const char Identifier[] = "%vec4_float";
		static constexpr const char Declaration[] = "OpTypeVector %float 4";
		static constexpr uint8_t Size = sizeof(ValueTraits::Type) * 4;
	};

	/**
	 * Vec4 double specialization.
	 */
	template<>
	struct TypeTraits<Vec4<double>>
	{
		using Type = Vec4<double>;
		using ValueTraits = TypeTraits<double>;
		static constexpr const char RawIdentifier[] = "vec4_double";
		static constexpr const char Identifier[] = "%vec4_double";
		static constexpr const char Declaration[] = "OpTypeVector %double 4";
		static constexpr uint8_t Size = sizeof(ValueTraits::Type) * 4;
	};

	/**
	 * Is complex type boolean specialization
	 */
	template<class Type>
	constexpr bool IsCompexType<Vec4<Type>> = true;
} // namespace ShaderBuilder