set(L3D_EXAMPLE_SOURCES
    main.cpp
)

set(L3D_EXAMPLE_RESOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/basic.vert"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/basic.frag"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/blinnphong.frag"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/floor.png"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/floor_spec.png"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/floor_norm.png"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/lamp.obj"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/lamp.mtl"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/lamp.png"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/lamp_spec.png"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/crate.jpg"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/crate_spec.jpg"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/crate_norm.jpg"
)

set(L3D_EXAMPLE_RESOURCES_BASE_DESTINATION
    $<TARGET_FILE_DIR:Lamp>
)

if (L3D_MACOS)
    set(L3D_EXAMPLE_SOURCES
        MACOSX_BUNDLE
        ${L3D_EXAMPLE_SOURCES}
    )

    set(L3D_EXAMPLE_RESOURCES_BASE_DESTINATION
        ${L3D_EXAMPLE_RESOURCES_BASE_DESTINATION}/..
    )
elseif (L3D_WIN)
    set(L3D_EXAMPLE_SOURCES
        WIN32
        ${L3D_EXAMPLE_SOURCES}
    )
endif ()

set(L3D_EXAMPLE_RESOURCES_DESTINATION
    ${L3D_EXAMPLE_RESOURCES_BASE_DESTINATION}/Resources
)

add_executable(Lamp
    ${L3D_EXAMPLE_SOURCES}
)

target_link_libraries(Lamp
    ${LEAF3D_EXAMPLES_REQUIRED_LIBS}
)

add_custom_command(TARGET Lamp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${L3D_EXAMPLE_RESOURCES_DESTINATION}
    COMMAND ${CMAKE_COMMAND} -E copy ${L3D_EXAMPLE_RESOURCES} ${L3D_EXAMPLE_RESOURCES_DESTINATION}
)
