//
// Created by Agamjeet Singh on 29/03/2026.
//

#ifndef ROOT_BOARD_GAME_FRONTEND_RECTHITBOX_H
#define ROOT_BOARD_GAME_FRONTEND_RECTHITBOX_H
#include "Hitbox.h"
#include "SFML/Graphics/Rect.hpp"

class RectHitbox : public Hitbox {
public:
    RectHitbox(sf::Vector2f position, sf::Vector2f size): position(position), size(size) {}

    [[nodiscard]] bool contains(sf::Vector2f point) const override {
        return rect.contains(point);
    }

    [[nodiscard]] const sf::Vector2f& getPosition() const override {
        return position;
    }

    void setPosition(sf::Vector2f newPosition) override {
        position = newPosition;
    }

private:
    sf::Vector2f position;

    sf::Vector2f size;

    sf::FloatRect rect{position, size};
};

#endif //ROOT_BOARD_GAME_FRONTEND_RECTHITBOX_H