add_executable(Water WIN32 MACOSX_BUNDLE
    main.cpp
    ${LEAF3D_EXAMPLES_EDITABLE_CONTENT_FILES}
)
add_dependencies(Water leaf3dExamplesCopyContentFiles)

target_link_libraries(Water
    ${LEAF3D_EXAMPLES_REQUIRED_LIBS}
)
