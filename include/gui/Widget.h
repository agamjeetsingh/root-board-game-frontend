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
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Event.hpp"
#include "../events/MousePosition.h"

namespace sf {
    class RenderTarget;
}

class Widget {
public:
    explicit Widget(std::unique_ptr<Hitbox> hitbox): hitbox(std::move(hitbox)) {}

    virtual ~Widget() = default;

    virtual void draw(sf::RenderTarget& target, sf::Vector2f parentPos) {
        recurseChildren(&Widget::draw, target, getAbsolutePosition(parentPos));
    }

    virtual void update(float dt) {
        recurseChildren(&Widget::update, dt);
    }

    virtual void onClick(const sf::Event::MouseButtonPressed& e) {
        for (auto&& children : children_map | std::views::values) {
            for (const auto& child : children) {
                if (child->contains(sf::Vector2f(e.position))) {
                    child->onClick(e);
                    return;
                }
            }
        }
    }

    bool mousePositionUpdate(const MousePosition& e) {
        bool blockedByChild = false;

        for (auto&& children : children_map | std::views::values) {
            for (const auto& child : children) {
                if (child->mousePositionUpdate(e)) {
                    blockedByChild = true;
                }
            }
        }

        if (!blockedByChild && contains(e.position)) {
            if (!isHovered) {
                isHovered = true;
                onHoverEnter(e);
            }
            onHover(e);
            return true;
        }

        if (isHovered) {
            isHovered = false;
            onHoverExit(e);
        }
        return false;
    }

    virtual void onHover(const MousePosition& e) {}

    virtual void onHoverEnter(const MousePosition& e) {}

    virtual void onHoverExit(const MousePosition& e) {}

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