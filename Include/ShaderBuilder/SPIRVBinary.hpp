// Copyright (c) 2022 Dhiraj Wishal

#pragma once

#include <vector>
#include <cstdint>
#include <string>

namespace ShaderBuilder
{
	/**
	 * SPIR-V binary class.
	 * This contains the final compiled output and can be used for reflection and transpile back to high level languages.
	 */
	class SPIRVBinary final
	{
	public:
		/**
		 * Explicit constructor.
		 *
		 * @param binary The SPIR-V binary.
		 */
		explicit SPIRVBinary(std::vector<uint32_t>&& binary) : m_Binary(std::move(binary)) {}

		/**
		 * Disassemble the compiled SPIR-V binary to the assembly.
		 *
		 * @return The assembly.
		 */
		[[nodiscard]] std::string disassemble() const;

		/**
		 * Transpile the binary to GLSL.
		 *
		 * @return The GLSL code.
		 */
		[[nodsicard]] std::string getGLSL() const;

		/**
		 * Transpile the binary to HLSL.
		 *
		 * @return The HLSL code.
		 */
		[[nodsicard]] std::string getHLSL() const;

		/**
		 * Transpile the binary to MSL.
		 *
		 * @return The MSL code.
		 */
		[[nodsicard]] std::string getMSL() const;

		/**
		 * Get the stored binary data.
		 *
		 * @return The binary data vector.
		 */
		[[nodiscard]] const std::vector<uint32_t>& getBinary() const { return m_Binary; }

	private:
		std::vector<uint32_t> m_Binary;
	};
} // namespace ShaderBuilder