// Copyright (c) 2022 Dhiraj Wishal

#pragma once

#include "FunctionBuilder.hpp"

#include <functional>

namespace ShaderBuilder
{
	/**
	 * Function class.
	 * This stores a single user-defined shader function and will be used for both calling and recording instructions.
	 *
	 * @tparam Return The return type.
	 * @tparam Parameters The parameter types.
	 */
	template<class FunctionBuilderType, class Return, class...Parameters>
	class Function final : public DataType<Function<FunctionBuilderType, Return, Parameters...>>
	{
	public:
		using Super = DataType<Function<FunctionBuilderType, Return, Parameters...>>;
		using FunctionType = std::function<Return(FunctionBuilderType&, Parameters...)>;
		using ReturnType = Return;

		static constexpr uint32_t ParameterCount = sizeof...(Parameters);

	public:
		/**
		 * Explicit constructor.
		 */
		explicit Function(SPIRVSource& source, FunctionType&& function) : Super(source, source.getUniqueIdentifier()), m_Builder(source), m_Function(std::move(function))
		{
			Super::m_Source.registerCallable<Return, Parameters...>();
			Super::m_Source.insertName(fmt::format("OpName %{} \"{}\"", Super::m_VariableName, Super::m_VariableName));
		}

		/**
		 * Call the function.
		 */
		Return operator()(Parameters... arguments)
		{
			if (m_Builder.isRecording())
			{
				auto& block = Super::m_Source.getCurrentFunctionBlock();
				block.m_Name = Super::m_VariableName;
				block.m_Definition.insert(fmt::format("%{} = OpFunction {} None {}", Super::m_VariableName, TypeTraits<Return>::Identifier, Super::m_Source.getFunctionIdentifier<Return, Parameters...>()));
			}

			if constexpr (std::is_void_v<Return>)
			{
				m_Function(m_Builder, arguments...);
				m_Builder.exit();
				m_Builder.toggleRecording();
			}

			else
			{
				auto ret = m_Function(m_Builder, arguments...);
				m_Builder.exit(ret);
				m_Builder.toggleRecording();

				return ret;
			}
		}

	private:
		FunctionBuilderType m_Builder;
		FunctionType m_Function;
	};
}