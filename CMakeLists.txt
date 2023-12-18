cmake_minimum_required(VERSION 3.5)
project(TimelessJourneys LANGUAGES CXX)
set(CMAKE_SIZEOF_VOID_P 8)


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

find_package(SDL2_ttf CONFIG REQUIRED)
if(NOT SDL2_ttf_FOUND)
    message(FATAL_ERROR "SDL2_ttf not found")
endif()


#include_directories(${SDL2_INCLUDE_DIRS})
#>>>>>>> Develop_attack

set(SOURCES
    main.cpp
    game.hpp game.cpp
    TextureManager.hpp
    TextureManager.cpp
    gameobject.hpp gameobject.cpp
    map.hpp
    map.cpp
    ECS/ECS.hpp
    ECS/Components.hpp
    ECS/SpriteComponent.hpp
    ECS/TransformComponent.hpp
    Vector2D.hpp
    Vector2D.cpp
    ECS/KeyboardController.hpp
    ECS/ColliderComponent.hpp
    Collision.hpp
    Collision.cpp
    ECS/TileComponent.hpp
    ECS/ECS.cpp
    ECS/Animation.hpp
    AssetManager.hpp
    AssetManager.cpp
    ECS/ProjectileComponent.hpp
    ECS/UILabel.hpp
    ECS/Stats.hpp
    ECS/Stats.cpp
    json.hpp)

add_executable(TimelessJourneys ${SOURCES}
    timer.hpp
    ECS/weaponComponent.hpp
    world.hpp world.cpp

)

target_link_libraries(TimelessJourneys PRIVATE SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf)

set(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_definitions(TimelessJourneys PRIVATE ROOT_DIR="${ROOT_DIR}")
