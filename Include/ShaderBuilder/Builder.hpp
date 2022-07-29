// Copyright (c) 2022 Dhiraj Wishal
#pragma once

#include "Types/Callable.hpp"
#include "Types/Vec4.hpp"

namespace ShaderBuilder
{
	/**
	 * Builder class.
	 * This class contains the base code for SPIR-V generation and can be used to
	 * compile to other high level languages, like GLSL and HLSL.
	 *
	 * This class doesn't need to be inherited, so the user can use this class as a
	 * object to create the shaders.
	 */
	class Builder
	{
	public:
		/**
		 * Default constructor.
		 */
		Builder();
	};
} // namespace ShaderBuilder