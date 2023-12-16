cmake_minimum_required(VERSION 3.5)
project(TimelessJourneys LANGUAGES CXX)
set(CMAKE_SIZEOF_VOID_P 8)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "C:/SDL2_sources")

find_package(SDL2 CONFIG REQUIRED)
if(NOT SDL2_FOUND)
message(FATAL_ERROR "SDL2 not found")
endif()

find_package(SDL2_image CONFIG REQUIRED)
if(NOT SDL2_image_FOUND)
    message(FATAL_ERROR "SDL2_image not found")
endif()

find_package(SDL2_ttf CONFIG REQUIRED)
if(NOT SDL2_ttf_FOUND)
    message(FATAL_ERROR "SDL2_ttf not found")
endif()

set(SOURCES
    main.cpp
    game.h game.cpp
    TextureManager.h
    TextureManager.cpp
    gameobject.h gameobject.cpp
    map.h
    map.cpp
    ECS/ECS.h
    ECS/Components.h
    ECS/SpriteComponent.h
    ECS/TransformComponent.h
    Vector2D.h
    Vector2D.cpp
    ECS/KeyboardController.h
    ECS/ColliderComponent.h
    Collision.h
    Collision.cpp
    ECS/TileComponent.h
    ECS/ECS.cpp
    ECS/Animation.h
    AssetManager.h
    AssetManager.cpp
    ECS/ProjectileComponent.h
    ECS/UILabel.h
    ECS/Stats.h
    ECS/Stats.cpp
    json.hpp)

add_executable(TimelessJourneys ${SOURCES}
    timer.hpp
    ECS/weaponComponent.h
    world.hpp world.cpp

)

target_link_libraries(TimelessJourneys PRIVATE SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf)

set(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_definitions(TimelessJourneys PRIVATE ROOT_DIR="${ROOT_DIR}")

