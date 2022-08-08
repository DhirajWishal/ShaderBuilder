// Copyright (c) 2022 Dhiraj Wishal

#pragma once

#include "TypeStorage.hpp"

namespace ShaderBuilder
{
	/**
	 * Input storage class.
	 * This class is used to store input attribute information.
	 *
	 * @tparam Type The data type.
	 */
	template<class Type>
	class InputStorage final : public TypeStorage
	{
	public:
		/**
		 * Explicit constructor.
		 *
		 * @param location The location of the input.
		 * @param identifier The input identifier.
		 * @param source The SPIR-V Source.
		 */
		explicit InputStorage(uint32_t location, const std::string& identifier, SPIRVSource& source) : TypeStorage(identifier, source), m_Location(location)
		{
			m_Source.insertType("%input_" + identifier, std::string("OpTypePointer Input ") + TypeTraits<Type>::Identifier);
			m_Source.insertType("%" + identifier, std::string("OpVariable %input_") + identifier + " Input");
			m_Source.insertName("%" + identifier, identifier);
			m_Source.insertAnnotation("OpDecorate %" + identifier + " Location " + std::to_string(location));
		}

	private:
		uint32_t m_Location = 0;
	};
} // namespace ShaderBuilder