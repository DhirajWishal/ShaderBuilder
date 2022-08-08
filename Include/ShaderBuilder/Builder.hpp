// Copyright (c) 2022 Dhiraj Wishal

#pragma once

#include "SPIRVBinary.hpp"
#include "FunctionBuilder.hpp"

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
	 * Builder class.
	 * This class contains the base code for SPIR-V generation and can be used to
	 * compile to other high level languages, like GLSL and HLSL.
	 */
	class Builder final
	{
	public:
		/**
		 * Explicit constructor.
		 *
		 * @param config The builder's initial configuration.
		 */
		explicit Builder(Configuration config = Configuration());

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
		 * Create a new local variable.
		 * These variables must be created within a function.
		 *
		 * @tparam Type The type of the variable.
		 * @tparam Types The initialization types.
		 * @param name The variable name.
		 * @param initializer The initializer types.
		 * @return The created variable.
		 */
		template<class Type, class... Types>
		[[nodiscard]] Type createLocalVariable(std::string&& name, Types&&... initializer)
		{
			registerType<Type>();
			m_Source.insertType(std::string("%variable_type_") + TypeTraits<Type>::RawIdentifier + " = OpTypePointer Function " + TypeTraits<Type>::Identifier);
			m_Source.insertName("OpName %" + name + " \"" + name + "\"");

			return Type(m_Source, name, std::forward<Types>(initializer)...);
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
			m_Source.insertType("%type_" + name + " = OpTypeStruct " + resolveMemberVariableTypeIdentifiers<Members...>());

			// Setup type declarations.
			m_Source.insertType("%uniform_" + name + " = OpTypePointer Uniform %type_" + name);
			m_Source.insertType("%" + name + " = OpVariable %uniform_" + name + " Uniform");

			// Set the type debug information and annotations.
			m_Source.insertName("OpName %uniform_" + name + " \"" + name + "\"");
			m_Source.insertName("%" + name, "");

			m_Source.insertAnnotation("OpDecorate %" + name + " DescriptorSet " + std::to_string(set));
			m_Source.insertAnnotation("OpDecorate %" + name + " Binding " + std::to_string(binding));

			// Create the uniform.
			auto uniform = Type(m_Source, name);

			uint64_t counter = 0, offsets = 0;
			auto logMemberInformation = [this, &uniform, &name, &counter, &offsets](auto member)
			{
				m_Source.insertName("OpMemberName %type_" + name + " " + std::to_string(counter) " \"" + (uniform.*member).getName() + "\"");
				m_Source.insertAnnotation("OpMemberDecorate %type_" + name + " " + std::to_string(counter) + " Offset " + std::to_string(offsets));

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
		[[nodiscard]] FunctionBuilder& createFunction(std::string&& name)
		{
			registerCallable<Callable<ReturnType>>();
			return m_Source.createFunctionBuilder(std::move(name));
		}

		/**
		 * Add an entry point to the shader.
		 *
		 * @tparam Attributes The input and output attribute types.
		 * @param shaderType The type of the shader.
		 * @param function The entry point function.
		 * @param attributes The names of the input and output attributes.
		 */
		template<class... Attributes>
		void addEntryPoint(ShaderType shaderType, const FunctionBuilder& function, Attributes&&... attributes)
		{
			// Resolve the proper shader type.
			std::string executionModel;
			switch (shaderType)
			{
			case ShaderBuilder::ShaderType::Vertex:
				executionModel = "Vertex ";
				break;

			case ShaderBuilder::ShaderType::TessellationControl:
				executionModel = "TessellationControl ";
				break;

			case ShaderBuilder::ShaderType::TessellationEvaluation:
				executionModel = "TessellationEvaluation ";
				break;

			case ShaderBuilder::ShaderType::Geometry:
				executionModel = "Geometry ";
				break;

			case ShaderBuilder::ShaderType::Fragment:
				executionModel = "Fragment ";
				break;

			case ShaderBuilder::ShaderType::Compute:
				executionModel = "GLCompute ";
				break;
			}

			// Setup the inputs.
			std::string attributeString;
			auto insertAttribute = [&attributeString](auto&& attribute) { attributeString += " %" + std::move(attribute); };
			(insertAttribute(std::move(attributes)), ...);

			const auto& name = function.getName();
			m_Source.insertEntryPoint("OpEntryPoint " + executionModel + " %" + name + " \"" + name + "\"" + attributeString);
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
		 * @param shouldOptimize Whether or not to optimize the generated SPIR-V binary. Default is true.
		 * @return The compiled binary.
		 */
		[[nodiscard]] SPIRVBinary compile(bool shouldOptimize = true) const;

	private:
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

			m_Source.insertType(TypeTraits<Type>::Identifier, TypeTraits<Type>::Declaration);
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

			m_Source.insertType(GetFunctionIdentifier<ValueType>(), std::string("OpTypeFunction ") + TypeTraits<ValueType>::Identifier);
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
				return std::string(TypeTraits<MemberType>::Identifier) + " " + resolveMemberVariableTypeIdentifiers<Rest...>();

			else
				return std::string(TypeTraits<MemberType>::Identifier);
		}

	private:
		SPIRVSource m_Source;
	};
} // namespace ShaderBuilder