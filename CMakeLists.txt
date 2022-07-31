# Copyright (c) 2022 Dhiraj Wishal
# Main build script for Minte.

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

# Execute the SPIR-V Tools initialization process.
execute_process(COMMAND "cd ThirdParty/SPIRV-Tools && python3 utils/git-sync-deps")

# Set the SPIR-V Tools include, library and binary data.
set(SPIRV_Tools_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/SPIRV-Tools/include)

# Add the SPIR-V Tools library as a subdirectory.
add_subdirectory(ThirdParty/SPIRV-Tools)

# Set the SPIR-V Cross include, library and binary data.
set(SPIRV_Cross_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/SPIRV-Cross)

# Add the SPIR-V Cross library as a subdirectory.
set(CACHE SPIRV_CROSS_STATIC ON)
set(CACHE SPIRV_CROSS_ENABLE_CPP ON)
set(CACHE SPIRV_CROSS_ENABLE_GLSL ON)
set(CACHE SPIRV_CROSS_ENABLE_HLSL ON)
add_subdirectory(ThirdParty/SPIRV-Cross)

# Set the central include directory.
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Include)

# Add the subdirectories.
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Source)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Tests)

# Set the startup project for Visual Studio and set multi processor compilation for other projects that we build.
if (MSVC) 
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ShaderBuilder)
endif ()