#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <cmath>

std::ostream& operator<<(std::ostream& out, const sf::Vector2f& v) {
    out << "Vector2f(" << v.x << ", " << v.y << ")";
    return out;
}
std::ostream& operator<<(std::ostream& out, const sf::Vector2i& v) {
    out << "Vector2i(" << v.x << ", " << v.y << ")";
    return out;
}
std::ostream& operator<<(std::ostream& out, const sf::Vector2u& v) {
    out << "Vector2u(" << v.x << ", " << v.y << ")";
    return out;
}

sf::Font myfont;

sf::Text makeText(const std::string& str, sf::Color color = sf::Color::Blue) {
    sf::Text text(str, myfont, 30);
    text.setFillColor(color);
    return text;
}

bool uninitialized = true;
void init() {
    if (uninitialized) {
        std::cerr << "Initializing...\n";
        myfont.loadFromFile("arial.ttf");
        uninitialized = false;
    }
}