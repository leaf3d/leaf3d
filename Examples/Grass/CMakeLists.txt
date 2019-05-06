set(L3D_EXAMPLE_SOURCES
    main.cpp
)

set(L3D_EXAMPLE_RESOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/basic.vert"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/blinnphong.frag"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/skyBox.vert"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/skyBox.frag"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/grassPlane.frag"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/grassBlades.frag"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/grassBlades.geom"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/grass_blade_diffuse.png"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/grass_diffuse.jpg"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/grass_dirt_diffuse.jpg"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/skybox2_right.jpg"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/skybox2_left.jpg"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/skybox2_top.jpg"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/skybox2_bottom.jpg"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/skybox2_back.jpg"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/skybox2_front.jpg"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/crate.jpg"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/crate_spec.jpg"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/crate_norm.jpg"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/tree1.obj"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/tree1.mtl"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/bark_diffuse.png"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/bark_norm.png"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/leaf_mapple_alpha.png"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/leaf_mapple_diffuse.png"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/leaf_mapple_norm.png"
)

set(L3D_EXAMPLE_RESOURCES_DESTINATION
    $<TARGET_FILE_DIR:Grass>/Resources
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

add_executable(Grass
    ${L3D_EXAMPLE_SOURCES}
)

target_link_libraries(Grass
    ${LEAF3D_EXAMPLES_REQUIRED_LIBS}
)

add_custom_command(TARGET Grass POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${L3D_EXAMPLE_RESOURCES_DESTINATION}
    COMMAND ${CMAKE_COMMAND} -E copy ${L3D_EXAMPLE_RESOURCES} ${L3D_EXAMPLE_RESOURCES_DESTINATION}
)
