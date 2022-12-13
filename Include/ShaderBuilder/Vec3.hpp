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
		using Super = DataType<Vec3<Type>>;
		using Traits = TypeTraits<Vec3<Type>>;

	public:
		/**
		 * Explicit constructor.
		 *
		 * @param source The source to insert the instructions to.
		 * @param variableName The name of the variable.
		 */
		explicit Vec3(SPIRVSource& source, const std::string& variableName) : Super(source, variableName), x(0), y(0), z(0) {}

		// /**
		//  * Copy constructor.
		//  *
		//  * @param other The other vector.
		//  */
		// Vec3(const Vec3& other) : Super(other.m_Source, other.m_VariableName), x(other.x), y(other.y), z(other.z)
		// {
		// 	// Load the memory.
		// 	auto& functionBlock = Super::m_Source.getCurrentFunctionBlock();
		// 	const auto variableIdentifier = fmt::format("%{}", Super::m_Source.getUniqueIdentifier());
		// 	functionBlock.m_Instructions.insert(fmt::format("{} = OpLoad {} %{}", variableIdentifier, TypeTraits<Vec3<Type>>::Identifier, other.getName()));
		// 
		// 	const auto xIdentifier = fmt::format("%{}", Super::m_Source.getUniqueIdentifier());
		// 	const auto yIdentifier = fmt::format("%{}", Super::m_Source.getUniqueIdentifier());
		// 	const auto zIdentifier = fmt::format("%{}", Super::m_Source.getUniqueIdentifier());
		// 
		// 	functionBlock.m_Instructions.insert(fmt::format("{} = OpCompositeExtract {} {} 0", xIdentifier, TypeTraits<Type>::Identifier, variableIdentifier));
		// 	functionBlock.m_Instructions.insert(fmt::format("{} = OpCompositeExtract {} {} 1", yIdentifier, TypeTraits<Type>::Identifier, variableIdentifier));
		// 	functionBlock.m_Instructions.insert(fmt::format("{} = OpCompositeExtract {} {} 3", zIdentifier, TypeTraits<Type>::Identifier, variableIdentifier));
		// 
		// 	// Create the composite.
		// 	const auto compositeIdentifier = fmt::format("%{}", Super::m_Source.getUniqueIdentifier());
		// 	functionBlock.m_Instructions.insert(fmt::format("{} = OpCompositeConstruct {} {} {} {}", compositeIdentifier, Traits::Identifier, xIdentifier, yIdentifier, zIdentifier));
		// 
		// 	// Store it.
		// 	functionBlock.m_Instructions.insert(fmt::format("OpStore %{} {}", other.m_VariableName, compositeIdentifier));
		// }
		// 
		// /**
		//  * Move constructor.
		//  *
		//  * @param other The other vector.
		//  */
		// Vec3(Vec3&& other) noexcept : Super(other.m_Source, std::move(other.m_VariableName)), x(std::exchange(other.x, Type())), y(std::exchange(other.y, Type())), z(std::exchange(other.z, Type()))
		// {
		// }

		/**
		 * Explicit constructor.
		 *
		 * @param source The source to insert the instructions to.
		 * @param variableName The name of the variable.
		 * @param other The other to copy the data from.
		 * @param shallow Whether we need a shallow copy or not. Default is false. If a shallow copy is performed, no instructions are recorded.
		 */
		explicit Vec3(SPIRVSource& source, const std::string& variableName, const Vec3& other, bool shallow = false) : Super(source, variableName), x(other.x), y(other.y), z(other.z)
		{			
			// If we just need a shallow copy, return without storing any instructions.
			if (shallow)
				return;

			// Load the memory.
			auto& functionBlock = source.getCurrentFunctionBlock();
			const auto variableIdentifier = fmt::format("%{}", source.getUniqueIdentifier());
			functionBlock.m_Instructions.insert(fmt::format("{} = OpLoad {} %{}", variableIdentifier, TypeTraits<Vec3<Type>>::Identifier, other.getName()));

			const auto xIdentifier = fmt::format("%{}", Super::m_Source.getUniqueIdentifier());
			const auto yIdentifier = fmt::format("%{}", Super::m_Source.getUniqueIdentifier());
			const auto zIdentifier = fmt::format("%{}", Super::m_Source.getUniqueIdentifier());

			functionBlock.m_Instructions.insert(fmt::format("{} = OpCompositeExtract {} {} 0", xIdentifier, TypeTraits<Type>::Identifier, variableIdentifier));
			functionBlock.m_Instructions.insert(fmt::format("{} = OpCompositeExtract {} {} 1", yIdentifier, TypeTraits<Type>::Identifier, variableIdentifier));
			functionBlock.m_Instructions.insert(fmt::format("{} = OpCompositeExtract {} {} 3", zIdentifier, TypeTraits<Type>::Identifier, variableIdentifier));

			// Create the composite.
			const auto compositeIdentifier = fmt::format("%{}", source.getUniqueIdentifier());
			functionBlock.m_Instructions.insert(fmt::format("{} = OpCompositeConstruct {} {} {} {}", compositeIdentifier, Traits::Identifier, xIdentifier, yIdentifier, zIdentifier));

			// Store it.
			functionBlock.m_Instructions.insert(fmt::format("OpStore %{} {}", variableName, compositeIdentifier));
		}

		/**
		 * Explicit constructor.
		 *
		 * @param source The source to insert the instructions to.
		 * @param variableName The name of the variable.
		 * @param value The value to initialize the type with.
		 */
		explicit Vec3(SPIRVSource& source, const std::string& variableName, Type value) : Super(source, variableName), x(value), y(value), z(value)
		{
			const auto identifier = GetConstantIdentifier<uint64_t>(value);

			// Setup the values.
			source.insertType(fmt::format("%{} = OpConstant {} {}", identifier, TypeTraits<Type>::Identifier, value));

			// Setup the vector constant.
			const Type vector[] = { value, value, value };
			const auto hash = GenerateHash(vector, sizeof(vector));
			source.insertType(fmt::format("%composite_{} = OpConstantComposite {} %{} %{} %{}", hash, Traits::Identifier
				, identifier
				, identifier
				, identifier
			));

			// Initialization happens only within function definitions, so we can simply assign it there.
			source.getCurrentFunctionBlock().m_Instructions.insert(fmt::format("OpStore %{} %composite_{}", variableName, hash));
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
		explicit Vec3(SPIRVSource& source, const std::string& variableName, Type x, Type y, Type z) : Super(source, variableName), x(x), y(y), z(z)
		{
			const auto xIdentifier = GetConstantIdentifier<uint64_t>(x);
			const auto yIdentifier = GetConstantIdentifier<uint64_t>(y);
			const auto zIdentifier = GetConstantIdentifier<uint64_t>(z);

			// Setup the values.
			source.insertType(fmt::format("%{} = OpConstant {} {}", xIdentifier, TypeTraits<Type>::Identifier, x));
			source.insertType(fmt::format("%{} = OpConstant {} {}", yIdentifier, TypeTraits<Type>::Identifier, y));
			source.insertType(fmt::format("%{} = OpConstant {} {}", zIdentifier, TypeTraits<Type>::Identifier, z));

			// Setup the vector constant.
			const Type vector[] = { x, y, z };
			const auto hash = GenerateHash(vector, sizeof(vector));
			source.insertType(fmt::format("%composite_{} = OpConstantComposite {} %{} %{} %{}", hash, Traits::Identifier
				, xIdentifier
				, yIdentifier
				, zIdentifier
			));

			// Initialization happens only within function definitions, so we can simply assign it there.
			source.getCurrentFunctionBlock().m_Instructions.insert(fmt::format("OpStore %{} %composite_{}", variableName, hash));
		}

		/**
		 * Explicit constructor.
		 *
		 * @param source The source to insert the instructions to.
		 * @param variableName The name of the variable.
		 * @param vec The vec2 to initialize vec3.
		 * @param z The z to initialize the z member with.
		 */
		explicit Vec3(SPIRVSource& source, const std::string& variableName, const Vec2<Type>& vec, Type z) : Super(source, variableName), x(vec.x), y(vec.y), z(z)
		{
			const auto zIdentifier = GetConstantIdentifier<uint64_t>(z);

			// Setup the values.
			source.insertType(fmt::format("%{} = OpConstant {} {}", zIdentifier, TypeTraits<Type>::Identifier, z));

			// Load the memory.
			auto& functionBlock = source.getCurrentFunctionBlock();
			const auto variableIdentifier = fmt::format("%{}", source.getUniqueIdentifier());
			functionBlock.m_Instructions.insert(fmt::format("{} = OpLoad {} %{}", variableIdentifier, TypeTraits<Vec2<Type>>::Identifier, vec.getName()));

			const auto xIdentifier = fmt::format("%{}", Super::m_Source.getUniqueIdentifier());
			const auto yIdentifier = fmt::format("%{}", Super::m_Source.getUniqueIdentifier());

			functionBlock.m_Instructions.insert(fmt::format("{} = OpCompositeExtract {} {} 0", xIdentifier, TypeTraits<Type>::Identifier, variableIdentifier));
			functionBlock.m_Instructions.insert(fmt::format("{} = OpCompositeExtract {} {} 1", yIdentifier, TypeTraits<Type>::Identifier, variableIdentifier));

			// Create the composite.
			const auto compositeIdentifier = fmt::format("%{}", source.getUniqueIdentifier());
			functionBlock.m_Instructions.insert(fmt::format("{} = OpCompositeConstruct {} {} {} %{}", compositeIdentifier, Traits::Identifier, xIdentifier, yIdentifier, zIdentifier));

			// Store it.
			functionBlock.m_Instructions.insert(fmt::format("OpStore %{} {}", variableName, compositeIdentifier));
		}

		/**
		 * Explicit constructor.
		 *
		 * @param source The source to insert the instructions to.
		 * @param variableName The name of the variable.
		 * @param x The x to initialize the x member with.
		 * @param vec The vec2 to initialize vec3.
		 */
		explicit Vec3(SPIRVSource& source, const std::string& variableName, Type x, const Vec2<Type>& vec) : Super(source, variableName), x(x), y(vec.x), z(vec.y)
		{
			const auto xIdentifier = GetConstantIdentifier<uint64_t>(x);

			// Setup the values.
			source.insertType(fmt::format("%{} = OpConstant {} {}", xIdentifier, TypeTraits<Type>::Identifier, x));

			// Load the memory.
			auto& functionBlock = source.getCurrentFunctionBlock();
			const auto variableIdentifier = fmt::format("%{}", source.getUniqueIdentifier());
			functionBlock.m_Instructions.insert(fmt::format("{} = OpLoad {} %{}", variableIdentifier, TypeTraits<Vec2<Type>>::Identifier, vec.getName()));

			const auto yIdentifier = fmt::format("%{}", Super::m_Source.getUniqueIdentifier());
			const auto zIdentifier = fmt::format("%{}", Super::m_Source.getUniqueIdentifier());

			functionBlock.m_Instructions.insert(fmt::format("{} = OpCompositeExtract {} {} 0", yIdentifier, TypeTraits<Type>::Identifier, variableIdentifier));
			functionBlock.m_Instructions.insert(fmt::format("{} = OpCompositeExtract {} {} 1", zIdentifier, TypeTraits<Type>::Identifier, variableIdentifier));

			// Create the composite.
			const auto compositeIdentifier = fmt::format("%{}", source.getUniqueIdentifier());
			functionBlock.m_Instructions.insert(fmt::format("{} = OpCompositeConstruct {} %{} {} {}", compositeIdentifier, Traits::Identifier, xIdentifier, yIdentifier, zIdentifier));

			// Store it.
			functionBlock.m_Instructions.insert(fmt::format("OpStore %{} {}", variableName, compositeIdentifier));
		}

		/**
		 * Assignment operator.
		 *
		 * @param other The other vector.
		 * @return The altered vector reference.
		 */
		Vec3& operator=(const Vec3& other)
		{
			Super::m_Source.getCurrentFunctionBlock().m_Instructions.insert(fmt::format("OpCopyMemory %{} %{}", Super::m_VariableName, other.getName()));

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