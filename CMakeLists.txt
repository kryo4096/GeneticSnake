cmake_minimum_required(VERSION 3.19)
project(GeneticSnake)

set(CMAKE_FIND_DEBUG_MODE 1)
find_package(Eigen3 3.3 REQUIRED)
find_package(SDL2 REQUIRED)

set(CMAKE_CXX_STANDARD 17)

include_directories(GeneticSnake ${SDL2_INCLUDE_DIRS})

add_executable(GeneticSnake main.cpp src/Game.cpp src/Game.hpp src/Ui.cpp src/Ui.hpp src/SimpleActor.cpp src/SimpleActor.hpp)
target_link_libraries (GeneticSnake Eigen3::Eigen SDL2)