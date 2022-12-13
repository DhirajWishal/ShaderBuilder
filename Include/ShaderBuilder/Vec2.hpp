// Copyright (c) 2022 Dhiraj Wishal

#pragma once

#include "DataType.hpp"
#include "Utilities.hpp"

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
		using Super = DataType<Vec2<Type>>;
		using Traits = TypeTraits<Vec2<Type>>;

	public:
		/**
		 * Explicit constructor.
		 *
		 * @param source The source to insert the instructions to.
		 * @param variableName The name of the variable.
		 */
		explicit Vec2(SPIRVSource& source, const std::string& variableName) : Super(source, variableName), x(0), y(0) {}

		// /**
		//  * Copy constructor.
		//  *
		//  * @param other The other vector.
		//  */
		// Vec2(const Vec2& other) : Super(other.m_Source, other.m_VariableName), x(other.x), y(other.y)
		// {
		// 	// Load the memory.
		// 	auto& functionBlock = Super::m_Source.getCurrentFunctionBlock();
		// 	const auto variableIdentifier = fmt::format("%{}", Super::m_Source.getUniqueIdentifier());
		// 	functionBlock.m_Instructions.insert(fmt::format("{} = OpLoad {} %{}", variableIdentifier, TypeTraits<Vec2<Type>>::Identifier, other.getName()));
		// 
		// 	const auto xIdentifier = fmt::format("%{}", Super::m_Source.getUniqueIdentifier());
		// 	const auto yIdentifier = fmt::format("%{}", Super::m_Source.getUniqueIdentifier());
		// 
		// 	functionBlock.m_Instructions.insert(fmt::format("{} = OpCompositeExtract {} {} 0", xIdentifier, TypeTraits<Type>::Identifier, variableIdentifier));
		// 	functionBlock.m_Instructions.insert(fmt::format("{} = OpCompositeExtract {} {} 1", yIdentifier, TypeTraits<Type>::Identifier, variableIdentifier));
		// 
		// 	// Create the composite.
		// 	const auto compositeIdentifier = fmt::format("%{}", Super::m_Source.getUniqueIdentifier());
		// 	functionBlock.m_Instructions.insert(fmt::format("{} = OpCompositeConstruct {} {} {}", compositeIdentifier, Traits::Identifier, xIdentifier, yIdentifier));
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
		// Vec2(Vec2&& other) noexcept : Super(other.m_Source, std::move(other.m_VariableName)), x(std::exchange(other.x, Type())), y(std::exchange(other.y, Type()))
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
		explicit Vec2(SPIRVSource& source, const std::string& variableName, const Vec2& other, bool shallow = false) : Super(source, variableName), x(other.x), y(other.y)
		{			
			// If we just need a shallow copy, return without storing any instructions.
			if (shallow)
				return;

			// Load the memory.
			auto& functionBlock = source.getCurrentFunctionBlock();
			const auto variableIdentifier = fmt::format("%{}", source.getUniqueIdentifier());
			functionBlock.m_Instructions.insert(fmt::format("{} = OpLoad {} %{}", variableIdentifier, TypeTraits<Vec2<Type>>::Identifier, other.getName()));

			const auto xIdentifier = fmt::format("%{}", Super::m_Source.getUniqueIdentifier());
			const auto yIdentifier = fmt::format("%{}", Super::m_Source.getUniqueIdentifier());

			functionBlock.m_Instructions.insert(fmt::format("{} = OpCompositeExtract {} {} 0", xIdentifier, TypeTraits<Type>::Identifier, variableIdentifier));
			functionBlock.m_Instructions.insert(fmt::format("{} = OpCompositeExtract {} {} 1", yIdentifier, TypeTraits<Type>::Identifier, variableIdentifier));

			// Create the composite.
			const auto compositeIdentifier = fmt::format("%{}", source.getUniqueIdentifier());
			functionBlock.m_Instructions.insert(fmt::format("{} = OpCompositeConstruct {} {} {}", compositeIdentifier, Traits::Identifier, xIdentifier, yIdentifier));

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
		explicit Vec2(SPIRVSource& source, const std::string& variableName, Type value) : Super(source, variableName), x(value), y(value)
		{
			const auto identifier = GetConstantIdentifier<uint64_t>(value);

			// Setup the values.
			source.insertType(fmt::format("%{} = OpConstant {} {}", identifier, TypeTraits<Type>::Identifier, value));

			// Setup the vector constant.
			const Type vector[] = { value, value };
			const auto hash = GenerateHash(vector, sizeof(vector));
			source.insertType(fmt::format("%composite_{} = OpConstantComposite {} %{} %{}", hash, Traits::Identifier, identifier, identifier));

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
		 */
		explicit Vec2(SPIRVSource& source, const std::string& variableName, Type x, Type y) : Super(source, variableName), x(x), y(y)
		{
			const auto xIdentifier = GetConstantIdentifier<uint64_t>(x);
			const auto yIdentifier = GetConstantIdentifier<uint64_t>(y);

			// Setup the values.
			source.insertType(fmt::format("%{} = OpConstant {} {}", xIdentifier, TypeTraits<Type>::Identifier, x));
			source.insertType(fmt::format("%{} = OpConstant {} {}", yIdentifier, TypeTraits<Type>::Identifier, y));

			// Setup the vector constant.
			const Type vector[] = { x, y };
			const auto hash = GenerateHash(vector, sizeof(vector));
			source.insertType(fmt::format("%composite_{} = OpConstantComposite {} %{} %{}", hash, Traits::Identifier, xIdentifier, yIdentifier));

			// Initialization happens only within function definitions, so we can simply assign it there.
			source.getCurrentFunctionBlock().m_Instructions.insert(fmt::format("OpStore %{} %composite_{}", variableName, hash));
		}

		/**
		 * Assignment operator.
		 *
		 * @param other The other vector.
		 * @return The altered vector reference.
		 */
		Vec2& operator=(const Vec2& other)
		{
			Super::m_Source.getCurrentFunctionBlock().m_Instructions.insert(fmt::format("OpCopyMemory %{} %{}", Super::m_VariableName, other.getName()));

			x = other.x;
			y = other.y;

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
		static constexpr const char RawIdentifier[] = "vec2_int8";
		static constexpr const char Identifier[] = "%vec2_int8";
		static constexpr const char Declaration[] = "OpTypeVector %int8 2";
		static constexpr uint8_t Size = sizeof(ValueTraits::Type) * 2;
	};

	/**
	 * Vec2 uint8_t specialization.
	 */
	template<>
	struct TypeTraits<Vec2<uint8_t>>
	{
		using Type = Vec2<uint8_t>;
		using ValueTraits = TypeTraits<uint8_t>;
		static constexpr const char RawIdentifier[] = "vec2_uint8";
		static constexpr const char Identifier[] = "%vec2_uint8";
		static constexpr const char Declaration[] = "OpTypeVector %uint8 2";
		static constexpr uint8_t Size = sizeof(ValueTraits::Type) * 2;
	};

	/**
	 * Vec2 int16_t specialization.
	 */
	template<>
	struct TypeTraits<Vec2<int16_t>>
	{
		using Type = Vec2<int16_t>;
		using ValueTraits = TypeTraits<int16_t>;
		static constexpr const char RawIdentifier[] = "vec2_int16";
		static constexpr const char Identifier[] = "%vec2_int16";
		static constexpr const char Declaration[] = "OpTypeVector %int16 2";
		static constexpr uint8_t Size = sizeof(ValueTraits::Type) * 2;
	};

	/**
	 * Vec2 uint16_t specialization.
	 */
	template<>
	struct TypeTraits<Vec2<uint16_t>>
	{
		using Type = Vec2<uint16_t>;
		using ValueTraits = TypeTraits<uint16_t>;
		static constexpr const char RawIdentifier[] = "vec2_uint16";
		static constexpr const char Identifier[] = "%vec2_uint16";
		static constexpr const char Declaration[] = "OpTypeVector %uint16 2";
		static constexpr uint8_t Size = sizeof(ValueTraits::Type) * 2;
	};

	/**
	 * Vec2 int32_t specialization.
	 */
	template<>
	struct TypeTraits<Vec2<int32_t>>
	{
		using Type = Vec2<int32_t>;
		using ValueTraits = TypeTraits<int32_t>;
		static constexpr const char RawIdentifier[] = "vec2_int32";
		static constexpr const char Identifier[] = "%vec2_int32";
		static constexpr const char Declaration[] = "OpTypeVector %int32 2";
		static constexpr uint8_t Size = sizeof(ValueTraits::Type) * 2;
	};

	/**
	 * Vec2 uint32_t specialization.
	 */
	template<>
	struct TypeTraits<Vec2<uint32_t>>
	{
		using Type = Vec2<uint32_t>;
		using ValueTraits = TypeTraits<uint32_t>;
		static constexpr const char RawIdentifier[] = "vec2_uint32";
		static constexpr const char Identifier[] = "%vec2_uint32";
		static constexpr const char Declaration[] = "OpTypeVector %uint32 2";
		static constexpr uint8_t Size = sizeof(ValueTraits::Type) * 2;
	};

	/**
	 * Vec2 int64_t specialization.
	 */
	template<>
	struct TypeTraits<Vec2<int64_t>>
	{
		using Type = Vec2<int64_t>;
		using ValueTraits = TypeTraits<int64_t>;
		static constexpr const char RawIdentifier[] = "vec2_int64";
		static constexpr const char Identifier[] = "%vec2_int64";
		static constexpr const char Declaration[] = "OpTypeVector %int64 2";
		static constexpr uint8_t Size = sizeof(ValueTraits::Type) * 2;
	};

	/**
	 * Vec2 uint64_t specialization.
	 */
	template<>
	struct TypeTraits<Vec2<uint64_t>>
	{
		using Type = Vec2<uint64_t>;
		using ValueTraits = TypeTraits<uint64_t>;
		static constexpr const char RawIdentifier[] = "vec2_uint64";
		static constexpr const char Identifier[] = "%vec2_uint64";
		static constexpr const char Declaration[] = "OpTypeVector %uint64 2";
		static constexpr uint8_t Size = sizeof(ValueTraits::Type) * 2;
	};

	/**
	 * Vec2 float specialization.
	 */
	template<>
	struct TypeTraits<Vec2<float>>
	{
		using Type = Vec2<float>;
		using ValueTraits = TypeTraits<float>;
		static constexpr const char RawIdentifier[] = "vec2_float";
		static constexpr const char Identifier[] = "%vec2_float";
		static constexpr const char Declaration[] = "OpTypeVector %float 2";
		static constexpr uint8_t Size = sizeof(ValueTraits::Type) * 2;
	};

	/**
	 * Vec2 double specialization.
	 */
	template<>
	struct TypeTraits<Vec2<double>>
	{
		using Type = Vec2<double>;
		using ValueTraits = TypeTraits<double>;
		static constexpr const char RawIdentifier[] = "vec2_double";
		static constexpr const char Identifier[] = "%vec2_double";
		static constexpr const char Declaration[] = "OpTypeVector %double 2";
		static constexpr uint8_t Size = sizeof(ValueTraits::Type) * 2;
	};

	/**
	 * Is complex type boolean specialization
	 */
	template<class Type>
	constexpr bool IsCompexType<Vec2<Type>> = true;
} // namespace ShaderBuilder