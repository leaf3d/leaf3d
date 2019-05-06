message(STATUS "Configuring leaf3d examples")

# Look for GLFW.
find_package(GLFW REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})

# Set examples required libs.
set(LEAF3D_EXAMPLES_REQUIRED_LIBS
    leaf3d
    ${GLFW_LIBRARIES}
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
