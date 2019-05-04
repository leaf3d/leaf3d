add_executable(Grass
    main.cpp
    ${LEAF3D_EXAMPLES_RESOURCE_FILES}
)

target_link_libraries(Grass
    ${LEAF3D_EXAMPLES_REQUIRED_LIBS}
)

set_target_properties(Grass PROPERTIES
    WIN32 TRUE
    MACOSX_BUNDLE TRUE
    RESOURCE "${LEAF3D_EXAMPLES_RESOURCE_FILES}"
)
