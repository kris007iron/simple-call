cmake_minimum_required(VERSION 3.28)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)
FetchContent_MakeAvailable(SFML)
add_executable(main
    src/main.cpp
    src/game/Game.cpp
    src/map/Map.cpp
    src/map/map_renderer/MapRenderer.cpp
    src/enemy/Enemy.cpp
    src/enemy/EnemyManager.cpp
    src/map/exit_renderer/ExitRenderer.cpp
    src/map/respawn_renderer/RespawnRenderer.cpp
)

target_compile_features(main PRIVATE cxx_std_20)

target_link_libraries(main PRIVATE
    SFML::Graphics
    SFML::Audio
)

set_target_properties(main PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

add_custom_command(
    TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/src/images
            $<TARGET_FILE_DIR:main>/images
)

add_custom_command(
    TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/src/Resources
            $<TARGET_FILE_DIR:main>/Resources
)

set_property(TARGET main PROPERTY
    VS_DEBUGGER_WORKING_DIRECTORY
    $<TARGET_FILE_DIR:main>
)
