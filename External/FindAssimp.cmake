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

SET(ASSIMP_CMAKE_ARGS
  # -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
  -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
  -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
  -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
  -DBUILD_SHARED_LIBS:BOOL=OFF
  -DASSIMP_NO_EXPORT:BOOL=ON
  -DASSIMP_BUILD_ASSIMP_TOOLS:BOOL=OFF
  -DASSIMP_BUILD_TESTS:BOOL=OFF
  -DASSIMP_BUILD_ZLIB:BOOL=ON
	-DASSIMP_INSTALL_PDB:BOOL=OFF
)

ExternalProject_Add(ext_Assimp
  URL https://github.com/assimp/assimp/archive/v4.1.0.zip
  URL_MD5 e98a03f9496ff761e67f29b645e04316
  CMAKE_ARGS ${ASSIMP_CMAKE_ARGS}
)

ExternalProject_Get_Property(ext_Assimp install_dir)

SET(ASSIMP_INCLUDE_DIR
  ${install_dir}/include
)

IF (MSVC)

	# Windows
	if (MSVC12)
		set(ASSIMP_MSVC_VERSION "vc120")
	elseif (MSVC14)
		set(ASSIMP_MSVC_VERSION "vc140")
	elseif (MSVC15)
		set(ASSIMP_MSVC_VERSION "vc150")
	endif ()

	if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
		set(ASSIMP_BUILD_SUFFIX "d")
	endif ()

  SET(ASSIMP_LIBRARY ${install_dir}/lib/assimp-${ASSIMP_MSVC_VERSION}-mt.lib)
  SET(IRRXML_LIBRARY ${install_dir}/lib/IrrXML.lib)
  # SET(ZLIB_LIBRARY ${install_dir}/lib/zlibstatic${ASSIMP_BUILD_SUFFIX}.lib)
	# WARNING: this is a hack! Probably an issue with Assimp.
  SET(ZLIB_LIBRARY ${install_dir}/lib/zlibstaticd.lib)

ELSE (MSVC)

	# Linux / MacOS
  SET(ASSIMP_LIBRARY ${install_dir}/lib/libassimp.a)
  SET(IRRXML_LIBRARY ${install_dir}/lib/libIrrXML.a)
  SET(ZLIB_LIBRARY ${install_dir}/lib/libzlibstatic.a)

ENDIF (MSVC)

# Add lib target based on external project
add_library(Assimp STATIC IMPORTED)
set_property(TARGET Assimp PROPERTY IMPORTED_LOCATION ${ASSIMP_LIBRARY})
add_dependencies(Assimp ext_Assimp)

# Handle REQUIRD argument, define *_FOUND variable
find_package_handle_standard_args(Assimp
  DEFAULT_MSG ASSIMP_INCLUDE_DIR
  ASSIMP_LIBRARY
)

# find_package(ZLIB REQUIRED)

# Define ASSIMP_LIBRARIES and ASSIMP_INCLUDE_DIRS
if (ASSIMP_FOUND)
	set(ASSIMP_LIBRARIES ${ASSIMP_LIBRARY} ${IRRXML_LIBRARY} ${ZLIB_LIBRARY})
	set(ASSIMP_INCLUDE_DIRS ${ASSIMP_INCLUDE_DIR})
endif ()
