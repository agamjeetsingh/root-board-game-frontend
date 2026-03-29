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
        const sf::Vector2f position,
        const sf::Vector2f size,
        const sf::Font& font,
        const std::string& text) :
    Widget(event_bus, std::make_unique<RectHitbox>(position, size)),
    text(font, text, 64),
    size(size) {
        this->text.setFillColor(sf::Color::Black);
    }

    void draw(sf::RenderTarget &target, sf::Vector2f parentPos) override {
        sf::RectangleShape rect{size};
        sf::Color c = sf::Color::Red;
        c.a = 40;
        rect.setFillColor(c);
        rect.setPosition(getAbsolutePosition(parentPos));
        target.draw(rect);
        text.setPosition(getAbsolutePosition(parentPos));
        target.draw(text);
    }

    TextBox& setText(const std::string& newText) {
        text.setString(newText);
        return *this;
    }

    sf::Text text;

    sf::Vector2f size;
};

#endif //ROOT_BOARD_GAME_FRONTEND_TEXTBOX_H