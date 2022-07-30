// Copyright (c) 2022 Dhiraj Wishal

#pragma once

#include "DataType.hpp"

namespace ShaderBuilder
{
	/**
	 * Uniform class.
	 * This class contains information about a single uniform.
	 */
	class Uniform : public DataType<Uniform>
	{
	public:
		/**
		 * Explicit constructor.
		 */
		template<class... Members>
		explicit Uniform(std::string&& variableName, Members... memberVariables)
		{
			auto callback = [this](auto member) { m_MemberDeclarations.emplace_back(TypeTraits<decltype(member)>::Declaration); };
			(callback(memberVariables), ...);
		}

	private:
		std::vector<std::string> m_MemberDeclarations;
		std:::string m_UniformDeclaration;
	};
} // namespace ShaderBuilder