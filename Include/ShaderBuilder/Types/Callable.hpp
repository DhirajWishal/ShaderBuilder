// Copyright (c) 2022 Dhiraj Wishal
#pragma once

#include "DataType.hpp"

namespace ShaderBuilder
{
	/**
	 * Callable class.
	 * This class contains the return type and other information about a function.
	 */
	template<class Return, class... Arguments>
	class Callable final : public DataType
	{
	public:
		/**
		 * Explicit constructor.
		 *
		 * @param typeName The type name of the return type.
		 * @param name The name of the function.
		 * @param codeStream The code stream to store the information to.
		 * @param arguments The parameter types.
		 */
		explicit Callable(std::string_view typeName, std::string&& name, CodeStream& codeStream, Arguments&&... arguments)
			: DataType(typeName, std::move(name), codeStream)
		{
			if constexpr (sizeof...(Arguments) > 0)
				m_CodeStream << "(" << getParameterNames(std::forward<Arguments>(arguments)...) << ")";
			else
				m_CodeStream << "()";
		}

		/**
		 * Function call operator overload.
		 *
		 * @param args The function arguments.
		 * @return The return of the function call.
		 */
		decltype(auto) operator()(Arguments&&... args)
		{
			if constexpr (sizeof...(Arguments) > 0)
				m_CodeStream << m_VariableName << "(" << getArgumentNames(std::forward<Arguments>(args)...) << ");" << std::endl;
			else
				m_CodeStream << m_VariableName << "();" << std::endl;
		}

	private:
		/**
		 * Get the argument names.
		 *
		 * @param first The first argument.
		 * @param rest The rest of the arguments.
		 * @return The full list of arguments.
		 */
		template<class First, class... Rest>
		[[nodsicard]] std::string getArgumentNames(First&& first, Rest&&... rest)
		{
			if constexpr (sizeof...(Rest) > 0)
				return first.getName() + ", " + getArgumentNames(std::forward<Rest>(rest)...);

			else
				return first.getName();
		}

		/**
		 * Get the parameter names.
		 *
		 * @param first The first parameter.
		 * @param rest The rest of the parameters.
		 * @return The full list of parameters.
		 */
		template<class First, class... Rest>
		[[nodsicard]] std::string getParameterNames(First&& first, Rest&&... rest)
		{
			if constexpr (sizeof...(Rest) > 0)
				return std::string(first.getTypeName().data()) + " " + first.getName() + ", " + getParameterNames(std::forward<Rest>(rest)...);

			else
				return std::string(first.getTypeName().data()) + " " + first.getName();
		}
	};
} // namespace ShaderBuilder