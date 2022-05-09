#include "inc.h"

class Model : public sf::Drawable {
  public:
    // static const std::vector<std::string> partType = {"Gun", "Hull"};
  private:
    std::string name;
    float scale, showScale;
    std::vector<sf::ConvexShape> convexes;
    sf::Texture modelTexture;
    sf::RectangleShape modelImage;
    bool drawConvex = false;
    int current = -1;
  public:
    float getRealScale() {
        return scale;
    }

    void setRealScale(const float& newScale) {
        scale = newScale;
    }

    void changeRealScale(const float& deltaScale) {
        scale += deltaScale;
        if (scale < 0.1f) {
            scale = 0.1f;
        }
        if (scale > 2.0f) {
            scale = 2.0f;
        }
    }

    void switchCurrent(int newCurrent) {
        if (current != -1) {
            convexes[current].setOutlineColor(sf::Color::Magenta);
        }
        current = newCurrent;
        if (current != -1) {
            convexes[current].setOutlineColor(sf::Color::Green);
        }
    }

    int getCurrent() const {
        return current;
    }

    void setDrawConvex(bool newDrawConvex) {
        drawConvex = newDrawConvex;
    }

    const std::vector<sf::ConvexShape>& getConvexes() const {
        return convexes;
    }

    void setName(const std::string& newName) {
        name = newName;
    }

    sf::Vector2u loadTexture() {
        if (!modelTexture.loadFromFile("models/" + name + "/" + name + ".png")) {
            std::cerr << "Error loading image from path " << "models/" + name + "/" + name + ".png" << "\n";
            exit(1);
        }

        sf::Vector2u imgsize = modelTexture.getSize();
        if (imgsize.x != imgsize.y) {
            std::cerr << "Sorry, we do not support non-square images yet\n";
            std::cerr << "Size is " << imgsize << "\n";
            exit(1);
        }

        modelImage.setTexture(&modelTexture);
        modelImage.setSize(static_cast<sf::Vector2f>(imgsize));
    
        return imgsize;
    }

    bool serialize() {
        std::ofstream output;
        output.open("models/" + name + "/" + name + ".minfo"); // Open the file at models/X/X.minfo

        if (!output.is_open()) {
            return false;
        }

        output << scale << '\n'; // Serialize the convexes for the model
        output << convexes.size() << '\n';
        for (const auto& convex : convexes) {
            output << convex.getPointCount() << '\n';
            for (int i = 0; i < convex.getPointCount(); ++i) {
                output << convex.getPoint(i).x << ' ' << convex.getPoint(i).y << ' ';
            }
            output << "\n\n";
        }

        return true;
    }

    void deserialize() {
        std::ifstream fin;
        fin.open("models/" + name + "/" + name + ".minfo");
        if (fin.is_open()) {
            std::cout << "Found existing model, reading...\n";
            std::cout.flush();
            fin >> scale;
            int sz;
            fin >> sz;
            convexes.resize(sz);
            for (int i = 0; i < sz; ++i) {
                int nm;
                fin >> nm;
                convexes[i].setPointCount(nm);
                convexes[i].setFillColor(sf::Color::Transparent);
                convexes[i].setOutlineThickness(5);
                convexes[i].setOutlineColor(sf::Color::Magenta);
                for (int j = 0; j < nm; ++j) {
                    float x, y;
                    fin >> x >> y;
                    convexes[i].setPoint(j, sf::Vector2f(x, y));
                }
            }
        }
    }

    void setShowScale(float newShowScale) {
        showScale = newShowScale;
        for (int i = 0; i < convexes.size(); ++i) {
            convexes[i].setScale(showScale, showScale);
        }
        modelImage.setScale(showScale, showScale);
    }

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        target.draw(modelImage, states);
        for (int i = 0; i < convexes.size(); ++i) {
            if (i != current) {
                target.draw(convexes[i]);
            }
        }
        if (current != -1) {
            target.draw(convexes[current]);
        }
    }

    void addConvex() {
        convexes.emplace_back();
        convexes.back().setScale(showScale, showScale);
        convexes.back().setFillColor(sf::Color::Transparent);
        convexes.back().setOutlineThickness(5);
        switchCurrent(convexes.size() - 1);
    }

    void removeCurrentConvex() {
        convexes.erase(convexes.begin() + current);
        switchCurrent(-1);
    }

    void addPointToCurrentConvex(const sf::Vector2f& point) {
        convexes[current].setPointCount(convexes[current].getPointCount() + 1);
        convexes[current].setPoint(convexes[current].getPointCount() - 1, point);
    }
};