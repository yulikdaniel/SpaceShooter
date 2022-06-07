#include "inc.h"
#include "models.h"

class BaseSprite : public sf::Drawable {
    Model model;
    sf::Vector2f position;
  public:
    BaseSprite(const std::string& modelName, sf::Vector2f newPosition) : position(newPosition) {
        model.setName(modelName);
        model.deserialize();
        model.loadTexture();
        model.setDrawConvex(false);
    }

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        target.draw(model, states);
    }
};

template <class Controller>
class Sprite : public BaseSprite {
    Controller controller;

  public:
    Sprite(const std::string& modelName, sf::Vector2f newPosition) : BaseSprite::BaseSprite(modelName, newPosition) {

    }
};