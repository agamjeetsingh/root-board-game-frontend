//
// Created by Agamjeet Singh on 29/03/2026.
//

#include "../../include/gui/TextBox.h"

TextBox::TextBox(
    EventBus &event_bus,
    const sf::Vector2f position,
    const sf::Vector2f size,
    const sf::Font &font,
    const std::string &text,
    const unsigned int characterSize,
    const sf::Color color) :
    Widget(event_bus, std::make_unique<RectHitbox>(position, size)),
    text(font, text, characterSize),
    size(size) {
    this->text.setFillColor(color);
}

void TextBox::draw(sf::RenderTarget &target, sf::Vector2f parentPos)  {
    text.setPosition(getAbsolutePosition(parentPos));
    target.draw(text);
}

TextBox &TextBox::setText(const std::string &newText) {
    text.setString(newText);
    return *this;
}

unsigned int TextBox::DEFAULT_CHAR_SIZE = 64;

sf::Color TextBox::DEFAULT_TEXT_COLOR = sf::Color::Black;
