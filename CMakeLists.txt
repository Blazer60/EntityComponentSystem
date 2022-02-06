cmake_minimum_required(VERSION 3.14)
set(LIBRARY_NAME EntityComponentSystem2022)

set(CMAKE_CXX_STANDARD 17)

add_library(${LIBRARY_NAME} STATIC
        ${CMAKE_CURRENT_LIST_DIR}/src/components/Archetype.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/components/ArchetypeManager.cpp

        ${CMAKE_CURRENT_LIST_DIR}/src/Core.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/Common.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/EntityManager.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/systems/SystemManager.cpp

        ${CMAKE_CURRENT_LIST_DIR}/src/components/ArchetypeManager.h
        ${CMAKE_CURRENT_LIST_DIR}/src/components/Archetype.h
        ${CMAKE_CURRENT_LIST_DIR}/src/components/ComponentArray.h

        ${CMAKE_CURRENT_LIST_DIR}/include/Ecs.h
        ${CMAKE_CURRENT_LIST_DIR}/include/Core.h
        ${CMAKE_CURRENT_LIST_DIR}/include/Common.h
        ${CMAKE_CURRENT_LIST_DIR}/src/EntityManager.h
        ${CMAKE_CURRENT_LIST_DIR}/include/systems/BaseSystem.h
        ${CMAKE_CURRENT_LIST_DIR}/src/systems/SystemManager.h

        ${CMAKE_CURRENT_LIST_DIR}/include/systems/Entities.h
        )

target_include_directories(${LIBRARY_NAME} PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
        ${CMAKE_CURRENT_LIST_DIR}/include/systems

        ${CMAKE_CURRENT_LIST_DIR}/src
        ${CMAKE_CURRENT_LIST_DIR}/src/components
        ${CMAKE_CURRENT_LIST_DIR}/src/systems
        )

