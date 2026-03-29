//
// Created by Agamjeet Singh on 29/03/2026.
//

#ifndef ROOT_BOARD_GAME_FRONTEND_TEXTBOX_H
#define ROOT_BOARD_GAME_FRONTEND_TEXTBOX_H

#include "Widget.h"
#include "SFML/Graphics/Font.hpp"
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
    Widget(event_bus, position, size), text(font, text, 64) {
        this->text.setFillColor(sf::Color::Black);
    }

    void draw(sf::RenderTarget &target, sf::Vector2f parentPos) override {
        text.setPosition(getAbsolutePosition(parentPos));
        target.draw(text);
    }

    TextBox& setText(const std::string& newText) {
        text.setString(newText);
        return *this;
    }

    sf::Text text;
};

#endif //ROOT_BOARD_GAME_FRONTEND_TEXTBOX_H