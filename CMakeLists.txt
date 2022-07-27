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

# Set the central include directory.
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Include)

# Add the subdirectories.
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Source)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Tests)

# Set the startup project for Visual Studio and set multi processor compilation for other projects that we build.
if (MSVC) 
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ShaderBuilder)
endif ()