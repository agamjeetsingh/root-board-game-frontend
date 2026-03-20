//
// Created by Agamjeet Singh on 19/03/2026.
//

#ifndef ROOT_BOARD_GAME_FRONTEND_GAMEAPP_H
#define ROOT_BOARD_GAME_FRONTEND_GAMEAPP_H

#include "../events/EventBus.h"
#include "../input/InputManager.h"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Window/VideoMode.hpp"

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };

class GameApp {
public:
    void run() {
        while (window.isOpen()) {
            sf::Time deltaTime = clock.restart();
            float dt = std::min(deltaTime.asSeconds(), 0.033f);

            pollEvents();

            window.clear(sf::Color::White);

            event_bus.execute();

            window.display();
        }
    }

private:

    sf::RenderWindow window{sf::VideoMode(sf::VideoMode::getDesktopMode().size), "My Game"};

    sf::Clock clock;

    EventBus event_bus;

    void pollEvents() {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            event->visit(overloaded{
                [&](const sf::Event::Closed&) {
                    window.close();
                },
                [&](const sf::Event::KeyPressed& e) {
                    InputManager::getInstance().keyPressed(e.code);
                },
                [&](const sf::Event::KeyReleased& e) {
                    InputManager::getInstance().keyReleased(e.code);
                },
                [](auto&&) { /* Default handler for other events */ }
            });
        }
    }
};

#endif //ROOT_BOARD_GAME_FRONTEND_GAMEAPP_H