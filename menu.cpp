#include "menu.h"

Menu::Menu(const sf::Vector2f position) : highlighted(false), position(position) {}

std::shared_ptr<menuItem> Menu::addItem(menuItem * newItem) {
    items.emplace_back(newItem);
    return items.back();
}

void Menu::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform.translate(position);
    for (const auto& button : items) {
        target.draw(*button, states);
    }
}

void Menu::update(const sf::Vector2i& mousPos) {
    highlighted = false;
    for (const auto& button : items) {
        button->update(mousPos - static_cast<sf::Vector2i>(position));
        highlighted = highlighted || button->getHighlighted();
    }
}

void Menu::onClick() {
    if (highlighted) {
        for (const auto& button : items) {
            button->onClick();
        }
    }
}

bool Menu::getHighlighted() {
    return highlighted;
}

std::shared_ptr<Info<sf::Text>> addDefaultText(Menu& menu, const std::string& str, float x, float y, sf::Color color) {
    return std::dynamic_pointer_cast<Info<sf::Text>>(menu.addItem(new Info<sf::Text>(makeText(str, color), sf::Vector2f(x, y))));
}