# Set necessary libs.
set(LEAF3D_EXAMPLES_REQUIRED_LIBS
    leaf3d
    leaf3dut
    ${LEAF3_EXAMPLES_REQUIRED_LIBS}
    ${LEAF3DUT_REQUIRED_LIBS}
    ${LEAF3D_REQUIRED_LIBS}
)

# Set editable content files.
file(GLOB_RECURSE LEAF3D_EXAMPLES_EDITABLE_CONTENT_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/Content/*.vert"
    "${CMAKE_CURRENT_SOURCE_DIR}/Content/*.geom"
    "${CMAKE_CURRENT_SOURCE_DIR}/Content/*.frag"
)

# Copy content files.
add_custom_target(
    leaf3dExamplesCopyContentFiles ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Content ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Content
)

# Add example projects.
add_subdirectory(Leaf)
add_subdirectory(Frankie)
add_subdirectory(Lamp)
add_subdirectory(PostProcessing)
add_subdirectory(SkyBox)
add_subdirectory(Water)
add_subdirectory(Grass)
add_subdirectory(Planet)
