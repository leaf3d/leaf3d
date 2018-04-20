add_executable(Grass WIN32 MACOSX_BUNDLE
    main.cpp
    ${LEAF3D_EXAMPLES_EDITABLE_CONTENT_FILES}
)
add_dependencies(Grass leaf3dExamplesCopyContentFiles)

target_link_libraries(Grass
    ${LEAF3D_EXAMPLES_REQUIRED_LIBS}
)
