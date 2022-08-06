// Copyright (c) 2022 Dhiraj Wishal

#pragma once

#include "Callable.hpp"

namespace ShaderBuilder
{
	/**
	 * Function builder return type structure.
	 * This structure is used to deduce the return type of a function builder.
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
		 * @param source The source to record all the commands to.
		 * @param name The name of the function.
		 * @param returnType The return type containing the return type. Default is {};
		 */
		template<class ReturnType>
		explicit FunctionBuilder(SPIRVSource& source, const std::string& name, [[maybe_unused]] FunctionBuilderReturnType<ReturnType>&& returnType = {}) : DataType<FunctionBuilder>(source, name)
		{
			source.insertName("%" + name, name);
			m_FunctionJSON["declaration"] = "%" + name + " = OpFunction " + TypeTraits<ReturnType>::Identifier + " None " + GetFunctionIdentifier<ReturnType>();

			m_FunctionJSON["firstBlock"] = "block_" + std::to_string(source.getUniqueID());
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
		 * @param name The variable name.
		 * @param initializer The initializer types.
		 * @return The created variable.
		 */
		template<class Type, class... Types>
		[[nodiscard]] Type createVariable(std::string&& name, Types&&... initializer)
		{
			registerType<Type>();
			m_Source.insertType(std::string("%variable_type_") + TypeTraits<Type>::RawIdentifier, std::string("OpTypePointer Function ") + TypeTraits<Type>::Identifier);
			m_Source.insertName("%" + name, name);

			m_FunctionJSON["variables"][name] = std::string("OpVariable %variable_type_") + TypeTraits<Type>::RawIdentifier + " Function";

			return Type(m_Source, name, std::forward<Types>(initializer)...);
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
			m_FunctionJSON["return"] = value.getIdentifier();
			exit();
		}

		/**
		 * Exit the function by returning nothing.
		 */
		void exit();

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

			m_Source.insertType(TypeTraits<Type>::Identifier, TypeTraits<Type>::Declaration);
		}

	private:
		Json m_FunctionJSON;
	};
} // namespace ShaderBuilder