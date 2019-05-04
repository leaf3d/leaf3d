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
file(GLOB_RECURSE LEAF3D_EXAMPLES_RESOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/Resources/*.vert"
    "${CMAKE_CURRENT_SOURCE_DIR}/Resources/*.frag"
    "${CMAKE_CURRENT_SOURCE_DIR}/Resources/*.geom"
    "${CMAKE_CURRENT_SOURCE_DIR}/Resources/*.png"
    "${CMAKE_CURRENT_SOURCE_DIR}/Resources/*.jpg"
    "${CMAKE_CURRENT_SOURCE_DIR}/Resources/*.obj"
    "${CMAKE_CURRENT_SOURCE_DIR}/Resources/*.mtl"
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
