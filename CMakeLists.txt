cmake_minimum_required(VERSION 3.5)
project(TimelessJourneys LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 CONFIG REQUIRED)
if(NOT SDL2_FOUND)

message(FATAL_ERROR "SDL2 not found")
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
    TextureManager.cpp)


add_executable(TimelessJourneys ${SOURCES})

target_link_libraries(TimelessJourneys PRIVATE SDL2_image::SDL2_image ${SDL2_LIBRARIES})

set(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_definitions(TimelessJourneys PRIVATE ROOT_DIR="${ROOT_DIR}")

