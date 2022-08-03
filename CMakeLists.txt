# Copyright (c) 2022 Dhiraj Wishal
# Main build script for Shader Builder.

# Set the minimum required CMake version.
cmake_minimum_required(VERSION 3.22.2)

# Set the basic project description.
project(
	ShaderBuilder
	VERSION 1.0.0 
	DESCRIPTION "C++ based SPIR-V generator."
)

# Lets tell CMake to add the default ALL_BUILD, ZERO_CHECK and INSTALL to a group.
# This way we can make things much more simpler for Visual Studio.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(PREDEFINED_TARGETS_FOLDER "PredefinedTargets")

# Find the python package.
find_package(Python COMPONENTS Interpreter Development)

# Execute the SPIR-V Tools initialization process.
execute_process(
	COMMAND ${Python_EXECUTABLE} "utils/git-sync-deps"
	WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/SPIRV-Tools"
	COMMAND_ECHO STDOUT
	ERROR_VARIABLE SPIRV_TOOLS_INIT_STATUS
)

# If there was an error with the initialization process, print it out.
if (NOT SPIRV_TOOLS_INIT_STATUS)
	# Set the SPIR-V Tools include, library and binary data.
	set(SPIRV_TOOLS_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/SPIRV-Tools/include)

	# Add the SPIR-V Tools library as a subdirectory.
	add_subdirectory(ThirdParty/SPIRV-Tools)

	# Set the SPIR-V Cross include, library and binary data.
	set(SPIRV_CROSS_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/SPIRV-Cross)

	# Add the SPIR-V Cross library as a subdirectory.
	set(CACHE SPIRV_CROSS_STATIC ON)
	set(CACHE SPIRV_CROSS_ENABLE_CPP ON)
	set(CACHE SPIRV_CROSS_ENABLE_GLSL ON)
	set(CACHE SPIRV_CROSS_ENABLE_HLSL ON)
	add_subdirectory(ThirdParty/SPIRV-Cross)

	# Set the xxHash header include directory.
	set(XXHASH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/xxHash)

	# Set the json header include directory.
	set(JSON_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/json/include)

	# Set the central include directory.
	include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Include)

	# Add the subdirectories.
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Source)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Examples)

	# Set the startup project for Visual Studio and set multi processor compilation for other projects that we build.
	if (MSVC) 
		set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ShaderBuilder)
		target_compile_options(SPIRV-Tools-opt PRIVATE "/MP")	
		target_compile_options(spirv-cross-c PRIVATE "/MP")	
	endif ()
else()
	message(FATAL_ERROR "SPIR-V Tools Initialization failed with ${SPIRV_TOOLS_INIT_STATUS}")
endif()