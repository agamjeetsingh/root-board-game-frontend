#include <iostream>

#include "include/constants/GameFont.h"
#include "include/events/EventBus.h"
#include "include/input/InputManager.h"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Window/VideoMode.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(sf::VideoMode::getDesktopMode().size), "My Game");
    sf::Clock clock;

    sf::Font font;
    if (!font.openFromFile(getPath(GameFont::TITLE))) {
        return 1;
    }

    sf::Text text{font, "Hello, World"};
    text.setFillColor(sf::Color::Black);

    text.setPosition({100.f, 100.f});
    text.setCharacterSize(96);

    EventBus event_bus;

    InputManager input_manager;

    while (window.isOpen()) {
        sf::Time deltaTime = clock.restart();
        float dt = std::min(deltaTime.asSeconds(), 0.033f);

        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (event->is<sf::Event::KeyPressed>()) {
                input_manager.keyPressed(event->getIf<sf::Event::KeyPressed>()->code);
            }

            if (event->is<sf::Event::KeyReleased>()) {
                input_manager.keyReleased(event->getIf<sf::Event::KeyReleased>()->code);
            }
        }

        window.clear(sf::Color::White);

        event_bus.execute();

        window.draw(text);

        window.display();
    }

    return 0;
}
