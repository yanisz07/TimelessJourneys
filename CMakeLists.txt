cmake_minimum_required(VERSION 3.5)
project(TimelessJourneys LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "C:/SDL2_sources")

find_package(SDL2 CONFIG REQUIRED)
if(NOT SDL2_FOUND)
message(FATAL_ERROR "SDL2 not found")
else()
    message(status "Found SDL2 in;${SDL2_DIR}")
endif()

find_package(SDL2_image CONFIG REQUIRED)
if(NOT SDL2_image_FOUND)
    message(FATAL_ERROR "SDL2_image not found")
endif()

include_directories(${SDL2_INCLUDE_DIRS})


set(SOURCES
    main.cpp
    game.h game.cpp
    TextureManager.h
    TextureManager.cpp
    Collision.cpp
    map.cpp
    gameobject.cpp
    Vector2D.cpp
    Collision.h
    gameobject.h
    Vector2D.h
    map.h
    ECS/Components.h
    ECS/ColliderComponent.h
    ECS/ECS.h
    ECS/KeyboardController.h
    ECS/SpriteComponent.h
    ECS/TransformComponent.h)


add_executable(TimelessJourneys ${SOURCES})

target_link_libraries(TimelessJourneys PRIVATE SDL2_image::SDL2_image ${SDL2_LIBRARIES})

set(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_definitions(TimelessJourneys PRIVATE ROOT_DIR="${ROOT_DIR}")

