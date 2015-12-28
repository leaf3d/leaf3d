add_executable(Leaf WIN32 MACOSX_BUNDLE
    main.cpp
)

target_link_libraries(Leaf
    ${LEAF3D_EXAMPLES_REQUIRED_LIBS}
)
