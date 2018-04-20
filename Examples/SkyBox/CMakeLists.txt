add_executable(SkyBox WIN32 MACOSX_BUNDLE
    main.cpp
    ${LEAF3D_EXAMPLES_EDITABLE_CONTENT_FILES}
)
add_dependencies(SkyBox leaf3dExamplesCopyContentFiles)

target_link_libraries(SkyBox
    ${LEAF3D_EXAMPLES_REQUIRED_LIBS}
)
