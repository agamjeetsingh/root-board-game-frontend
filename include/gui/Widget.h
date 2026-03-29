//
// Created by Agamjeet Singh on 20/03/2026.
//

#ifndef ROOT_BOARD_GAME_FRONTEND_WIDGET_H
#define ROOT_BOARD_GAME_FRONTEND_WIDGET_H
#include <map>
#include <memory>
#include <ranges>
#include <vector>
#include "../events/Listener.h"
#include "hitbox/Hitbox.h"
#include "SFML/Graphics/Rect.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Event.hpp"

struct MousePosition;

namespace sf {
    class RenderTarget;
}

class Widget {
public:
    Widget(EventBus& event_bus, std::unique_ptr<Hitbox> hitbox): hitbox(std::move(hitbox)), onClickListener(Listener::make_listener<sf::Event::MouseButtonPressed>([this](const sf::Event::MouseButtonPressed& e) {
        if (contains(sf::Vector2f(e.position))) {
            onClick();
        }
    }, event_bus)), onHoverListener(Listener::make_listener<MousePosition>([this](const MousePosition& e) {
        if (contains(sf::Vector2f(e.position))) {
            if (!isHovered) {
                onHoverEnter();
            }
            isHovered = true;
            onHover();
        } else {
            if (isHovered) {
                onHoverExit();
            }
            isHovered = false;
        }
    }, event_bus)) {}

    virtual ~Widget() = default;

    virtual void draw(sf::RenderTarget& target, sf::Vector2f parentPos) {
        recurseChildren(&Widget::draw, target, getAbsolutePosition(parentPos));
    }

    virtual void update(float dt) {}

    virtual void onClick() {
        recurseChildren(&Widget::onClick);
    }

    virtual void onHover() {
        recurseChildren(&Widget::onHover);
    }

    virtual void onHoverEnter() {}

    virtual void onHoverExit() {}

    [[nodiscard]] bool contains(sf::Vector2f point) const {
        return hitbox->contains(point);
    }

    void addChild(std::unique_ptr<Widget> child, int z = 0) {
        children_map[z].push_back(std::move(child));
    }

protected:
    std::unique_ptr<Hitbox> hitbox;

private:
    std::map<int, std::vector<std::unique_ptr<Widget>>, std::greater<>> children_map;

    Listener onClickListener;

    Listener onHoverListener;

    bool isHovered = false;

    template<typename F, typename... Args>
    void recurseChildren(F&& f, Args&&... args) const {
        auto invoker = [&](const std::unique_ptr<Widget>& child) {
            std::invoke(f, child.get(), args...);
        };

        for (auto&& children : children_map | std::views::values) {
            for (auto& child : children) {
                invoker(child);
            }
        }
    }

protected:

    [[nodiscard]] sf::Vector2f getAbsolutePosition(sf::Vector2f parentPos) const {
        return parentPos + hitbox->getPosition();
    }
};

#endif //ROOT_BOARD_GAME_FRONTEND_WIDGET_H