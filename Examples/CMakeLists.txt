message(STATUS "Configuring leaf3d examples")

# Look for GLFW.
find_package(GLFW REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})

# Set examples required libs.
set(LEAF3D_EXAMPLES_REQUIRED_LIBS
    leaf3d
    ${GLFW_LIBRARIES}
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
add_dependencies(leaf3dExamplesCopyContentFiles leaf3d GLFW)

# Add example projects.
add_subdirectory(Leaf)
add_subdirectory(Frankie)
add_subdirectory(Lamp)
add_subdirectory(PostProcessing)
add_subdirectory(SkyBox)
add_subdirectory(Water)
add_subdirectory(Grass)
add_subdirectory(Planet)
