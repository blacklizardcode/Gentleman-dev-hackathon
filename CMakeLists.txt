cmake_minimum_required(VERSION 3.23)

project(gentleman-dev-hackathon C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

include(FetchContent)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG 5.5
)

FetchContent_MakeAvailable(raylib)

add_executable(MyProgram 
    src/main.c
    src/UI.c
    src/drawRoom.c 
    src/loadTexture.c 
    src/moove.c
    src/mouse.c
    src/globals.c
)

target_link_libraries(MyProgram PRIVATE raylib)

add_custom_command(TARGET MyProgram POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/src/surse
    $<TARGET_FILE_DIR:MyProgram>/surse
)