//
// Created by Agamjeet Singh on 19/03/2026.
//

#ifndef ROOT_BOARD_GAME_FRONTEND_GAMEAPP_H
#define ROOT_BOARD_GAME_FRONTEND_GAMEAPP_H

#include <iostream>

#include "../events/EventBus.h"
#include "../input/InputManager.h"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Window/VideoMode.hpp"

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };

class GameApp {
public:
    GameApp() : ui_manager(event_bus) {
        if (!titleFont.openFromFile(getPath(GameFont::TITLE))) {
            throw std::runtime_error("Failed to load title font");
        }
        if (!textFont.openFromFile(getPath(GameFont::BODY))) {
            throw std::runtime_error("Failed to load body font");
        }
    }

    void run() {
        while (window.isOpen()) {
            sf::Time deltaTime = clock.restart();
            float dt = std::min(deltaTime.asSeconds(), 0.033f);

            pollEvents();

            ui_manager.newMousePosition(input_manager.getMousePosition());

            ui_manager.update(dt);

            window.clear(sf::Color::White);

            ui_manager.draw(window);

            event_bus.execute();

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
};

#endif //ROOT_BOARD_GAME_FRONTEND_GAMEAPP_H