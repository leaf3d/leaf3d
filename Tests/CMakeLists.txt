include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Engine)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../External)

set(LEAF3D_TESTS_SOURCES
    main.cpp
)

set(LEAF3D_TESTS_LIBRARIES
    leaf3d
    ${LEAF3DUT_REQUIRED_LIBS}
    ${LEAF3D_REQUIRED_LIBS}
)

add_subdirectory(core)
add_subdirectory(camera)
add_subdirectory(light)
add_subdirectory(mesh)

add_executable(leaf3dTests ${LEAF3D_TESTS_SOURCES})

target_link_libraries(leaf3dTests ${LEAF3D_TESTS_LIBRARIES})
