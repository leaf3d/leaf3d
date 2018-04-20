# Locate GLFW library (3.x)
#
# This module defines:
#
# GLFW_FOUND, if false, do not try to link to GLFW
# GLFW_LIBRARY, the name of the library to link against
# GLFW_LIBRARIES, the full list of libs to link against
# GLFW_INCLUDE_DIR, where to find glfw3.h
#=============================================================================

INCLUDE(ExternalProject)
INCLUDE(FindPackageHandleStandardArgs)

SET(GLFW_CMAKE_ARGS
  -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
  -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
	-DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
  -DGLFW_BUILD_DOCS:BOOL=OFF
  -DGLFW_BUILD_EXAMPLES:BOOL=OFF
  -DGLFW_BUILD_TESTS:BOOL=OFF
)

ExternalProject_Add(ext_GLFW
  URL https://github.com/glfw/glfw/archive/3.2.1.zip
  URL_MD5 cb412cba8daba35d849ddfe78700a210
  CMAKE_ARGS ${GLFW_CMAKE_ARGS}
)

ExternalProject_Get_Property(ext_GLFW install_dir)

SET(GLFW_INCLUDE_DIR
  ${install_dir}/include
)

IF (MSVC)

  # Windows
  SET(GLFW_LIBRARY ${install_dir}/lib/glfw3.lib)

ELSE (MSVC)

  # Linux / MacOS
  SET(GLFW_LIBRARY ${install_dir}/lib/libglfw3.a)
  
ENDIF (MSVC)

# Add lib target based on external project
add_library(GLFW STATIC IMPORTED)
set_property(TARGET GLFW PROPERTY IMPORTED_LOCATION ${GLFW_LIBRARY})
add_dependencies(GLFW ext_GLFW)

# Handle REQUIRD argument, define *_FOUND variable
find_package_handle_standard_args(GLFW
  DEFAULT_MSG GLFW_INCLUDE_DIR
  GLFW_LIBRARY
)

# Define GLFW_LIBRARIES and GLFW_INCLUDE_DIRS
if (GLFW_FOUND)
	set(GLFW_LIBRARIES ${GLFW_LIBRARY})
	set(GLFW_INCLUDE_DIRS ${GLFW_INCLUDE_DIR})

  # GLFW may require threads on your system.
  # The Apple build may not need an explicit flag because one of the
  # frameworks may already provide it.
  # But for non-OSX systems, I will use the CMake Threads package.
  # In fact, there seems to be a problem if I used the Threads package
  # and try using this line, so I'm just skipping it entirely for OS X.
  IF (UNIX AND NOT APPLE)
		FIND_PACKAGE(Threads)
		SET(GLFW_LIBRARIES ${GLFW_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

		if (CMAKE_DL_LIBS)
			SET(GLFW_LIBRARIES ${GLFW_LIBRARIES} ${CMAKE_DL_LIBS})
		endif ()

		find_library(RT_LIBRARY rt)
		mark_as_advanced(RT_LIBRARY)
		if (RT_LIBRARY)
			SET(GLFW_LIBRARIES ${GLFW_LIBRARIES} ${RT_LIBRARY})
		endif ()

	ENDIF (UNIX AND NOT APPLE)

  # For OS X, GLFW uses Cocoa as a backend so it must link to Cocoa.
  IF (APPLE OR ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    FIND_LIBRARY(COCOA_LIBRARY Cocoa)
    FIND_LIBRARY(OPENGL_LIBRARY OpenGL)
    FIND_LIBRARY(IOKIT_LIBRARY IOKit)
    FIND_LIBRARY(COREVIDEO_LIBRARY CoreVideo)
    SET(GLFW_LIBRARIES
      ${GLFW_LIBRARIES}
      ${COCOA_LIBRARY}
      ${OPENGL_LIBRARY}
      ${IOKIT_LIBRARY}
      ${COREVIDEO_LIBRARY}
    )
  ENDIF (APPLE OR ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

  # For MinGW library.
  IF (MINGW)
    # MinGW needs an additional library, mwindows
    # It's total link flags should look like -lmingw32 -lGLFW -lmwindows
    # (Actually on second look, I think it only needs one of the m* libraries.)
    SET(MINGW32_LIBRARY mingw32 CACHE STRING "mwindows for MinGW")
    SET(GLFW_LIBRARIES ${MINGW32_LIBRARY} ${GLFW_LIBRARIES})
  ENDIF (MINGW)

  # For Unix, GLFW should be linked to X11-related libraries.
  IF (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    FIND_PACKAGE(X11 REQUIRED)
    SET(GLFW_LIBRARIES ${GLFW_LIBRARIES} ${X11_X11_LIB} ${X11_Xrandr_LIB} ${X11_Xxf86vm_LIB} ${X11_Xinput_LIB} ${X11_Xinerama_LIB} ${X11_Xcursor_LIB})
  ENDIF (${CMAKE_SYSTEM_NAME} MATCHES "Linux")

ENDIF(GLFW_FOUND)
