set(L3D_EXAMPLE_SOURCES
    main.cpp
)

set(L3D_EXAMPLE_RESOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/basic.vert"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/basic.frag"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/blinnphong.frag"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/skyBox.vert"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/skyBox.frag"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/skybox1_right.jpg"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/skybox1_left.jpg"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/skybox1_top.jpg"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/skybox1_bottom.jpg"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/skybox1_back.jpg"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/skybox1_front.jpg"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/crate.jpg"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/crate_spec.jpg"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/crate_norm.jpg"
)

set(L3D_EXAMPLE_RESOURCES_DESTINATION
    $<TARGET_FILE_DIR:SkyBox>/Resources
)

if (L3D_MACOS)
    set(L3D_EXAMPLE_SOURCES
        ${L3D_EXAMPLE_SOURCES}
        MACOSX_BUNDLE
    )
elseif (L3D_WIN)
    set(L3D_EXAMPLE_SOURCES
        ${L3D_EXAMPLE_SOURCES}
        WIN32
    )
endif ()

add_executable(SkyBox
    ${L3D_EXAMPLE_SOURCES}
)

target_link_libraries(SkyBox
    ${LEAF3D_EXAMPLES_REQUIRED_LIBS}
)

add_custom_command(TARGET SkyBox POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${L3D_EXAMPLE_RESOURCES_DESTINATION}
    COMMAND ${CMAKE_COMMAND} -E copy ${L3D_EXAMPLE_RESOURCES} ${L3D_EXAMPLE_RESOURCES_DESTINATION}
)
