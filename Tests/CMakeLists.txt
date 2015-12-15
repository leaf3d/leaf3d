include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Engine)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../External)

set(SOURCES
    main.cpp
)

set(LIBRARIES
    leaf3d
)

add_executable(leaf3dTests ${SOURCES})

target_link_libraries(leaf3dTests ${LIBRARIES})
