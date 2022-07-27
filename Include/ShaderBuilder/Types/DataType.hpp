// Copyright (c) 2022 Dhiraj Wishal
#pragma once

#include <string>
#include <sstream>
#include <cstdint>

namespace ShaderBuilder
{
	using CodeStream = std::stringstream;

	/**
	 * Meta information enum.
	 * This contains additional information regarding a type.
	 */
	enum class MetaInformation : uint8_t
	{
		Variable = 1 << 0,
		ParameterType = 1 << 1,
		Pointer = 1 << 2,
		Reference = 1 << 3,
		BuiltIn = 1 << 4
	};

	/**
	 * Convert an enum to integer.
	 *
	 * @tparam Type The type of the enum.
	 * @param value The value to convert.
	 * @return The integer value it holds.
	 */
	template<class Type>
	[[nodiscard]] constexpr std::underlying_type_t<Type> EnumToInt(const Type value) noexcept
	{
		return static_cast<std::underlying_type_t<Type>>(value);
	}

	/**
	 * Convert an integer to an enum.
	 *
	 * @tparam Type The type of the data.
	 * @param value The value to convert.
	 * @return The converted enum.
	 */
	template<class Type>
	[[nodiscard]] constexpr Type IntToEnum(const std::underlying_type_t<Type> value) noexcept
	{
		return static_cast<Type>(value);
	}

	constexpr MetaInformation operator|(const MetaInformation& lhs, const MetaInformation& rhs) { return static_cast<MetaInformation>(EnumToInt(lhs) | EnumToInt(rhs)); }
	constexpr bool operator&(const MetaInformation& lhs, const MetaInformation& rhs) { return EnumToInt(lhs) & EnumToInt(rhs); }

	/**
	 * Data type class.
	 * This class is the base class for all the supported types.
	 */
	class DataType
	{
	public:
		/**
		 * Explicit constructor.
		 *
		 * @param typeName The type name of the data type.
		 * @param variableName The name of the variable.
		 * @param codeStream The code stream to register the code to.
		 * @param meta The meta information regarding the type.
		 */
		explicit DataType(std::string_view typeName, std::string&& variableName, CodeStream& codeStream, MetaInformation meta = MetaInformation::Variable);

		/**
		 * Get the type name of the variable/ function.
		 *
		 * @return The type name.
		 */
		[[nodsicard]] std::string_view getTypeName() const { return m_TypeName; }

		/**
		 * Get the name of the variable/ function.
		 *
		 * @return The name.
		 */
		[[nodsicard]] std::string getName() const { return m_VariableName; }

	protected:
		std::string_view m_TypeName;
		std::string m_VariableName;
		CodeStream& m_CodeStream;

		MetaInformation m_MetaInformation;
	};
} // namespace ShaderBuilder
