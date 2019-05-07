set(L3D_EXAMPLE_SOURCES
    main.cpp
)

set(L3D_EXAMPLE_RESOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/basic.vert"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/basic.frag"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/instancing.vert"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/simpletexture.frag"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/skyBox.vert"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/skyBox.frag"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/skybox3_right.jpg"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/skybox3_left.jpg"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/skybox3_top.jpg"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/skybox3_bottom.jpg"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/skybox3_back.jpg"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/skybox3_front.jpg"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/planet.obj"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/planet.mtl"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/planet.png"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/planet_norm.png"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/rock.obj"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/rock.mtl"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/rock.png"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/rock_norm.png"
)

set(L3D_EXAMPLE_RESOURCES_BASE_DESTINATION
    $<TARGET_FILE_DIR:Planet>
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

add_executable(Planet
    ${L3D_EXAMPLE_SOURCES}
)

target_link_libraries(Planet
    ${LEAF3D_EXAMPLES_REQUIRED_LIBS}
)

add_custom_command(TARGET Planet POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${L3D_EXAMPLE_RESOURCES_DESTINATION}
    COMMAND ${CMAKE_COMMAND} -E copy ${L3D_EXAMPLE_RESOURCES} ${L3D_EXAMPLE_RESOURCES_DESTINATION}
)
