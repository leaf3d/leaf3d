INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Engine)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../External)

set(SOURCES
    main.cpp
)

set(LIBRARIES
    leaf3d
)

ADD_EXECUTABLE(leaf3dTests ${SOURCES})

TARGET_LINK_LIBRARIES(leaf3dTests ${LIBRARIES})
