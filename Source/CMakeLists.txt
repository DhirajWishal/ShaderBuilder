# Copyright (c) 2022 Dhiraj Wishal

# Set the basic project information.
project(
	ShaderBuilder
	VERSION 1.0.0
	DESCRIPTION "Shader Builder library"
)

# Add the library.
add_library(
	ShaderBuilder
	STATIC

	"${CMAKE_SOURCE_DIR}/Include/ShaderBuilder/SPIRVSource.hpp"

	"SPIRVSource.cpp"
)

# Add the target includes.
target_include_directories(
	ShaderBuilder 

	PRIVATE ${SPIRV_TOOLS_INCLUDE_DIR}
	PRIVATE ${SPIRV_CROSS_INCLUDE_DIR}
	PRIVATE ${XXHASH_INCLUDE_DIR}
	PUBLIC ${SPIRV_HEADERS_INCLUDE_DIR}
)

# Add the target links.
target_link_libraries(ShaderBuilder SPIRV-Tools-opt spirv-cross-c)

# Make sure to specify the C++ standard to C++20.
set_property(TARGET ShaderBuilder PROPERTY CXX_STANDARD 20)

# If we are on MSVC, we can use the Multi Processor Compilation option.
if (MSVC)
	target_compile_options(ShaderBuilder PRIVATE "/MP")	
endif ()