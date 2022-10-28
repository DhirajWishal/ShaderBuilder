// Copyright (c) 2022 Dhiraj Wishal

#pragma once

#include "Attribute.hpp"

namespace ShaderBuilder
{
	/**
	 * Input attribute type wrapper.
	 *
	 * @tparam Type The output type.
	 */
	template<class Type>
	class Output final : public Attribute<Type>
	{
		using Super = Attribute<Type>;

	public:
		/**
		 * Explicit constructor.
		 *
		 * @param source The source to record all the instructions to.
		 * @param location The output location.
		 * @param data The data to initialize the internally stored data with.
		 */
		explicit Output(SPIRVSource& source, uint32_t location) : Super(source, location)
		{
			const auto identifier = Super::m_Data.getName();
			Super::m_Source.insertType(fmt::format("%output_{} = OpTypePointer Output {}", identifier, TypeTraits<Type>::Identifier));
			Super::m_Source.insertType(fmt::format("%{} = OpVariable %output_{} Output", identifier, identifier));
		}

		/**
		 * Get the stored value.
		 *
		 * @return The const value reference.
		 */
		[[nodiscard]] const Type& value() const { return Super::m_Data; }

		/**
		 * Get the stored value.
		 *
		 * @return The value reference.
		 */
		[[nodiscard]] Type& value() { return Super::m_Data; }

		/**
		 * Get the stored value.
		 *
		 * @return The const value reference.
		 */
		[[nodiscard]] operator const Type& () const { return Super::m_Data; }

		/**
		 * Get the stored value.
		 *
		 * @return The value reference.
		 */
		[[nodiscard]] operator Type& () { return Super::m_Data; }

		/**
		 * The stored data assignment operator.
		 *
		 * @param other The data to store.
		 * @return The stored data reference.
		 */
		Type& operator=(const Type& other) { Super::m_Data = other; return Super::m_Data; }
	};
}