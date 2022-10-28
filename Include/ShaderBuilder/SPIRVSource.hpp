// Copyright (c) 2022 Dhiraj Wishal

#pragma once

#include "Storages/UniqueInstructionStorage.hpp"

namespace ShaderBuilder
{
	/**
	 * Function block structure.
	 * This stores information about a single function.
	 */
	struct FunctionBlock final
	{
		InstructionStorage m_Definition;
		InstructionStorage m_Instructions;

		UniqueInstructionStorage m_Variables;

		std::string m_Name;
	};

	/**
	 * SPIR-V Source class.
	 * This contains all the source information provided by the data types and others.
	 */
	class SPIRVSource final
	{
	public:
		/**
		 * Default constructor.
		 */
		SPIRVSource() = default;

		/**
		 * Insert a new shader capability.
		 *
		 * @param instruction The shader capability.
		 */
		void insertCapability(std::string&& instruction);

		/**
		 * Insert a new extension.
		 *
		 * @param instruction The extension to insert.
		 */
		void insertExtension(std::string&& instruction);

		/**
		 * Insert a new extended instruction set.
		 *
		 * @param instruction The instruction.
		 */
		void insertExtendedInstructionSet(std::string&& instruction);

		/**
		 * Set the memory model.
		 *
		 * @param addressingModel The addressing model to set.
		 * @param memoryModel The memory model to set.
		 */
		void setMemoryModel(std::string&& instruction);

		/**
		 * Insert an entry point.
		 *
		 * @param instruction The instruction.
		 */
		void insertEntryPoint(std::string&& instruction);

		/**
		 * Insert a new execution mode.
		 *
		 * @param instruction The instruction.
		 */
		void insertExecutionMode(std::string&& instruction);

		/**
		 * Insert a new debug name.
		 *
		 * @param instruction The instruction.
		 */
		void insertName(std::string&& instruction);

		/**
		 * Insert a new annotation.
		 *
		 * @param instruction The instruction.
		 */
		void insertAnnotation(std::string&& instruction);

		/**
		 * Insert a new type.
		 *
		 * @param instruction The instruction.
		 */
		void insertType(std::string&& instruction);

		/**
		 * Insert a new instruction.
		 * This instruction will be stored in the function definitions.
		 */
		void insertInstruction(std::string&& instruction);

		/**
		 * Create a new function block.
		 *
		 * @return The created block reference.
		 */
		[[nodiscard]] FunctionBlock& createFunctionBlock();

		/**
		 * Get current function block.
		 *
		 * @return The current function block.
		 */
		[[nodiscard]] FunctionBlock& getCurrentFunctionBlock();

	public:
		/**
		 * Get the source assembly.
		 *
		 * @return The assembly string
		 */
		[[nodiscard]] std::string getSourceAssembly() const;

		/**
		 * Get a unique ID.
		 *
		 * @return The unique ID.
		 */
		[[nodiscard]] uint64_t getUniqueID() { return m_UniqueID++; }

		/**
		 * Get a unique identifier.
		 * The difference between this and the getUniqueID is that this returns a string while the other returns the integer.
		 *
		 * @return The unique identifier.
		 */
		[[nodiscard]] std::string getUniqueIdentifier() { return std::to_string(m_UniqueID++); }

	public:
		/**
		 * Register type function.
		 *
		 * @tparam Type The type to register.
		 */
		template<class Type>
		void registerType()
		{
			// Try and register value types if the Type is complex.
			if constexpr (IsCompexType<Type>)
				registerType<typename TypeTraits<Type>::ValueTraits::Type>();

			insertType(fmt::format("{} = {}", TypeTraits<Type>::Identifier, TypeTraits<Type>::Declaration));
		}

		/**
		 * Store a constant to the storage.
		 * The identifier will be const_<type identifier>_<value as a integer>.
		 *
		 * @tparam Type The type of the value.
		 * @param value The constant value.
		 */
		template<class Type>
		void storeConstant(const Type& value)
		{
			registerType<Type>();
			insertType(fmt::format("%{} = OpConstant {} {}", GetConstantIdentifier(value), TypeTraits<Type>::Identifier, value));
		}

		/**
		 * Register an array function.
		 *
		 * @tparam ValueType The value type of the array.
		 * @tparam Size The size of the array.
		 */
		template<class ValueType, size_t Size>
		void registerArray()
		{
			// Try and register value types if the Type is complex.
			if constexpr (IsCompexType<ValueType>)
				registerType<typename TypeTraits<ValueType>::ValueTraits::Type>();

			storeConstant<uint32_t>(Size);
			insertType(fmt::format("%array_{}_{} = OpTypeArray {} %{}", TypeTraits<ValueType>::RawIdentifier, Size, TypeTraits<ValueType>::Identifier, GetConstantIdentifier<uint32_t>(Size)));
		}

		/**
		 * Register a function callback type.
		 *
		 * @tparam Type The callback type.
		 */
		template<class Type>
		void registerCallable()
		{
			using ValueType = typename TypeTraits<Type>::ValueTraits::Type;

			// Try and register value types.
			registerType<ValueType>();
			insertType(fmt::format("{} = OpTypeFunction {}", GetFunctionIdentifier<ValueType>(), TypeTraits<ValueType>::Identifier));
		}

		/**
		 * Resolve the member variable type identifiers to register.
		 *
		 * @tparam First The first type.
		 * @tparam Rest The rest of the types.
		 */
		template<class First, class... Rest>
		[[nodiscard]] std::string resolveMemberVariableTypeIdentifiers()
		{
			using MemberType = typename MemberVariableType<First>::Type;
			registerType<MemberType>();

			if constexpr (sizeof...(Rest) > 0)
				return fmt::format("{} {}", TypeTraits<MemberType>::Identifier, resolveMemberVariableTypeIdentifiers<Rest...>());

			else
				return TypeTraits<MemberType>::Identifier;
		}

	private:
		std::vector<FunctionBlock> m_FunctionBlocks;

		InstructionStorage m_Capabilities;
		InstructionStorage m_Extensions;
		InstructionStorage m_ExtendedInstructions;

		std::string m_MemoryModel;

		InstructionStorage m_EntryPoints;
		InstructionStorage m_ExecutionModes;
		InstructionStorage m_DebugNames;
		InstructionStorage m_Annotations;
		UniqueInstructionStorage m_Types;

		InstructionStorage m_FunctionDeclarations;

		uint64_t m_UniqueID = 1;
	};
} // namespace ShaderBuilder