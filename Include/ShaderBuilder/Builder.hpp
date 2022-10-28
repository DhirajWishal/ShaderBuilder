// Copyright (c) 2022 Dhiraj Wishal

#pragma once

#include "SPIRVBinary.hpp"
#include "FunctionBuilder.hpp"
#include "Callable.hpp"

#include "Input.hpp"
#include "Output.hpp"

#include <array>

namespace ShaderBuilder
{
	/**
	 * Shader type enum.
	 */
	enum class ShaderType : uint8_t
	{
		Vertex,
		TessellationControl,
		TessellationEvaluation,
		Geometry,
		Fragment,
		Compute,
	};

	/**
	 * Addressing model enum.
	 */
	enum class AddressingModel : uint8_t
	{
		Logical,
		Physical32,
		Physical64,
		PhysicalStorageBuffer64
	};

	/**
	 * Memory model enum.
	 */
	enum class MemoryModel : uint8_t
	{
		Simple,
		GLSL450,
		OpenCL,
		Vulkan
	};

	/**
	 * Configuration structure.
	 * This contains basic information about the shader code that is being built.
	 */
	struct Configuration final
	{
		AddressingModel m_AddressingModel = AddressingModel::Logical;
		MemoryModel m_MemoryModel = MemoryModel::GLSL450;

	};

	/**
	 * Optimization flags enum.
	 */
	enum class OptimizationFlags : uint8_t
	{
		None = 0,
		FreezeCosntants = 1 << 0,
		UnifyConstants = 1 << 1,
		StripNonSemanticInfo = 1 << 2,
		EliminateDeadFunctions = 1 << 3,
		EliminateDeadMembers = 1 << 4,
		StripDebugInfo = 1 << 5,

		DebugMode = FreezeCosntants | UnifyConstants | StripNonSemanticInfo | EliminateDeadFunctions | EliminateDeadMembers,
		Release = FreezeCosntants | UnifyConstants | StripNonSemanticInfo | EliminateDeadFunctions | EliminateDeadMembers | StripDebugInfo
	};

	[[nodiscard]] constexpr OptimizationFlags operator|(OptimizationFlags lhs, OptimizationFlags rhs) { return static_cast<OptimizationFlags>(static_cast<std::underlying_type_t<OptimizationFlags>>(lhs) | static_cast<std::underlying_type_t<OptimizationFlags>>(rhs)); }
	[[nodiscard]] constexpr bool operator&(OptimizationFlags lhs, OptimizationFlags rhs) { return static_cast<std::underlying_type_t<OptimizationFlags>>(lhs) & static_cast<std::underlying_type_t<OptimizationFlags>>(rhs); }

	/**
	 * Builder class.
	 * This class contains the base code for SPIR-V generation and can be used to
	 * compile to other high level languages, like GLSL and HLSL.
	 */
	class Builder
	{
	public:
		/**
		 * Explicit constructor.
		 *
		 * @param config The builder's initial configuration.
		 */
		explicit Builder(Configuration config = Configuration());

		/**
		 * Pure virtual destructor.
		 */
		virtual ~Builder() = 0;

		/**
		 * Create a new shader input.
		 *
		 * @tparam Type The type of the variable.
		 * @return The created variable.
		 */
		template<class Type>
		[[nodiscard]] Input<Type> createInput(uint32_t location)
		{
			return Input<Type>(m_Source, location);
		}

		/**
		 * Create a new shader output.
		 *
		 * @tparam Type The type of the variable.
		 * @return The created variable.
		 */
		template<class Type>
		[[nodiscard]] Output<Type> createOutput(uint32_t location)
		{
			return Output<Type>(m_Source, location);
		}

		/**
		 * Create a new Uniform.
		 *
		 * Note that members should be pointers to the member variables and should be in the same order they appear in the actual struct.
		 * For example,
		 *
		 * ```c++
		 * struct Camera { Mat4 Projection = Mat4("Projection"); Mat4 View = Mat4("View"); };
		 * builder.createUniform<Camera>(0, 0, "camera", &Camera::Projection, &Camera::View);
		 * ```
		 *
		 * @tparam Type The type of the uniform.
		 * @tparam Members The uniform's members.
		 * @param set The descriptor set index.
		 * @param binding The uniform's binding.
		 * @param name The name of the uniform.
		 * @param members The members of the uniform.
		 * @return The created uniform.
		 */
		template<class Type, class... Members>
		[[nodiscard]] Type createUniform(uint32_t set, uint32_t binding, const std::string& name, Members... members)
		{
			// Register the members.
			m_Source.insertType(fmt::format("%type_{} = OpTypeStruct {}", name, m_Source.resolveMemberVariableTypeIdentifiers<Members...>()));

			// Setup type declarations.
			m_Source.insertType(fmt::format("%uniform_{} = OpTypePointer Uniform %type_{}", name, name));
			m_Source.insertType(fmt::format("%{} = OpVariable %uniform_{} Uniform", name, name));

			// Set the type debug information and annotations.
			m_Source.insertName(fmt::format("OpName %uniform_{} \"{}\"", name, name));
			m_Source.insertName(fmt::format("OpName %{} \"\"", name));

			m_Source.insertAnnotation(fmt::format("OpDecorate %{} DescriptorSet {}", name, set));
			m_Source.insertAnnotation(fmt::format("OpDecorate %{} Binding {}", name, binding));

			// Create the uniform.
			auto uniform = Type(m_Source, name);

			uint64_t counter = 0, offsets = 0;
			auto logMemberInformation = [this, &uniform, &name, &counter, &offsets](auto member)
			{
				m_Source.insertName(fmt::format("OpMemberName %type_{} {} \"{}\"", name, counter, (uniform.*member).getName()));
				m_Source.insertAnnotation(fmt::format("OpMemberDecorate %type_{} {} Offset {}", name, counter, offsets));

				counter++;
				offsets += TypeTraits<typename MemberVariableType<decltype(member)>::Type>::Size;
			};
			(logMemberInformation(members), ...);

			return uniform;
		}

		/**
		 * Create a new function.
		 *
		 * @tparam Type The function type.
		 * @param name The name of the function.
		 * @param function The function definition. Make sure that the function must contain a single parameter of const FunctionBuilder&.
		 * @return The function builder. Note that the reference will be invalidated after another function creation.
		 */
		template<class ReturnType>
		[[nodiscard]] FunctionBuilder createFunction(std::string&& name)
		{
			m_Source.registerCallable<Callable<ReturnType>>();
			return FunctionBuilder(m_Source, std::move(name), FunctionBuilderReturnType<ReturnType>());
		}

	public:
		/**
		 * Get the internal string.
		 *
		 * @return The string.
		 */
		[[nodiscard]] std::string getString() const;

		/**
		 * Get the internal JSON representation.
		 *
		 * @return The JSON representation as a string.
		 */
		[[nodiscard]] std::string getJSON() const;

		/**
		 * Compile the shader code and inform if there were any errors.
		 *
		 * @param flags Optimization flags. Default is Release.
		 * @return The compiled binary.
		 */
		[[nodiscard]] SPIRVBinary compile(OptimizationFlags flags = OptimizationFlags::Release) const;

	protected:
		SPIRVSource m_Source;
	};
} // namespace ShaderBuilder