//
// Created by Agamjeet Singh on 29/03/2026.
//

#ifndef ROOT_BOARD_GAME_FRONTEND_HITBOX_H
#define ROOT_BOARD_GAME_FRONTEND_HITBOX_H
#include "SFML/System/Vector2.hpp"

class Hitbox {
public:
    virtual ~Hitbox() = default;

    [[nodiscard]] virtual bool contains(sf::Vector2f point) const = 0;

    [[nodiscard]] virtual const sf::Vector2f& getPosition() const = 0;

    virtual void setPosition(sf::Vector2f newPosition) = 0;
};

#endif //ROOT_BOARD_GAME_FRONTEND_HITBOX_H