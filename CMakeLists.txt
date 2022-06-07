cmake_minimum_required(VERSION 3.13)

project(SpaceShooter)

find_package(SFML 2.5 COMPONENTS 
    system window graphics network audio REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(inc inc.cpp)
add_library(menu menu.cpp)
add_library(models models.cpp)
add_library(sprites sprites.cpp)
add_library(levels levels.cpp)

target_link_libraries(menu inc)
target_link_libraries(levels inc sprites)
target_link_libraries(models inc sprites)
target_link_libraries(sprites inc models)

add_executable(SpaceShooter main.cpp)
target_link_libraries(SpaceShooter sfml-graphics)
target_link_libraries(SpaceShooter inc menu levels)

add_executable(ModelCreator model_creator.cpp)
target_link_libraries(ModelCreator sfml-graphics)
target_link_libraries(ModelCreator inc menu models)

IF(CMAKE_BUILD_TYPE MATCHES Debug)
    target_link_options(SpaceShooter PRIVATE -fsanitize=address,undefined -g)
ELSEIF(CMAKE_BUILD_TYPE MATCHES Release)
    add_compile_options(-O2)
ENDIF()
