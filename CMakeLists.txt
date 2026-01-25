cmake_minimum_required(VERSION 3.28)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)
FetchContent_MakeAvailable(SFML)

add_executable(main src/main.cpp "src/game/Game.cpp" "src/map/Map.cpp" "src/map/map_renderer/MapRenderer.cpp" "src/enemy/Enemy.cpp" "src/enemy/EnemyManager.cpp" "src/map/exit_renderer/ExitRenderer.cpp" "src/map/respawn_renderer/RespawnRenderer.cpp")

target_link_libraries(main PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
)
