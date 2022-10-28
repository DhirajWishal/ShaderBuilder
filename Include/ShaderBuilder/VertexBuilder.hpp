// Copyright (c) 2022 Dhiraj Wishal

#pragma once

#include "Builder.hpp"
#include "BuilderError.hpp"
#include "Vec4.hpp"

namespace ShaderBuilder
{
	/**
	 * Vertex builder class.
	 * This contains the default vertex shader outputs and other information.
	 */
	class VertexBuilder final : public Builder
	{
		/**
		 * Built in structure.
		 */
		struct BuiltIn final : public DataType<BuiltIn>
		{
			Vec4<float> m_Position;
			float m_PointSize;
			std::array<float, 1> m_ClipDistance;
			std::array<float, 1> m_CullDistance;
		};

	public:
		/**
		 * Default constructor.
		 */
		VertexBuilder();

		/**
		 * Add an entry point to the shader.
		 * Note that this will run the entry point function once for convenience.
		 *
		 * @tparam FunctionType The function type.
		 * @tparam Attributes The input and output attribute types.
		 * @param function The entry point function. Make sure that the return type is void and there are no parameters.
		 * @param attributes The names of the input and output attributes.
		 */
		template<class FunctionType, class... Attributes>
		void addEntryPoint(FunctionType& function, const Attributes&... attributes)
		{
			// Validate the entry point and run it once.
			if constexpr (FunctionType::ParameterCount > 0 || !std::is_void_v<typename FunctionType::ReturnType>)
				throw BuilderError("Entry points should not have any parameters and the return type should be void!");

			else
				function();

			// Setup the inputs.
			std::string attributeString;
			auto insertAttribute = [&attributeString](const auto& attribute) { attributeString += fmt::format(" %{}", attribute.getName()); };
			(insertAttribute(attributes), ...);

			const auto& name = function.getName();
			m_Source.insertEntryPoint(fmt::format("OpEntryPoint Vertex %{} \"{}\" %perVertex {}", name, name, attributeString));
		}

		/**
		 * Set the position value.
		 *
		 * @param value The value to set.
		 */
		void setPoisition(const Vec4<float>& value);
	};
} // namespace ShaderBuilder