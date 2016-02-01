add_executable(Leaf WIN32 MACOSX_BUNDLE
    main.cpp
    ${LEAF3D_EXAMPLES_EDITABLE_CONTENT_FILES}
)

target_link_libraries(Leaf
    ${LEAF3D_EXAMPLES_REQUIRED_LIBS}
)
