add_executable(PostProcessing
    main.cpp
    ${LEAF3D_EXAMPLES_RESOURCE_FILES}
)

target_link_libraries(PostProcessing
    ${LEAF3D_EXAMPLES_REQUIRED_LIBS}
)

set_target_properties(PostProcessing PROPERTIES
    WIN32 TRUE
    MACOSX_BUNDLE TRUE
    RESOURCE "${LEAF3D_EXAMPLES_RESOURCE_FILES}"
)
