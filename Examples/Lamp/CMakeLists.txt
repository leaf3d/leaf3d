add_executable(Lamp
    main.cpp
    ${LEAF3D_EXAMPLES_RESOURCE_FILES}
)

target_link_libraries(Lamp
    ${LEAF3D_EXAMPLES_REQUIRED_LIBS}
)

set_target_properties(Lamp PROPERTIES
    WIN32 TRUE
    MACOSX_BUNDLE TRUE
    RESOURCE "${LEAF3D_EXAMPLES_RESOURCE_FILES}"
)
