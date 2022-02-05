cmake_minimum_required(VERSION 3.21)
set(PROJECT_NAME EntityComponentSystem2022_Test)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME}
        Main.cpp
        )

include(${CMAKE_CURRENT_SOURCE_DIR}/../CMakeLists.txt)
find_library(ECS EntityComponentSystem2022 ../ ../cmake-build-debug)
target_link_libraries(${PROJECT_NAME} PUBLIC ${ECS})

target_include_directories(${PROJECT_NAME} PUBLIC
        ../include
        ../include/systems

        ..
        )
