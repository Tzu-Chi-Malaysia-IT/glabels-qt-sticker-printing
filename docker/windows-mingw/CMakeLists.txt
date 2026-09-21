cmake_minimum_required(VERSION 3.22)

project(jpeg-smoke LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.7 REQUIRED COMPONENTS Core Gui)

add_executable(jpeg-smoke jpeg-smoke.cpp)

target_link_libraries(jpeg-smoke
    PRIVATE
    Qt6::Core
    Qt6::Gui
)
