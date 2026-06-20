cmake_minimum_required(VERSION 3.4.1)

project(swappywrapper)

find_package(games-frame-pacing REQUIRED CONFIG)

file(GLOB_RECURSE UNITY_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR} *.cpp *.h *.c)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Werror")
set (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--wrap=__android_log_print")

# now build app's shared lib
add_library(swappywrapper SHARED
     ${UNITY_SOURCE_FILES}
)

add_definitions("-DEXTERNAL_FRAME_PACING_CODE")

target_include_directories(swappywrapper PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# add lib dependencies
target_link_libraries(swappywrapper
     android
     games-frame-pacing::swappy_static
     log
     c++
)
