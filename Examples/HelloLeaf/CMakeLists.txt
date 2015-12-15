add_executable(HelloLeaf WIN32 MACOSX_BUNDLE
    main.cpp
)

target_link_libraries(HelloLeaf
    ${LEAF3D_EXAMPLES_REQUIRED_LIBS}
    png
)
