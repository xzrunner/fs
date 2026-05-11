cmake_minimum_required(VERSION 3.16)

project(fs)

################################################################################
# Source groups
################################################################################

set(inc
    "fs_file.h"
)
source_group("inc" FILES ${inc})

set(src
    "fs_file_common.c"
)
source_group("src" FILES ${src})

set(ALL_FILES
    ${inc}
    ${src}
)

add_library(fs ${ALL_FILES})

target_include_directories(fs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
