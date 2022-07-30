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
		
	};
} // namespace ShaderBuilder