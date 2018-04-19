# Locate Assimp library (4.x)
#
# This module defines:
#
# ASSIMP_FOUND, if false, do not try to link to Assimp
# ASSIMP_LIBRARY, the name of the library to link against
# ASSIMP_LIBRARIES, the full list of libs to link against
# ASSIMP_INCLUDE_DIR, where to find assimp/*.h files
#=============================================================================

INCLUDE(ExternalProject)
INCLUDE(FindPackageHandleStandardArgs)

MESSAGE(STATUS "Looking for Assimp")

SET(ASSIMP_CMAKE_ARGS
    # -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
    -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
	  -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
    -DASSIMP_NO_EXPORT:BOOL=ON
    -DASSIMP_BUILD_ASSIMP_TOOLS:BOOL=OFF
    -DASSIMP_BUILD_TESTS:BOOL=OFF
)

ExternalProject_Add(Assimp
    URL https://github.com/assimp/assimp/archive/v4.1.0.zip
    URL_MD5 e98a03f9496ff761e67f29b645e04316
    CMAKE_ARGS ${ASSIMP_CMAKE_ARGS}
)

ExternalProject_Get_Property(Assimp install_dir)

SET(ASSIMP_INCLUDE_DIR
    ${install_dir}/include
)

find_library(ASSIMP_LIBRARY
             NAMES assimp
             PATHS ${install_dir}/lib
)

# Handle REQUIRD argument, define *_FOUND variable
find_package_handle_standard_args(Assimp
                                  DEFAULT_MSG ASSIMP_INCLUDE_DIR
                                  ASSIMP_LIBRARY
)

# Define ASSIMP_LIBRARIES and ASSIMP_INCLUDE_DIRS
if (ASSIMP_FOUND)
	set(ASSIMP_LIBRARIES ${ASSIMP_LIBRARY})
	set(ASSIMP_INCLUDE_DIRS ${ASSIMP_INCLUDE_DIR})
endif()
