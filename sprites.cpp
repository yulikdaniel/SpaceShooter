#include "sprites.h"

BaseSprite::BaseSprite(const std::string& modelName, sf::Vector2f newPosition) : position(newPosition) {
    model.setName(modelName);
    model.deserialize();
    model.loadTexture();
    model.setDrawConvex(false);
}

void BaseSprite::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(model, states);
}