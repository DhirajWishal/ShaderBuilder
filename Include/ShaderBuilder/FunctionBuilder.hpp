// Copyright (c) 2022 Dhiraj Wishal

#pragma once

#include "DataType.hpp"

namespace ShaderBuilder
{
	/**
	 * Function builder return type structure.
	 * This structure is used to deduce the return type of a function builder.
	 *
	 * @tparam Type The return type of the function.
	 */
	template<class Type>
	struct FunctionBuilderReturnType {};

	/**
	 * Function builder class.
	 */
	class FunctionBuilder
	{
	public:
		/**
		 * Explicit constructor.
		 *
		 * @param source The source to record all the commands to.
		 */
		explicit FunctionBuilder(SPIRVSource& source) : m_Source(source) {}

		/**
		 * Default destructor.
		 */
		virtual ~FunctionBuilder();

		/**
		 * Create a new variable.
		 * These variables must be created within a function.
		 *
		 * @tparam Type The type of the variable.
		 * @tparam Types The initialization types.
		 * @param initializer The initializer types.
		 * @return The created variable.
		 */
		template<class Type, class... Types>
		[[nodiscard]] Type createVariable(Types&&... initializer)
		{
			auto identifier = m_Source.getUniqueIdentifier();
			if (m_IsRecording)
			{
				m_Source.registerType<Type>();
				m_Source.insertType(fmt::format(FMT_STRING("%variable_type_{} = OpTypePointer Function {}"), TypeTraits<Type>::RawIdentifier, TypeTraits<Type>::Identifier));

				m_Source.getCurrentFunctionBlock().m_Variables.insert(fmt::format("%{} = OpVariable %variable_type_{} Function", identifier, TypeTraits<Type>::RawIdentifier));
			}

			return Type(m_Source, std::move(identifier), std::forward<Types>(initializer)...);
		}

		/**
		 * Call a function from the builder.
		 *
		 * @tparam FunctionType The function type to call.
		 * @tparam Arguments The argument types.
		 * @param function The function to call.
		 * @param arguments The arguments to pass into the function.
		 * @return The return from the function.
		 */
		template<class FunctionType, class... Arguments>
		decltype(auto) call(FunctionType& function, Arguments&&... arguments)
		{
			const auto returnIdentifier = m_Source.getUniqueIdentifier();
			if constexpr (sizeof...(Arguments) > 0)
			{
				std::string argumentString;
				auto argumentToString = [&argumentString](const auto& argument) mutable
				{
					using ArgumentType = decltype(argument);

					argumentString += fmt::format("%{} ", argument.getName());
				};
				(argumentToString(arguments), ...);

				m_Source.getCurrentFunctionBlock().m_Instructions.insert(fmt::format("%{} = OpFunctionCall {} %{} {}", returnIdentifier, TypeTraits<typename FunctionType::ReturnType>::Identifier, function.getName(), argumentString));
			}
			else
			{
				m_Source.getCurrentFunctionBlock().m_Instructions.insert(fmt::format("%{} = OpFunctionCall {} %{}", returnIdentifier, TypeTraits<typename FunctionType::ReturnType>::Identifier, function.getName()));
			}

			m_Source.pushFunctionBlock();
			return function(std::forward<Arguments>(arguments)...);
		}

		/**
		 * Exit from the function by returning a value.
		 *
		 * @tparam Type The value type.
		 * @param value The value to return.
		 */
		template<class Type>
		void exit(const Type& value)
		{
			if (m_IsRecording)
			{
				// auto& block = m_Source.getCurrentFunctionBlock();
				// block.m_Instructions.insert("OpReturn");
				m_IsComplete = true;
			}
		}

		/**
		 * Exit the function by returning nothing.
		 */
		void exit();

		/**
		 * Toggle the recording to false.
		 * From here on, the builder will not record any instructions but will only create the variables.
		 */
		void toggleRecording();

		/**
		 * Check if the builder is recording or not.
		 *
		 * @return True if the builder is recording.
		 * @return False if the builder is not recording.
		 */
		[[nodiscard]] bool isRecording() const { return m_IsRecording; }

	protected:
		SPIRVSource& m_Source;

		bool m_IsComplete = false;
		bool m_IsRecording = true;
	};
} // namespace ShaderBuilder