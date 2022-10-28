// Copyright (c) 2022 Dhiraj Wishal

#pragma once

#include "DataType.hpp"

namespace ShaderBuilder
{
	/**
	 * Parameter class.
	 * This class is used when defining functions as parameters.
	 *
	 * @tparam Type The parameter type.
	 */
	template<class Type>
	class Parameter final : public DataType<Parameter<Type>>
	{
		using Super = DataType<Parameter<Type>>;

	public:
		/**
		 * Converting constructor.
		 *
		 * @param data The data to construct the parameter with.
		 */
		Parameter(Type data) : Super(data.getSource(), data.getSource().getUniqueIdentifier()), m_Data(data)
		{
			Super::m_Source.registerType<Type>();
			Super::m_Source.getCurrentFunctionBlock().m_Parameters.insert(fmt::format("%{} = OpFunctionParameter {}", Super::m_VariableName, TypeTraits<Type>::Identifier));
		}

		/**
		 * Get the stored value.
		 *
		 * @return The const value reference.
		 */
		[[nodiscard]] const Type& value() const { return m_Data; }

		/**
		 * Get the stored value.
		 *
		 * @return The value reference.
		 */
		[[nodiscard]] Type& value() { return m_Data; }

		/**
		 * Get the stored value.
		 *
		 * @return The const value reference.
		 */
		[[nodiscard]] operator const Type& () const { return m_Data; }

		/**
		 * Get the stored value.
		 *
		 * @return The value reference.
		 */
		[[nodiscard]] operator Type& () { return m_Data; }

		/**
		 * The stored data assignment operator.
		 *
		 * @param other The data to store.
		 * @return The stored data reference.
		 */
		Type& operator=(const Type& other) { m_Data = other; return m_Data; }

	private:
		Type m_Data;
	};

	/**
	 * Parameter type traits.
	 *
	 * @tparam ValueType
	 */
	template<class ValueType>
	struct TypeTraits<Parameter<ValueType>>
	{
		using Type = ValueType;
		using ValueTraits = TypeTraits<Type>::ValueTraits;
		static constexpr const char* RawIdentifier = TypeTraits<Type>::RawIdentifier;
		static constexpr const char* Identifier = TypeTraits<Type>::Identifier;
		static constexpr const char* Declaration = TypeTraits<Type>::Declaration;
		static constexpr uint8_t Size = TypeTraits<Type>::Size;
	};
}