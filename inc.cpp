#include "inc.h"

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
std::istream& operator>>(std::istream& in, sf::Vector2f& v) {
    in >> v.x >> v.y;
    return in;
}
std::istream& operator>>(std::istream& in, sf::Vector2i& v) {
    in >> v.x >> v.y;
    return in;
}
std::istream& operator>>(std::istream& in, sf::Vector2u& v) {
    in >> v.x >> v.y;
    return in;
}

sf::Font myfont;

static sf::Font getDefaultFont() {
    return myfont;
}

sf::Text makeText(const std::string& str, sf::Color color) {
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