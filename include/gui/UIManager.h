//
// Created by Agamjeet Singh on 20/03/2026.
//

#ifndef ROOT_BOARD_GAME_FRONTEND_UIMANAGER_H
#define ROOT_BOARD_GAME_FRONTEND_UIMANAGER_H
#include <map>
#include <ranges>

#include "Widget.h"

class UIManager {
public:
    explicit UIManager(EventBus& event_bus): clickListener(Listener::make_listener<sf::Event::MouseButtonPressed>([](const sf::Event::MouseButtonPressed& e) {

    }, event_bus)), widget(std::make_unique<Widget>(event_bus, std::make_unique<RectHitbox>(sf::Vector2f{0, 0}, sf::Vector2f{0, 0}))) {}

    void addChild(std::unique_ptr<Widget> child) const {
        widget->addChild(std::move(child));
    }

    void draw(sf::RenderTarget& target) const {
        widget->draw(target, ORIGIN);
    }

    void newMousePosition(const sf::Vector2i pos) {
        mousePosition = pos;
    }

    void update(float dt) const {
        widget->update(dt);
    }

private:
    std::unique_ptr<Widget> widget;

    std::optional<Widget*> widgetUnderMouse;

    sf::Vector2i mousePosition;

    Listener clickListener;

    sf::Vector2f ORIGIN{0, 0};
};

#endif //ROOT_BOARD_GAME_FRONTEND_UIMANAGER_H