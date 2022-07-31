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
		 */
		explicit Callable(std::string&& name) : DataType<Callable<Return, Arguments...>>(std::move(name)) {}

		/**
		 * Function call operator overload.
		 */
		decltype(auto) operator()(Arguments&&... arguments) {}
	};

	/**
	 * Callable void specialization.
	 */
	template<class... Arguments>
	struct TypeTraits<Callable<void, Arguments...>>
	{
		using Type = Callable<void, Arguments...>;
		using ValueTraits = TypeTraits<void>;
		static constexpr const char Identifier[] = "%void_callable";
		static constexpr const char Declaration[] = "%void_callable = OpTypeFunction %void";
	};
} // namespace ShaderBuilder