# Copyright (c) 2022 Dhiraj Wishal

# Set the basic project information.
project(
	ShaderBuilder
	VERSION 1.0.0
	DESCRIPTION "Minte library"
)

# Add the library.
add_library(
	ShaderBuilder
	STATIC

	"${CMAKE_SOURCE_DIR}/Include/ShaderBuilder/Types/DataType.hpp"
	"${CMAKE_SOURCE_DIR}/Include/ShaderBuilder/Types/Vec2.hpp"
	"${CMAKE_SOURCE_DIR}/Include/ShaderBuilder/Types/Vec3.hpp"
	"${CMAKE_SOURCE_DIR}/Include/ShaderBuilder/Types/Vec4.hpp"
	"${CMAKE_SOURCE_DIR}/Include/ShaderBuilder/Builder.hpp"

	"Types/DataType.cpp"
	"Types/Vec2.cpp"
	"Types/Vec3.cpp"
	"Types/Vec4.cpp"
	
	"Builder.cpp" 
	
)

# Make sure to specify the C++ standard to C++20.
set_property(TARGET ShaderBuilder PROPERTY CXX_STANDARD 20)

# If we are on MSVC, we can use the Multi Processor Compilation option.
if (MSVC)
	target_compile_options(ShaderBuilder PRIVATE "/MP")	
endif ()