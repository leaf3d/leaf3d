# Locate Assimp library (3.x)
#
# This module defines:
#
# ASSIMP_FOUND, if false, do not try to link to Assimp
# ASSIMP_LIBRARY, the name of the library to link against
# ASSIMP_LIBRARIES, the full list of libs to link against
# ASSIMP_INCLUDE_DIR, where to find assimp.hpp
#=============================================================================

FIND_PATH(ASSIMP_INCLUDE_DIR Importer.hpp
        HINTS
        $ENV{GLFWDIR}
        PATH_SUFFIXES include
        PATHS
        ~/Library/Frameworks
        /Library/Frameworks
    ~/Library/Frameworks/AssImp.framework/Headers
    /Library/Frameworks/AssImp.framework/Headers
        /usr/local/include/
        /usr/include/
        /sw # Fink
        /opt/local # DarwinPorts
        /opt/csw # Blastwave
        /opt
    C:/assimp
    "$ENV{ProgramFiles}/assimp"
)

FIND_LIBRARY(ASSIMP_LIBRARY
        NAMES assimp ASSIMP
        HINTS
        $ENV{ASSIMPDIR}
        PATH_SUFFIXES lib64 lib
        PATHS
        /sw
        /usr
        /usr/local
        /opt/local
        /opt/csw
        /opt
    C:/assimp
    "$ENV{ProgramFiles}/assimp"
)

SET(ASSIMP_FOUND FALSE)

IF(NOT ASSIMP_LIBRARY OR FORCE_DOWNLOAD_ASSIMP)
    # If not found, try to build with local sources.
    # It uses CMake's "ExternalProject_Add" target.
    MESSAGE(STATUS "Preparing external ASSIMP project")
    INCLUDE(ExternalProject)
    ExternalProject_Add(project_assimp
        URL https://github.com/assimp/assimp/archive/v3.2.zip
        URL_MD5 a2297eb35d7c321ec230627aa29ec314
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR> -DBUILD_SHARED_LIBS=OFF
        LOG_DOWNLOAD 1
        LOG_UPDATE 1
        LOG_CONFIGURE 1
        LOG_BUILD 1
        LOG_TEST 1
        LOG_INSTALL 1
    )
    MESSAGE(STATUS "External ASSIMP project done")

    ExternalProject_Get_Property(project_assimp install_dir)
    SET(ASSIMP_INCLUDE_DIR
        ${install_dir}/src/project_assimp/include
    )

    IF(WIN32)
        SET(ASSIMP_LIBRARY
            ${install_dir}/lib/assimp.lib
        )
    ELSE(WIN32)
        SET(ASSIMP_LIBRARY
            ${install_dir}/lib/libassimp.a
        )
    ENDIF(WIN32)

ENDIF(NOT ASSIMP_LIBRARY OR FORCE_DOWNLOAD_ASSIMP)

IF(ASSIMP_LIBRARY)

    SET(ASSIMP_LIBRARIES ${ASSIMP_LIBRARY} CACHE STRING "All the libs required to link Assimp")

    # For Unix, Assimp should be linked to zlib library.
    IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    FIND_PACKAGE(ZLIB REQUIRED)
    SET(ASSIMP_LIBRARIES ${ASSIMP_LIBRARIES} ${ZLIB_LIBRARIES})
    ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

    # Set the final string here so the GUI reflects the final state.
    SET(ASSIMP_LIBRARY ${ASSIMP_LIBRARY} CACHE STRING "Where the ASSIMP Library can be found")

    SET(ASSIMP_FOUND TRUE)

ENDIF(ASSIMP_LIBRARY)

MESSAGE("-- Found ASSIMP: ${ASSIMP_FOUND}")
