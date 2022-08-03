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

	"${CMAKE_SOURCE_DIR}/Include/ShaderBuilder/DataType.hpp"
	"${CMAKE_SOURCE_DIR}/Include/ShaderBuilder/Vec2.hpp"
	"${CMAKE_SOURCE_DIR}/Include/ShaderBuilder/Vec3.hpp"
	"${CMAKE_SOURCE_DIR}/Include/ShaderBuilder/Vec4.hpp"
	"${CMAKE_SOURCE_DIR}/Include/ShaderBuilder/Builder.hpp"
	"${CMAKE_SOURCE_DIR}/Include/ShaderBuilder/BuilderError.hpp"
	"${CMAKE_SOURCE_DIR}/Include/ShaderBuilder/SPIRVBinary.hpp"
	"${CMAKE_SOURCE_DIR}/Include/ShaderBuilder/SPIRVSource.hpp"
	"${CMAKE_SOURCE_DIR}/Include/ShaderBuilder/Utilities.hpp"
	"${CMAKE_SOURCE_DIR}/Include/ShaderBuilder/FunctionBuilder.hpp"
	
	"Builder.cpp" 
	"SPIRVBinary.cpp"
	"SPIRVSource.cpp"
	"Utilities.cpp"
	"FunctionBuilder.cpp"
)

# Add the target includes.
target_include_directories(
	ShaderBuilder 

	PRIVATE ${SPIRV_TOOLS_INCLUDE_DIR}
	PRIVATE ${SPIRV_CROSS_INCLUDE_DIR}
	PRIVATE ${XXHASH_INCLUDE_DIR}
	PUBLIC ${JSON_INCLUDE_DIR}
)

# Add the target links.
target_link_libraries(ShaderBuilder SPIRV-Tools-opt spirv-cross-c)

# Make sure to specify the C++ standard to C++20.
set_property(TARGET ShaderBuilder PROPERTY CXX_STANDARD 20)

# If we are on MSVC, we can use the Multi Processor Compilation option.
if (MSVC)
	target_compile_options(ShaderBuilder PRIVATE "/MP")	
endif ()