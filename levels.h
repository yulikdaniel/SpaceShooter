#include "sprites.h"
#include "control.h"

class Level : public sf::Drawable {
    std::string name;
    std::vector<std::unique_ptr<BaseSprite>> sprites;
  public:
    void setName(const std::string& newName);

    void deserialize();
    
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
