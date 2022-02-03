cmake_minimum_required(VERSION 3.21)
set(PROJECT_NAME EntityComponentSystem2022)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME}
        src/ecs/components/Archetype.cpp
        src/ecs/components/ArchetypeManager.cpp

        src/ecs/Core.cpp
        src/ecs/Common.cpp
        src/ecs/EntityManager.cpp
        src/ecs/SystemManager.cpp

        src/Main.cpp

        include/ecs/components/ArchetypeManager.h
        include/ecs/components/Archetype.h
        include/ecs/components/ComponentArray.h

        include/ecs/Ecs.h
        include/ecs/Core.h
        include/ecs/Common.h
        include/ecs/EntityManager.h
        include/ecs/BaseSystem.h
        include/ecs/SystemManager.h

        include/ecs/Entities.h)

target_include_directories(${PROJECT_NAME} PUBLIC
        include
        include/ecs
        include/ecs/components
        )

target_precompile_headers(${PROJECT_NAME} PUBLIC

        )