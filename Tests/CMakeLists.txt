message(STATUS "Configuring leaf3d tests")

set(LEAF3D_TESTS_SOURCES
    main.cpp
)

set(LEAF3D_TESTS_LIBRARIES
    leaf3d
    ${LEAF3D_REQUIRED_LIBS}
)

add_subdirectory(core)
add_subdirectory(camera)
add_subdirectory(light)
add_subdirectory(mesh)

add_executable(leaf3dTests ${LEAF3D_TESTS_SOURCES})

target_link_libraries(leaf3dTests ${LEAF3D_TESTS_LIBRARIES})
