// Copyright (c) 2022 Dhiraj Wishal

#pragma once

#include <stdexcept>

namespace ShaderBuilder
{
	/**
	 * Builder error class.
	 * This exception is thrown from the builder class if an error was encountered.
	 */
	class BuilderError final : public std::runtime_error
	{
	public:
		using std::runtime_error::runtime_error;
	};
} // namespace ShaderBuilder