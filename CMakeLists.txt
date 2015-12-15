cmake_minimum_required(VERSION 2.8)

# Project details.
project(leaf3d)

set(${PROJECT_NAME}_MAJOR_VERSION 0)
set(${PROJECT_NAME}_MINOR_VERSION 1)
set(${PROJECT_NAME}_PATCH_LEVEL 0)

# Add local repository for FindXXX.cmake modules.
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake/Modules/" ${CMAKE_MODULE_PATH})

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

# Set binaries output folder.
set(LEAF3D_OUTPUT_PATH_PREFIX "${PROJECT_BINARY_DIR}/Binaries")
set(LEAF3D_OUTPUT_PATH_SUFFIX "")
if(${CMAKE_CFG_INTDIR} STREQUAL ".")
    set(LEAF3D_OUTPUT_PATH_SUFFIX ${CMAKE_BUILD_TYPE})
    if(NOT LEAF3D_OUTPUT_PATH_SUFFIX)
        set(LEAF3D_OUTPUT_PATH_SUFFIX "Release")
    endif(NOT LEAF3D_OUTPUT_PATH_SUFFIX)
endif(${CMAKE_CFG_INTDIR} STREQUAL ".")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${LEAF3D_OUTPUT_PATH_PREFIX}/${CMAKE_SYSTEM_NAME}/${LEAF3D_OUTPUT_PATH_SUFFIX}
    CACHE STRING "Where binaries and shared lib files go" FORCE)

if(MSVC)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup ")
endif(MSVC)

# Add source targets.
add_subdirectory(Engine)
add_subdirectory(Examples)
add_subdirectory(Tests)

# Unit testing.
ENABLE_TESTING()
ADD_TEST(leaf3d Tests/leaf3dTests)

