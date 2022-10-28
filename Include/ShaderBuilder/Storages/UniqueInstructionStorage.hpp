// Copyright (c) 2022 Dhiraj Wishal

#pragma once

#include "InstructionStorage.hpp"

#include "../Utilities.hpp"

#include <algorithm>

namespace ShaderBuilder
{
	/**
	 * Unique instruction storage class.
	 * This container uniquely stores instructions.
	 */
	class UniqueInstructionStorage final : public InstructionStorage
	{
	public:
		/**
		 * Default constructor.
		 */
		UniqueInstructionStorage() = default;

		/**
		 * Default destructor.
		 */
		~UniqueInstructionStorage() override = default;

		/**
		 * Insert a new instruction.
		 * Note that this will check if the instruction is unique before storing it.
		 *
		 * @param instruction The type instruction.
		 */
		void insert(std::string&& instruction) override
		{
			if (m_ShouldRecord && registerInstruction(GenerateHash(instruction.data(), instruction.size())))
				m_Instructions.emplace_back(std::move(instruction));
		}

	private:
		/**
		 * Register a new instruction.
		 *
		 * @param hash The hash to register.
		 * @return true if the hash was registered now (meaning that it was not available in the storage), false if the hash was not registered now (meaning that it was available).
		 */
		[[nodiscard]] bool registerInstruction(uint64_t hash)
		{
			// Get the iterator where the hash is saved (if it's saved ofc).
			auto itr = std::lower_bound(m_Availability.begin(), m_Availability.end(), hash);

			// Check if it's available or not.
			if (itr == m_Availability.end() || *itr != hash)
			{
				// If it's not, then save it and return true.
				m_Availability.emplace(itr, hash);
				return true;
			}

			// Else return false.
			return false;
		}

	private:
		std::vector<uint64_t> m_Availability;
	};
} // namespace ShaderBuilder