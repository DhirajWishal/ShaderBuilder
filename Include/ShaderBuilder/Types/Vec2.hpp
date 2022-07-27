// Copyright (c) 2022 Dhiraj Wishal
#pragma once

#include "DataType.hpp"

namespace ShaderBuilder
{
	/**
	 * Vector 2 data type.
	 */
	class Vec2 final : public DataType
	{
	public:
		/**
		 * Explicit constructor.
		 *
		 * @param variableName The name of the variable.
		 * @param codeStream The code stream to store the data in.
		 */
		explicit Vec2(std::string&& variableName, CodeStream& codeStream);

		/**
		 * Assignment operator.
		 *
		 * @param other The other vector.
		 * @return The altered vector reference.
		 */
		Vec2& operator=(const Vec2& other);

	public:
		union
		{
			struct { float x, y; };
			struct { float a, b; };
			struct { float u, v; };
		};
	};
} // namespace ShaderBuilder