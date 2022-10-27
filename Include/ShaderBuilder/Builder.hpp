// Copyright (c) 2022 Dhiraj Wishal

#pragma once

#include "SPIRVBinary.hpp"
#include "FunctionBuilder.hpp"
#include "Callable.hpp"

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
		 * @param name The variable name.
		 * @return The created variable.
		 */
		template<class Type>
		[[nodiscard]] Type createInput(uint32_t location, std::string&& name)
		{
			registerType<Type>();
			return Type(location, true, m_Source, name);
		}

		/**
		 * Create a new shader output.
		 *
		 * @tparam Type The type of the variable.
		 * @param name The variable name.
		 * @return The created variable.
		 */
		template<class Type>
		[[nodiscard]] Type createOutput(uint32_t location, std::string&& name)
		{
			registerType<Type>();
			return Type(location, false, m_Source, name);
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
			m_Source.insertType(std::format("%type_{} = OpTypeStruct {}", name, resolveMemberVariableTypeIdentifiers<Members...>()));

			// Setup type declarations.
			m_Source.insertType(std::format("%uniform_{} = OpTypePointer Uniform %type_{}", name, name));
			m_Source.insertType(std::format("%{} = OpVariable %uniform_{} Uniform", name, name));

			// Set the type debug information and annotations.
			m_Source.insertName(std::format("OpName %uniform_{} \"{}\"", name, name));
			m_Source.insertName(std::format("OpName %{} \"\"", name));

			m_Source.insertAnnotation(std::format("OpDecorate %{} DescriptorSet {}", name, set));
			m_Source.insertAnnotation(std::format("OpDecorate %{} Binding {}", name, binding));

			// Create the uniform.
			auto uniform = Type(m_Source, name);

			uint64_t counter = 0, offsets = 0;
			auto logMemberInformation = [this, &uniform, &name, &counter, &offsets](auto member)
			{
				m_Source.insertName(std::format("OpMemberName %type_{} {} \"{}\"", name, counter, (uniform.*member).getName()));
				m_Source.insertAnnotation(std::format("OpMemberDecorate %type_{} {} Offset {}", name, counter, offsets));

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
			registerCallable<Callable<ReturnType>>();
			return FunctionBuilder(m_Source, std::move(name), FunctionBuilderReturnType<ReturnType>());
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
			m_Source.insertType(std::format("%{} = OpConstant {} {}", GetConstantIdentifier(value), TypeTraits<Type>::Identifier, value));
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

			m_Source.insertType(std::format("{} = {}", TypeTraits<Type>::Identifier, TypeTraits<Type>::Declaration));
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
			m_Source.insertType(std::format("%array_{}_{} = OpTypeArray {} %{}", TypeTraits<ValueType>::RawIdentifier, Size, TypeTraits<ValueType>::Identifier, GetConstantIdentifier<uint32_t>(Size)));
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
			m_Source.insertType(std::format("{} = OpTypeFunction {}", GetFunctionIdentifier<ValueType>(), TypeTraits<ValueType>::Identifier));
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
				return std::format("{} {}", TypeTraits<MemberType>::Identifier, resolveMemberVariableTypeIdentifiers<Rest...>());

			else
				return std::string(TypeTraits<MemberType>::Identifier);
		}

	protected:
		SPIRVSource m_Source;
	};
} // namespace ShaderBuilder