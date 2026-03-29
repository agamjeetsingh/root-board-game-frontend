//
// Created by Agamjeet Singh on 19/03/2026.
//

#ifndef ROOT_BOARD_GAME_FRONTEND_GAMEAPP_H
#define ROOT_BOARD_GAME_FRONTEND_GAMEAPP_H

#include <magic_enum.hpp>

#include "../events/EventBus.h"
#include "../events/MousePosition.h"
#include "../gui/TextBox.h"
#include "../gui/UIManager.h"
#include "../gui/Widget.h"
#include "../input/InputManager.h"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Window/VideoMode.hpp"

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };

class GameApp {
public:
    GameApp() : ui_manager(event_bus) {
        loadFonts();

        ui_manager.addChild(std::make_unique<TextBox>(
            sf::Vector2f{100, 100},
            sf::Vector2f{100, 100},
            fonts[GameFont::TITLE],
            "hey, welcome to root!"));
    }

    void run() {
        while (window.isOpen()) {
            sf::Time deltaTime = clock.restart();
            float dt = std::min(deltaTime.asSeconds(), 1.0f / MIN_FRAMERATE);

            pollEvents();

            ui_manager.newMousePosition(input_manager.getMousePosition());

            event_bus.execute();

            ui_manager.update(dt);

            window.clear(sf::Color::White);

            ui_manager.draw(window);

            window.display();
        }
    }

private:

    sf::RenderWindow window{sf::VideoMode(sf::VideoMode::getDesktopMode().size), "My Game"};

    sf::Clock clock;

    EventBus event_bus;

    InputManager input_manager;

    UIManager ui_manager;

    void pollEvents() {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            event->visit(overloaded{
                [&](const sf::Event::Closed&) {
                    window.close();
                },
                [&](const sf::Event::KeyPressed& e) {
                    input_manager.keyPressed(e.code);
                },
                [&](const sf::Event::KeyReleased& e) {
                    input_manager.keyReleased(e.code);
                },
                [&](const sf::Event::MouseMoved& e) {
                    input_manager.mouseMovedTo(e.position);
                },
                [&](const sf::Event::MouseButtonPressed& e) {
                    input_manager.mouseMovedTo(e.position);
                    input_manager.mouseButtonPressed(e.button);

                    event_bus.emit(e);
                },
                [&](const sf::Event::MouseButtonReleased& e) {
                    input_manager.mouseMovedTo(e.position);
                    input_manager.mouseButtonReleased(e.button);
                },
                [](auto&& e) {
                    /* Default handler for other events */
                }
            });
        }
    }

    std::unordered_map<GameFont, sf::Font> fonts;

    void loadFonts() {
        for (GameFont fontType: magic_enum::enum_values<GameFont>()) {
            sf::Font font;

            if (!font.openFromFile(getPath(fontType))) {
                throw std::runtime_error(
                    "Failed to load font type: " +
                    std::string(magic_enum::enum_name(fontType)) +
                    " at address: " +
                    getPath(fontType));
            }

            fonts[fontType] = font;
        }
    }

    float MIN_FRAMERATE = 30;
};

#endif //ROOT_BOARD_GAME_FRONTEND_GAMEAPP_H