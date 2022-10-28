// Copyright (c) 2022 Dhiraj Wishal

#pragma once

#include "Attribute.hpp"

namespace ShaderBuilder
{
	/**
	 * Input attribute type wrapper.
	 *
	 * @tparam Type The input type.
	 */
	template<class Type>
	class Input final : public Attribute<Type>
	{
		using Super = Attribute<Type>;

	public:
		/**
		 * Explicit constructor.
		 *
		 * @param source The source to record all the instructions to.
		 * @param location The input location.
		 */
		explicit Input(SPIRVSource& source, uint32_t location) : Super(source, location)
		{
			const auto identifier = Super::m_Data.getName();
			Super::m_Source.insertType(fmt::format("%input_{} = OpTypePointer Input {}", identifier, TypeTraits<Type>::Identifier));
			Super::m_Source.insertType(fmt::format("%{} = OpVariable %input_{} Input", identifier, identifier));
		}

		/**
		 * Get the stored value.
		 *
		 * @return The const value.
		 */
		[[nodiscard]] const Type value() const { return Super::m_Data; }

		/**
		 * Get the stored value.
		 *
		 * @return The const value.
		 */
		[[nodiscard]] operator const Type() const { return Super::m_Data; }
	};
}