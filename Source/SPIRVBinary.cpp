// Copyright (c) 2022 Dhiraj Wishal

#include "ShaderBuilder/SPIRVBinary.hpp"
#include "ShaderBuilder/BuilderError.hpp"

#include <spirv-tools/libspirv.hpp>
#include <spirv_glsl.hpp>
#include <spirv_hlsl.hpp>

namespace ShaderBuilder
{
	std::string SPIRVBinary::disassemble() const
	{
		spvtools::SpirvTools tools(SPV_ENV_UNIVERSAL_1_6);
		tools.SetMessageConsumer([](spv_message_level_t level, const char*, const spv_position_t& position, const char* message) { throw BuilderError(message); });

		std::string disassembly;
		if (!tools.Disassemble(m_Binary, &disassembly)) throw BuilderError("Failed to disassemble the binary!");

		return disassembly;
	}

	std::string SPIRVBinary::getGLSL() const
	{
		spirv_cross::CompilerGLSL glsl(m_Binary);

		spirv_cross::CompilerGLSL::Options options;
		options.version = 450;
		options.vulkan_semantics = true;
		glsl.set_common_options(options);

		return glsl.compile();
	}

	std::string SPIRVBinary::getHLSL() const
	{
		spirv_cross::CompilerHLSL hlsl(m_Binary);

		spirv_cross::CompilerHLSL::Options options;
		//hlsl.set_common_options(options);

		return hlsl.compile();
	}
} // namespace ShaderBuilder