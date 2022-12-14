// Copyright (c) 2022 Dhiraj Wishal

#pragma once

#include "Storages/UniqueInstructionStorage.hpp"

#include <stack>

namespace ShaderBuilder
{
	/**
	 * Function block structure.
	 * This stores information about a single function.
	 */
	struct FunctionBlock final
	{
		/**
		 * Enable the function block's instruction recording.
		 */
		void enableRecording();

		/**
		 * Disable the function block's instruction recording.
		 */
		void disableRecording();

	public:
		InstructionStorage m_Definition;
		InstructionStorage m_Parameters;
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
		 * Create a new function block and push it to the stack.
		 *
		 * @return The created block reference.
		 */
		FunctionBlock& pushFunctionBlock();

		/**
		 * Get current function block.
		 *
		 * @return The current function block.
		 */
		[[nodiscard]] FunctionBlock& getCurrentFunctionBlock();

		/**
		 * This will pop the top of the function block stack and will place it in the finished queue.
		 */
		[[nodiscard]] void finishFunctionBlock();

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
		constexpr void registerType()
		{
			// Try and register value types if the Type is complex.
			if constexpr (IsCompexType<Type>)
				registerType<typename TypeTraits<Type>::ValueTraits::Type>();

			insertType(fmt::format(FMT_STRING("{} = {}"), TypeTraits<Type>::Identifier, TypeTraits<Type>::Declaration));
		}

		/**
		 * Register multiple types function.
		 *
		 * @tparam Type The type to register.
		 * @tparam Types The rest of the types.
		 */
		template<class Type, class... Types>
		constexpr void registerTypes()
		{
			// Try and register the type.
			registerType<Type>();

			// Register the rest if available.
			if constexpr (sizeof...(Types) > 0)
				registerTypes<Types...>();
		}

		/**
		 * Store a constant to the storage.
		 * The identifier will be const_<type identifier>_<value as a integer>.
		 *
		 * @tparam Type The type of the value.
		 * @param value The constant value.
		 */
		template<class Type>
		constexpr void storeConstant(const Type& value)
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
		constexpr void registerArray()
		{
			// Try and register value types if the Type is complex.
			if constexpr (IsCompexType<ValueType>)
				registerType<typename TypeTraits<ValueType>::ValueTraits::Type>();

			storeConstant<uint32_t>(Size);
			insertType(fmt::format("%array_{}_{} = OpTypeArray {} %{}", TypeTraits<ValueType>::RawIdentifier, Size, TypeTraits<ValueType>::Identifier, GetConstantIdentifier<uint32_t>(Size)));
		}

		/**
		 * Get type identifier.
		 *
		 * @tparam Type The type to get the identifier of.
		 * @return The identifier.
		 */
		template<class Type>
		[[nodiscard]] constexpr std::string getTypeIdentifier()
		{
			registerType<Type>();
			return fmt::format(FMT_STRING("{} "), TypeTraits<Type>::Identifier);
		}

		/**
		 * Get multiple type identifiers.
		 *
		 * @tparam Type The type to get the identifier of.
		 * @tparam Types The rest of the types.
		 * @return The identifier.
		 */
		template<class Type, class... Types>
		[[nodiscard]] constexpr std::string getTypeIdentifiers()
		{
			registerType<Type>();
			if constexpr (sizeof...(Types) > 0)
				return fmt::format("{} {}", TypeTraits<Type>::Identifier, getTypeIdentifiers<Types...>());

			else
				return fmt::format(FMT_STRING("{} "), TypeTraits<Type>::Identifier);
		}

		/**
		 * Get the parameter identifier of multiple parameter types.
		 *
		 * @tparam Type The parameter type.
		 * @tparam Types The rest of the parameters
		 * @return The type string.
		 */
		template<class Type, class... Types>
		[[nodiscard]] constexpr std::string getParameterIdentifier()
		{
			registerType<Type>();
			if constexpr (sizeof...(Types) > 0)
				return fmt::format("{}_{}", TypeTraits<Type>::RawIdentifier, getParameterIdentifier<Types...>());

			else
				return TypeTraits<Type>::RawIdentifier;
		}

		/**
		 * Get a function's identifier using the value type.
		 *
		 * @tparam Return The return type.
		 * @tparam Parameters The parameter types.
		 * @return The identifier string.
		 */
		template<class Return, class... Parameters>
		[[nodiscard]] constexpr std::string getFunctionIdentifier()
		{
			if constexpr (sizeof...(Parameters) > 0)
				return fmt::format("{}_{}_callable", TypeTraits<Return>::Identifier, getParameterIdentifier<Parameters...>());

			else
				return fmt::format(FMT_STRING("{}_callable"), TypeTraits<Return>::Identifier);
		}

		/**
		 * Register a function callback type.
		 *
		 * @tparam Type The callback type.
		 */
		template<class Return, class... Parameters>
		constexpr void registerCallable()
		{
			using ReturnType = typename TypeTraits<Return>::Type;

			// Try and register value types.
			registerType<ReturnType>();

			std::string parameterTypes;
			if constexpr (sizeof...(Parameters) > 0)
				parameterTypes = getTypeIdentifiers<Parameters...>();

			insertType(fmt::format("{} = OpTypeFunction {} {}", getFunctionIdentifier<ReturnType, Parameters...>(), TypeTraits<ReturnType>::Identifier, parameterTypes));
		}

		/**
		 * Resolve the member variable type identifiers to register.
		 *
		 * @tparam First The first type.
		 * @tparam Rest The rest of the types.
		 */
		template<class First, class... Rest>
		[[nodiscard]] constexpr std::string resolveMemberVariableTypeIdentifiers()
		{
			using MemberType = typename MemberVariableType<First>::Type;
			registerType<MemberType>();

			if constexpr (sizeof...(Rest) > 0)
				return fmt::format("{} {}", TypeTraits<MemberType>::Identifier, resolveMemberVariableTypeIdentifiers<Rest...>());

			else
				return TypeTraits<MemberType>::Identifier;
		}

	private:
		std::stack<FunctionBlock> m_FunctionBlockStack;
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