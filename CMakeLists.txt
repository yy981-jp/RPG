cmake_minimum_required(VERSION 3.20...4.1)
project(RPG) # RPG
add_compile_options(-fdiagnostics-color=always)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# ソースファイル
file(GLOB SOURCES src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

# ライブラリをリンク
target_link_libraries(${PROJECT_NAME} PRIVATE
    SDL2
    SDL2_image
)
