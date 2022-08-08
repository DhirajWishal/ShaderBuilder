// Copyright (c) 2022 Dhiraj Wishal

#pragma once

#include "SPIRVSource.hpp"

namespace ShaderBuilder
{
	/**
	 * Type storage class.
	 */
	class TypeStorage
	{
	public:
		/**
		 * Explicit constructor.
		 *
		 * @param identifier The input identifier.
		 * @param source The SPIR-V Source.
		 */
		explicit TypeStorage(const std::string& identifier, SPIRVSource& source) : m_Identifier(identifier), m_Source(source) {}

	private:
		std::string m_Identifier;
		SPIRVSource& m_Source;
	};
} // namespace ShaderBuilder