add_executable(Frankie WIN32 MACOSX_BUNDLE
    main.cpp
    ${LEAF3D_EXAMPLES_EDITABLE_CONTENT_FILES}
)

target_link_libraries(Frankie
    ${LEAF3D_EXAMPLES_REQUIRED_LIBS}
)
