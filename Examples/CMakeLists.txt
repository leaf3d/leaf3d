include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Engine)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../External)

# Look for GLFW.
find_package(GLFW REQUIRED)
include_directories(${GLFW_INCLUDE_DIR})

# Set necessary libs.
set(LEAF3D_EXAMPLES_REQUIRED_LIBS
    leaf3d
    ${GLFW_LIBRARIES}
    ${LEAF3DUT_REQUIRED_LIBS}
    ${LEAF3D_REQUIRED_LIBS}
)

# Set editable content files.
set(LEAF3D_EXAMPLES_EDITABLE_CONTENT_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Content/basic.vert
    ${CMAKE_CURRENT_SOURCE_DIR}/Content/basic.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/Content/blinnphong.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/Content/simpletexture.frag
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
