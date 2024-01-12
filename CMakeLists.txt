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

find_package(SDL2_mixer CONFIG REQUIRED)
if(NOT SDL2_mixer_FOUND)
    message(FATAL_ERROR "SDL2_mixer not found")
endif()

if (APPLE)
    find_library(COREGRAPHICS_FRAMEWORK CoreGraphics)
    if(NOT COREGRAPHICS_FRAMEWORK)
        message(FATAL_ERROR "CoreGraphics framework not found")
    endif()
endif()

set(SOURCES
    main.cpp
    game.hpp game.cpp
    TextureManager.hpp
    TextureManager.cpp
    gameobject.hpp gameobject.cpp
    map.hpp
    map.cpp
    setting.cpp setting.h
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
    json.hpp
    timer.hpp
    ECS/WeaponComponent.hpp
    world.hpp world.cpp
    menu.h
    menu.cpp
    ECS/sword.h
    game_over.cpp
    game_over.hpp
    ingame_menu.cpp
    ingame_menu.h
    ECS/EnemyMovement.h
    ECS/Animation.cpp
    ECS/ColliderComponent.cpp
    ECS/KeyboardController.cpp
    ECS/ProjectileComponent.cpp
    ECS/SpriteComponent.cpp
    ECS/WeaponComponent.cpp
    ECS/EnemyMovement.cpp)

add_executable(TimelessJourneys ${SOURCES}
    ECS/armor.h ECS/armor.cpp
    inventoryscreen.cpp
    inventoryscreen.h
    ECS/InteractComponent.h
    chestScreen.cpp
    chestScreen.h)


if (APPLE)
    target_link_libraries(TimelessJourneys PRIVATE SDL2::SDL2 SDL2_image::SDL2_image SDL2_mixer::SDL2_mixer SDL2_ttf::SDL2_ttf ${COREGRAPHICS_FRAMEWORK} )
endif()

if (WIN32)
    target_link_libraries(TimelessJourneys PRIVATE SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf SDL2_mixer::SDL2_mixer)
endif()

set(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_definitions(TimelessJourneys PRIVATE ROOT_DIR="${ROOT_DIR}")
