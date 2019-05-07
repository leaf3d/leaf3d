set(L3D_EXAMPLE_SOURCES
    main.cpp
)

set(L3D_EXAMPLE_RESOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/basic.vert"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/simpletexture.frag"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/frankie.obj"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/frankie.mtl"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/frankie_diffuse.png"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/frankie_env_diffuse.png"
)

set(L3D_EXAMPLE_RESOURCES_BASE_DESTINATION
    $<TARGET_FILE_DIR:PostProcessing>
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

add_executable(PostProcessing
    ${L3D_EXAMPLE_SOURCES}
)

target_link_libraries(PostProcessing
    ${LEAF3D_EXAMPLES_REQUIRED_LIBS}
)

add_custom_command(TARGET PostProcessing POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${L3D_EXAMPLE_RESOURCES_DESTINATION}
    COMMAND ${CMAKE_COMMAND} -E copy ${L3D_EXAMPLE_RESOURCES} ${L3D_EXAMPLE_RESOURCES_DESTINATION}
)
