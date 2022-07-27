// Copyright (c) 2022 Dhiraj Wishal
#pragma once

#include "DataType.hpp"

namespace ShaderBuilder
{
	/**
	 * Vector 4 data type.
	 */
	class Vec4 final : public DataType
	{
	public:
		/**
		 * Explicit constructor.
		 *
		 * @param variableName The name of the variable.
		 * @param codeStream The code stream to store the data in.
		 * @param meta The meta information regarding the type.
		 */
		explicit Vec4(std::string&& variableName, CodeStream& codeStream, MetaInformation meta = MetaInformation::Variable);

		/**
		 * Explicit constructor.
		 *
		 * @param r The red value.
		 * @param g The green value.
		 * @param b	The blue value.
		 * @param a	The alpha value.
		 * @param variableName The name of the variable.
		 * @param codeStream The code stream to store the data in.
		 * @param meta The meta information regarding the type.
		 */
		explicit Vec4(float r, float g, float b, float a, std::string&& variableName, CodeStream& codeStream, MetaInformation meta = MetaInformation::Variable);

		/**
		 * Assignment operator.
		 *
		 * @param other The other vector.
		 * @return The altered vector reference.
		 */
		Vec4& operator=(const Vec4& other);

	public:
		union
		{
			struct { float x, y, z, w; };
			struct { float r, g, b, a; };
		};
	};
} // namespace ShaderBuilder