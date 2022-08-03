// Copyright (c) 2022 Dhiraj Wishal

#pragma once

#include "DataType.hpp"

namespace ShaderBuilder
{
	/**
	 * Callable class.
	 * This class contains the return type and other information about a function.
	 */
	template<class Return, class... Arguments>
	class Callable final : public DataType<Callable<Return, Arguments...>>
	{
	public:
		/**
		 * Explicit constructor.
		 *
		 * @param source The source to record all the instructions to.
		 * @param name The name of the function.
		 */
		explicit Callable(SPIRVSource& source, const std::string& name) : DataType<Callable<Return, Arguments...>>(source, name) {}
	};

	/**
	 * Callable void specialization.
	 */
	template<class Return, class... Arguments>
	struct TypeTraits<Callable<Return, Arguments...>>
	{
		using Type = Callable<Return, Arguments...>;
		using ValueTraits = TypeTraits<Return>;
	};

	/**
	 * Get a function's identifier using the value type.
	 *
	 * @tparam ValueType The value type.
	 * @return The identifier string.
	 */
	template<class ValueType>
	[[nodiscard]] std::string GetFunctionIdentifier() { return std::string(TypeTraits<ValueType>::Identifier) + "_callable"; }
} // namespace ShaderBuilder