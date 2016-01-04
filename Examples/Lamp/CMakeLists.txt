add_executable(Lamp WIN32 MACOSX_BUNDLE
    main.cpp
)

target_link_libraries(Lamp
    ${LEAF3D_EXAMPLES_REQUIRED_LIBS}
)
