// Copyright (c) 2022 Dhiraj Wishal

#pragma once

#include <string>
#include <vector>

namespace ShaderBuilder
{
	/**
	 * Instruction storage class.
	 * This is the base class for all the instruction storages.
	 */
	class InstructionStorage
	{
	public:
		/**
		 * Default constructor.
		 */
		InstructionStorage() = default;

		/**
		 * Default virtual destructor.
		 */
		virtual ~InstructionStorage() = default;

		/**
		 * Insert a new instruction to the storage.
		 *
		 * @param instruction The instruction to be stored.
		 */
		virtual void insert(std::string&& instruction) { m_Instructions.emplace_back(std::move(instruction)); }

		/**
		 * Get the begin iterator.
		 *
		 * @return The iterator.
		 */
		[[nodiscard]] decltype(auto) begin() { return m_Instructions.begin(); }

		/**
		 * Get the end iterator.
		 *
		 * @return The iterator.
		 */
		[[nodiscard]] decltype(auto) end() { return m_Instructions.end(); }

		/**
		 * Get the begin iterator.
		 *
		 * @return The iterator.
		 */
		[[nodiscard]] decltype(auto) begin() const { return m_Instructions.begin(); }

		/**
		 * Get the end iterator.
		 *
		 * @return The iterator.
		 */
		[[nodiscard]] decltype(auto) end() const { return m_Instructions.end(); }

	protected:
		std::vector<std::string> m_Instructions;
	};
} // namespace ShaderBuilder