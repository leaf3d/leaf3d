include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Engine)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../External)

file(COPY Content DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

# Look for GLFW.
find_package(GLFW REQUIRED)
include_directories(${GLFW_INCLUDE_DIR})

# Set necessary libs.
set(LEAF3D_EXAMPLES_REQUIRED_LIBS
    leaf3d
    ${GLFW_LIBRARIES}
    ${LEAF3D_REQUIRED_LIBS}
)

add_subdirectory(HelloLeaf)
