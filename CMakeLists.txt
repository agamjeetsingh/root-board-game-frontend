cmake_minimum_required(VERSION 4.1)
project(root_board_game_frontend)

set(CMAKE_CXX_STANDARD 23)

include(FetchContent)

FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.12.0
)
FetchContent_MakeAvailable(nlohmann_json)

FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG master
)
set(SFML_BUILD_NETWORK OFF)
FetchContent_MakeAvailable(SFML)

add_executable(root_board_game_frontend main.cpp
        include/input/InputManager.h
        include/constants/GameFont.h
        include/events/Event.h
        include/events/EventBus.h
        src/events/EventBus.cpp
        include/events/Listener.h
        include/events/ListenerPriority.h
        include/game/GameApp.h
        src/events/Listener.cpp)
        src/events/Listener.cpp
        include/gui/Widget.h
        include/events/MousePosition.h
        include/gui/UIManager.h

target_link_libraries(root_board_game_frontend PRIVATE
        nlohmann_json::nlohmann_json
        SFML::Graphics
        SFML::Window
        SFML::System
        SFML::Audio
)