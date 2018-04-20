add_executable(PostProcessing WIN32 MACOSX_BUNDLE
    main.cpp
    ${LEAF3D_EXAMPLES_EDITABLE_CONTENT_FILES}
)
add_dependencies(PostProcessing leaf3dExamplesCopyContentFiles)

target_link_libraries(PostProcessing
    ${LEAF3D_EXAMPLES_REQUIRED_LIBS}
)
