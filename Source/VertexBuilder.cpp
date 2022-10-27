// Copyright (c) 2022 Dhiraj Wishal

#include "ShaderBuilder/VertexBuilder.hpp"

namespace ShaderBuilder
{
	VertexBuilder::VertexBuilder()
	{
		// Setup the annotations.
		m_Source.insertAnnotation("OpMemberDecorate %gl_PerVertex 0 BuiltIn Position");
		m_Source.insertAnnotation("OpMemberDecorate %gl_PerVertex 1 BuiltIn PointSize");
		m_Source.insertAnnotation("OpMemberDecorate %gl_PerVertex 2 BuiltIn ClipDistance");
		m_Source.insertAnnotation("OpMemberDecorate %gl_PerVertex 3 BuiltIn CullDistance");
		m_Source.insertAnnotation("OpDecorate %gl_PerVertex Block");

		// Setup the names.
		m_Source.insertName("OpName %gl_PerVertex \"gl_PerVertex\"");
		m_Source.insertName("OpMemberName %gl_PerVertex 0 \"gl_Position\"");
		m_Source.insertName("OpMemberName %gl_PerVertex 1 \"gl_PointSize\"");
		m_Source.insertName("OpMemberName %gl_PerVertex 2 \"gl_ClipDistance\"");
		m_Source.insertName("OpMemberName %gl_PerVertex 3 \"gl_CullDistance\"");

		// Register the types.
		registerType<Vec4<float>>();
		registerType<float>();
		registerType<int>();
		registerArray<float, 1>();

		// Setup the types.
		m_Source.insertType("%gl_PerVertex = OpTypeStruct %vec4_float %float %array_float_1 %array_float_1");
		m_Source.insertType("%pointer_gl_PerVertex = OpTypePointer Output %gl_PerVertex");
		m_Source.insertType("%perVertex = OpVariable %pointer_gl_PerVertex Output");
		m_Source.insertType("%type_gl_Position = OpTypePointer Output %vec4_float");

		storeConstant(0);
		storeConstant(1);
	}

	void VertexBuilder::setPoisition(const Vec4<float>& value)
	{
		const auto valuePointer = std::format("%{}", m_Source.getUniqueIdentifier());
		m_Source.getCurrentFunctionBlock().m_Instructions.insert(std::format("{} = OpLoad %vec4_float %{}", valuePointer, value.getName()));

		const auto positionPointer = std::format("%{}", m_Source.getUniqueIdentifier());
		m_Source.getCurrentFunctionBlock().m_Instructions.insert(std::format("{} = OpAccessChain %type_gl_Position %perVertex %{}", positionPointer, GetConstantIdentifier(0)));
		m_Source.getCurrentFunctionBlock().m_Instructions.insert(std::format("OpStore {} {}", positionPointer, valuePointer));
	}
} // namespace ShaderBuilder