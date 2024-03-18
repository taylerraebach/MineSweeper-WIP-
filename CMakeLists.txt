cmake_minimum_required(VERSION 3.27)
project(MineSweeper)

set(CMAKE_CXX_STANDARD 17)

set(SFML_INCLUDE_DIR "/usr/local/include")
set(SFML_INCLUDE_DIR "/usr/local/lib")

find_package(SFML 2.5.1 COMPONENTS system window graphics network audio)

add_executable(MineSweeper minesweeper.cpp)
configure_file("Roboto-Medium.ttf" "Roboto-Medium.ttf" COPYONLY)
target_link_libraries(MineSweeper sfml-graphics sfml-window sfml-system)
