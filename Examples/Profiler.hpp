// Copyright (c) 2022 Dhiraj Wishal

#pragma once

#include <chrono>

/**
 * Profiler class.
 * This object prints out the time taken from it's creation till it's destruction.
 */
class Profiler final
{
	using Clock = std::chrono::high_resolution_clock;
	using TimePoint = std::chrono::time_point<Clock>;

public:
	/**
	 * Default constructor.
	 */
	Profiler();

	/**
	 * Default destructor.
	 */
	~Profiler();

private:
	TimePoint m_Begin;
};