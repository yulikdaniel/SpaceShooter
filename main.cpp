#include "menu.h"
#include "levels.h"

int main() {
    const int WIDTH = 1200, HEIGHT = 800;

    Level level;
    level.setName("level1");
    level.deserialize();

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Model creator");
    window.setFramerateLimit(60);

    sf::RectangleShape background(sf::Vector2f(WIDTH, HEIGHT));
    background.setFillColor(sf::Color::White);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
            }
        }

        window.clear();
        window.draw(background);
        window.draw(level);
        window.display();
    }
    return 0;
}