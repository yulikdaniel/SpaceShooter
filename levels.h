#include "models.h"
#include "sprites.h"
#include "control.h"

class Level : public sf::Drawable {
    std::string name;
    std::vector<std::unique_ptr<BaseSprite>> sprites;
  public:
    void setName(const std::string& newName) {
        name = newName;
    }

    void deserialize() {
        std::ifstream fin;
        fin.open("levels/" + name + ".linfo");
        std::cerr << fin.is_open() << std::endl;
        std::string type;
        while (fin >> type) {
            if (type == "sprite") {
                std::string modelName, controller;
                sf::Vector2f coords;
                fin >> modelName >> controller >> coords;
                if (controller == "player") {
                    sprites.emplace_back(new Sprite<PlayerController>(modelName, coords));
                }
            }
        }
    }

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {\
        for (int i = 0; i < sprites.size(); ++i) {
            target.draw(*sprites[i], states);
        }
    }
};
