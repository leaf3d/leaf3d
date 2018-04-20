if (L3D_BUILD_UTILITY)
    message(STATUS "Configuring leaf3d (with utilities)")
else (L3D_BUILD_UTILITY)
    message(STATUS "Configuring leaf3d")
endif (L3D_BUILD_UTILITY)

# Look for OpenGL and GLAD.
find_package(OpenGL REQUIRED)
find_package(GLAD REQUIRED)
include_directories(${GLAD_INCLUDE_DIR})

# Set required libs.
set(LEAF3D_REQUIRED_LIBS
    ${OPENGL_LIBRARY}
)

set(LEAF3D_SOURCES
    leaf3d/platform.h
    leaf3d/types.h
    leaf3d/L3DResource.h
    leaf3d/L3DBuffer.h
    leaf3d/L3DTexture.h
    leaf3d/L3DShader.h
    leaf3d/L3DShaderProgram.h
    leaf3d/L3DFrameBuffer.h
    leaf3d/L3DMaterial.h
    leaf3d/L3DCamera.h
    leaf3d/L3DLight.h
    leaf3d/L3DMesh.h
    leaf3d/L3DRenderQueue.h
    leaf3d/L3DRenderer.h
    leaf3d/leaf3d.h
    L3DResource.cpp
    L3DBuffer.cpp
    L3DTexture.cpp
    L3DShader.cpp
    L3DShaderProgram.cpp
    L3DFrameBuffer.cpp
    L3DMaterial.cpp
    L3DCamera.cpp
    L3DLight.cpp
    L3DMesh.cpp
    L3DRenderQueue.cpp
    L3DRenderer.cpp
    leaf3d.cpp
)

set(LEAF3D_DEPENDENCIES
    # Empty.
)

if (L3D_BUILD_UTILITY)
    # Look for Assimp.
    find_package(Assimp REQUIRED)
    include_directories(${ASSIMP_INCLUDE_DIRS})

    # Set utility required libs.
    set(LEAF3DUT_REQUIRED_LIBS
        ${ASSIMP_LIBRARIES}
    )

    set(LEAF3DUT_SOURCES
        leaf3d/leaf3dut.h
        leaf3dut.cpp
    )

    set(LEAF3DUT_DEPENDENCIES
        Assimp
    )

    set(LEAF3D_REQUIRED_LIBS
        ${LEAF3D_REQUIRED_LIBS}
        ${LEAF3DUT_REQUIRED_LIBS}
    )

    set(LEAF3D_SOURCES
        ${LEAF3D_SOURCES}
        ${LEAF3DUT_SOURCES}
    )

    set(LEAF3D_DEPENDENCIES
        ${LEAF3D_DEPENDENCIES}
        ${LEAF3DUT_DEPENDENCIES}
    )
endif (L3D_BUILD_UTILITY)

add_library(leaf3d STATIC ${LEAF3D_SOURCES} ${GLAD_SOURCES})
add_dependencies(leaf3d ${LEAF3D_DEPENDENCIES})

target_link_libraries(leaf3d
    ${LEAF3D_REQUIRED_LIBS}
    ${CMAKE_DL_LIBS}
)

target_include_directories(leaf3d PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  ${GLAD_INCLUDE_DIR}
)
