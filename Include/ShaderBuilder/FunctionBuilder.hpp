// Copyright (c) 2022 Dhiraj Wishal

#pragma once

#include "Callable.hpp"

namespace ShaderBuilder
{
	/**
	 * Function builder return type structure.
	 * This structure is used to deduce the return type of a function builder.
	 *
	 * @tparam Type The return type of the function.
	 */
	template<class Type>
	struct FunctionBuilderReturnType {};

	/**
	 * Function builder class.
	 */
	class FunctionBuilder final : public DataType<FunctionBuilder>
	{
	public:
		/**
		 * Explicit constructor.
		 *
		 * @tparam ReturnType The return type of the function.
		 * @param source The source to record all the commands to.
		 * @param name The name of the function.
		 * @param returnType The return type containing the return type. Default is {};
		 */
		template<class ReturnType>
		explicit FunctionBuilder(SPIRVSource& source, const std::string& name, [[maybe_unused]] FunctionBuilderReturnType<ReturnType>&& returnType = {}) : DataType<FunctionBuilder>(source, name)
		{
			m_Source.insertName(fmt::format("OpName %{} \"{}\"", name, name));

			auto& block = source.createFunctionBlock();
			block.m_Name = name;
			block.m_Definition.insert(fmt::format("%{} = OpFunction {} None {}", name, TypeTraits<ReturnType>::Identifier, GetFunctionIdentifier<ReturnType>()));
		}

		/**
		 * Default destructor.
		 */
		~FunctionBuilder();

		/**
		 * Create a new variable.
		 * These variables must be created within a function.
		 *
		 * @tparam Type The type of the variable.
		 * @tparam Types The initialization types.
		 * @param initializer The initializer types.
		 * @return The created variable.
		 */
		template<class Type, class... Types>
		[[nodiscard]] Type createVariable(Types&&... initializer)
		{
			m_Source.registerType<Type>();
			m_Source.insertType(fmt::format("%variable_type_{} = OpTypePointer Function {}", TypeTraits<Type>::RawIdentifier, TypeTraits<Type>::Identifier));

			auto identifier = m_Source.getUniqueIdentifier();
			m_Source.getCurrentFunctionBlock().m_Variables.insert(fmt::format("%{} = OpVariable %variable_type_{} Function", identifier, TypeTraits<Type>::RawIdentifier));

			return Type(m_Source, std::move(identifier), std::forward<Types>(initializer)...);
		}

		/**
		 * Exit from the function by returning a value.
		 *
		 * @tparam Type The value type.
		 * @param value The value to return.
		 */
		template<class Type>
		void exit(const Type& value)
		{
			// auto& block = m_Source.getCurrentFunctionBlock();
			// block.m_Instructions.insert("OpReturn");
			m_IsComplete = true;
		}

		/**
		 * Exit the function by returning nothing.
		 */
		void exit();

	private:
		bool m_IsComplete = false;
	};
} // namespace ShaderBuilder