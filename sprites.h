#include "inc.h"
#include "models.h"

class BaseSprite : public sf::Drawable {
    Model model;
    sf::Vector2f position;
  public:
    BaseSprite(const std::string& modelName, sf::Vector2f newPosition);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

template <class Controller>
class Sprite : public BaseSprite {
    Controller controller;

  public:
    Sprite(const std::string& modelName, sf::Vector2f newPosition) : BaseSprite::BaseSprite(modelName, newPosition) {

    }
};