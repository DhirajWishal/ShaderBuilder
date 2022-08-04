// Copyright (c) 2022 Dhiraj Wishal

#include "Profiler.hpp"

#include <iostream>

Profiler::Profiler()
	: m_Begin(Clock::now())
{
}

Profiler::~Profiler()
{
	std::cout << "Time elapsed: " << Clock::now() - m_Begin << std::endl;
}
