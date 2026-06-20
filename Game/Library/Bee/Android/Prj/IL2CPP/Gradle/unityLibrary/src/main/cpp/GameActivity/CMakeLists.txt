cmake_minimum_required(VERSION 3.4.1)
project(game)

find_package(game-activity REQUIRED CONFIG)

file(GLOB_RECURSE UNITY_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR} *.cpp *.h *.c)

# Keep commented
# Shows preprocessed files, helpful when you want to see what your macro is epxanded to
# For ex., .cxx\Debug\3xw3n3b3\armeabi-v7a\UnityGameActivitySoftKeyboard.ii
#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -save-temps")

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Werror")

add_library(game SHARED
    ${UNITY_SOURCE_FILES}
)

add_definitions("-DEXTERNAL_GAME_ACTIVITY_CODE")

target_include_directories(game PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(game
    android
    game-activity::game-activity_static
    log
    c++
)
