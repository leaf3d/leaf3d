add_executable(Leaf
    main.cpp
    ${LEAF3D_EXAMPLES_RESOURCE_FILES}
)

target_link_libraries(Leaf
    ${LEAF3D_EXAMPLES_REQUIRED_LIBS}
)

set_target_properties(Leaf PROPERTIES
    WIN32 TRUE
    MACOSX_BUNDLE TRUE
    RESOURCE "${LEAF3D_EXAMPLES_RESOURCE_FILES}"
)
