#include "inc.h"
#include "convex.h"

class Model : public sf::Drawable {
  public:
    const std::vector<std::string> partTypes = {"none", "hull", "gun", "cockpit", "thrusters"};
    class Part {
        int type = 0;
        sf::ConvexShape convex;
        friend class Model;

      public:
        int getType() const;
        const sf::ConvexShape& getConvex() const;
    };
  private:
    std::string name;
    float scale, showScale;
    std::vector<Part> parts;
    sf::Texture modelTexture;
    sf::RectangleShape modelImage;
    bool drawConvex = false;
    int current = -1;
  public:
    float getRealScale() const;

    void setRealScale(const float& newScale);

    void changeRealScale(const float& deltaScale);

    void switchCurrent(int newCurrent);

    int getCurrent() const;

    void setDrawConvex(bool newDrawConvex);

    const std::vector<Part>& getParts() const;

    void setName(const std::string& newName);

    sf::Vector2u loadTexture();

    bool serialize();

    void deserialize();

    void setShowScale(float newShowScale);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void addConvex();

    void removeCurrentConvex();

    void addPointToCurrentConvex(const sf::Vector2f& point);

    void setCurrentPartType(int newCurrentPartType);

    const Part& getCurrentPart() const;
};