cmake_minimum_required(VERSION 3.13)

project(SpaceShooter)

find_package(SFML 2.5 COMPONENTS 
    system window graphics network audio REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(SpaceShooter main.cpp)
target_link_libraries(SpaceShooter sfml-graphics)

IF(CMAKE_BUILD_TYPE MATCHES Debug)
    target_link_options(SpaceShooter PRIVATE -fsanitize=address,undefined -g)
ELSEIF(CMAKE_BUILD_TYPE MATCHES Release)
    add_compile_options(-O2)
ENDIF()
