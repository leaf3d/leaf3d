cmake_minimum_required(VERSION 2.8)

# Project details.
project(leaf3d)

set(${PROJECT_NAME}_MAJOR_VERSION 0)
set(${PROJECT_NAME}_MINOR_VERSION 1)
set(${PROJECT_NAME}_PATCH_LEVEL 0)

# Add local repository for FindXXX.cmake modules.
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/External/" ${CMAKE_MODULE_PATH})

# Set binaries output folder.
set(LEAF3D_OUTPUT_PATH_PREFIX "${PROJECT_BINARY_DIR}/Binaries")
set(LEAF3D_OUTPUT_PATH_SUFFIX "")
if(${CMAKE_CFG_INTDIR} STREQUAL ".")
    set(LEAF3D_OUTPUT_PATH_SUFFIX ${CMAKE_BUILD_TYPE})
    if(NOT LEAF3D_OUTPUT_PATH_SUFFIX)
        set(LEAF3D_OUTPUT_PATH_SUFFIX "Release")
    endif(NOT LEAF3D_OUTPUT_PATH_SUFFIX)
endif(${CMAKE_CFG_INTDIR} STREQUAL ".")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${LEAF3D_OUTPUT_PATH_PREFIX}/${CMAKE_SYSTEM_NAME}/${LEAF3D_OUTPUT_PATH_SUFFIX}
    CACHE STRING "Where binaries and shared lib files go" FORCE)

# Config options.
option (L3D_BUILD_UTILITY "If the utility functions are built as well." ON)
option (L3D_BUILD_EXAMPLES "If the official examples are built as well." ON)
option (L3D_BUILD_TESTS "If the official tests are built as well." ON)

if (L3D_BUILD_EXAMPLES)
    set(L3D_BUILD_UTILITY ON)
endif (L3D_BUILD_EXAMPLES)

# Look for OpenGL.
find_package(OpenGL REQUIRED)

# Look for GLEW.
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIR})

# Set necessary libs.
set(LEAF3D_REQUIRED_LIBS
    ${GLEW_LIBRARIES}
    ${OPENGL_LIBRARY}
)

if (L3D_BUILD_UTILITY)
    # Look for Assimp.
    find_package(Assimp REQUIRED)
    include_directories(${ASSIMP_INCLUDE_DIR})

    # Set utility required libs.
    set(LEAF3DUT_REQUIRED_LIBS
        ${ASSIMP_LIBRARIES}
        png
    )
endif (L3D_BUILD_UTILITY)

# Engine target.
add_subdirectory(Engine)

# Samples target.
if (L3D_BUILD_EXAMPLES)
    add_subdirectory(Examples)
endif (L3D_BUILD_EXAMPLES)

# Tests target.
if (L3D_BUILD_TESTS)
    add_subdirectory(Tests)
    enable_testing()
    add_test(leaf3d Tests/leaf3dTests)
endif (L3D_BUILD_TESTS)

