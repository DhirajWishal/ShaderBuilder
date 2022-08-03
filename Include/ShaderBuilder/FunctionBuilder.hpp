// Copyright (c) 2022 Dhiraj Wishal

#pragma once

#include "InstructionStores.hpp"
#include "TypeTraits.hpp"

#include <string>
#include <list>

namespace ShaderBuilder
{
	class SPIRVSource;

	/**
	 * Function builder class.
	 */
	class FunctionBuilder final
	{
	public:
		/**
		 * Explicit constructor.
		 *
		 * @param source The source to record all the commands to.
		 */
		explicit FunctionBuilder(SPIRVSource& source) : m_Source(source) {}

		/**
		 * Create a new variable.
		 * These variables must be created within a function.
		 *
		 * @tparam Type The type of the variable.
		 * @tparam Types The initialization types.
		 * @param name The variable name.
		 * @param initializer The initializer types.
		 * @return The created variable.
		 */
		template<class Type, class... Types>
		[[nodiscard]] Type createVariable(std::string&& name, Types&&... initializer)
		{
			registerType<Type>();
			m_Source.insertTypeDeclaration("%variable_type_", TypeTraits<Type>::RawIdentifier, " = OpTypePointer Function ", TypeTraits<Type>::Identifier);
			m_Source.insertDebugName("OpName %", name, " \"", name, "\"");

			m_FunctionBlock.insert("%" + name + " = OpVariable %variable_type_" + TypeTraits<Type>::RawIdentifier + " Function");

			return Type(m_Source, name, std::forward<Types>(initializer)...);
		}

	public:
		/**
		 * Convert the internal blocks to a string.
		 *
		 * @return The converted string.
		 */
		[[nodiscard]] std::string getString() const;

	private:
		/**
		 * Register type function.
		 *
		 * @tparam Type The type to register.
		 */
		template<class Type>
		void registerType()
		{
			// Try and register value types if the Type is complex.
			if constexpr (IsCompexType<Type>)
				registerType<typename TypeTraits<Type>::ValueTraits::Type>();

			m_Source.insertTypeDeclaration(TypeTraits<Type>::Declaration);
		}

	private:
		BlockInstructions m_Variables;
		BlockInstructions m_FunctionBlock;
		std::list<BlockInstructions> m_BlockInstructions;
		SPIRVSource& m_Source;
	};
} // namespace ShaderBuilder