# Locate local GLAD repository
#
# This module defines:
#
# GLAD_FOUND, if false, do not try to link to GLAD
# GLAD_SOURCES, the full list of sources to include
# GLAD_INCLUDE_DIR, where to find glad.h
#=============================================================================

SET(GLAD_INCLUDE_DIR
    ${CMAKE_MODULE_PATH}/glad/include
)

SET(GLAD_SOURCES
    ${CMAKE_MODULE_PATH}/glad/src/glad.c
)

SET(GLAD_FOUND
    ON
)
