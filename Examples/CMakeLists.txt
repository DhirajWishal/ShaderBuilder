# Copyright (c) 2022 Dhiraj Wishal

# Set the basic project information.
project(
	ShaderBuilderExamples
	VERSION 1.0.0
	DESCRIPTION "Examples application."
)

# Add the executable.
add_executable(
	ShaderBuilderExamples

	"Main.cpp"
)

# Add the optick static library as a target link.
target_link_libraries(ShaderBuilderExamples ShaderBuilder)

# Make sure to specify the C++ standard to C++20.
set_property(TARGET ShaderBuilderExamples PROPERTY CXX_STANDARD 20)

# If we are on MSVC, we can use the Multi Processor Compilation option.
if (MSVC)
	target_compile_options(ShaderBuilderExamples PRIVATE "/MP")	
endif ()