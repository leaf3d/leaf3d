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

add_library(leaf3d SHARED ${LEAF3D_SOURCES} ${GLAD_SOURCES})

target_link_libraries(leaf3d
    ${LEAF3D_REQUIRED_LIBS}
    ${CMAKE_DL_LIBS}
)

if (L3D_BUILD_UTILITY)
    set(LEAF3DUT_SOURCES
        leaf3d/leaf3dut.h
        leaf3dut.cpp
    )

    add_library(leaf3dut SHARED ${LEAF3DUT_SOURCES})
    add_dependencies(leaf3dut leaf3d)

    target_link_libraries(leaf3dut
        leaf3d
        ${LEAF3DUT_REQUIRED_LIBS}
        ${CMAKE_DL_LIBS}
    )
endif (L3D_BUILD_UTILITY)
