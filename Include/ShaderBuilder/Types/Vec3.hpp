// Copyright (c) 2022 Dhiraj Wishal
#pragma once

#include "DataType.hpp"

namespace ShaderBuilder
{
	/**
	 * Vector 3 data type.
	 */
	class Vec3 final : public DataType
	{
	public:
		/**
		 * Explicit constructor.
		 *
		 * @param variableName The name of the variable.
		 * @param codeStream The code stream to store the data in.
		 */
		explicit Vec3(std::string&& variableName, CodeStream& codeStream);
	};
} // namespace ShaderBuilder