//
// Created by Agamjeet Singh on 29/03/2026.
//

#ifndef ROOT_BOARD_GAME_FRONTEND_TEXTBOX_H
#define ROOT_BOARD_GAME_FRONTEND_TEXTBOX_H

#include "Widget.h"
#include "hitbox/RectHitbox.h"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/Text.hpp"

class TextBox : public Widget {
public:
    TextBox(
        EventBus& event_bus,
        sf::Vector2f position,
        sf::Vector2f size,
        const sf::Font& font,
        const std::string& text,
        unsigned int characterSize = DEFAULT_CHAR_SIZE,
        sf::Color color = DEFAULT_TEXT_COLOR);

    void draw(sf::RenderTarget &target, sf::Vector2f parentPos) override;

    void setText(const std::string& newText);

    void setCharacterSize(unsigned int characterSize);

    void setColor(sf::Color color);

    sf::Text text;

    sf::Vector2f size;

    static unsigned int DEFAULT_CHAR_SIZE;

    static sf::Color DEFAULT_TEXT_COLOR;
};

#endif //ROOT_BOARD_GAME_FRONTEND_TEXTBOX_H