// Copyright (c) 2022 Dhiraj Wishal

#pragma once

#include <vector>
#include <cstdint>

#include <spirv/1.2/spirv.hpp>

namespace ShaderBuilder
{
	/**
	 * Instruction view structure.
	 * This structure contains information about a single SPIR-V instruction in the source buffer.
	 */
	struct InstructionView final
	{
		uint64_t m_Begin = 0;
		uint64_t m_Count = 0;
		uint64_t m_Hash = 0;
	};

	/**
	 * SPIR-V source class.
	 * This class will store all the SPIR-V source instructions which then can be compiled to SPIR-V binary.
	 */
	class SPIRVSource final
	{
		/**
		 * Function declaration structure.
		 */
		struct FunctionDeclaration final
		{
			InstructionView m_OpFunction;

			std::vector<InstructionView> m_OpFunctionParameters;
		};

		/**
		 * Function definition structure.
		 */
		struct FunctionDefinition final
		{
			FunctionDeclaration m_Declaration;

			InstructionView m_OpReturn;

			std::vector<InstructionView> m_VariableInstructions;
			std::vector<InstructionView> m_Instructions;
		};

	public:
		/**
		 * Default constructor.
		 */
		SPIRVSource() = default;

		template<class... Words>
		InstructionView insert(spv::Op opCode, Words... words)
		{
		}

	private:
		std::vector<uint32_t> m_Words;

		InstructionView m_OpMemoryModel;

		std::vector<InstructionView> m_OpCapabilities;
		std::vector<InstructionView> m_OpExtensions;
		std::vector<InstructionView> m_OpExtInstImports;

		std::vector<InstructionView> m_OpEntryPoints;
		std::vector<InstructionView> m_ExecutionModes;

		std::vector<InstructionView> m_DebugSource;
		std::vector<InstructionView> m_OpNames;
		std::vector<InstructionView> m_OpModulesProcessed;

		std::vector<InstructionView> m_Annotations;

		std::vector<InstructionView> m_TypeDeclarations;	// This needs to be unique!

		std::vector<FunctionDeclaration> m_FunctionDeclarations;

		std::vector<FunctionDefinition> m_FunctionDefinitions;

		uint32_t m_UniqueIdentifier = 1;
	};
}