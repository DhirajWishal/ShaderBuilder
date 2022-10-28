// Copyright (c) 2022 Dhiraj Wishal

#pragma once

#include "Builder.hpp"
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
		 *
		 * @tparam Attributes The input and output attribute types.
		 * @param function The entry point function.
		 * @param attributes The names of the input and output attributes.
		 */
		template<class... Attributes>
		void addEntryPoint(const FunctionBuilder& function, Attributes&&... attributes)
		{
			// Setup the inputs.
			std::string attributeString;
			auto insertAttribute = [&attributeString](auto&& attribute) { attributeString += fmt::format(" %{}", std::move(attribute)); };
			(insertAttribute(std::move(attributes)), ...);

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