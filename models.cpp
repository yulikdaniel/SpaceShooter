#include "models.h"

int Model::Part::getType() const {
    return type;
}
const sf::ConvexShape& Model::Part::getConvex() const {
    return convex;
}

float Model::getRealScale() const {
    return scale;
}

void Model::setRealScale(const float& newScale) {
    scale = newScale;
}

void Model::changeRealScale(const float& deltaScale) {
    scale += deltaScale;
    if (scale < 0.1f) {
        scale = 0.1f;
    }
    if (scale > 2.0f) {
        scale = 2.0f;
    }
}

void Model::switchCurrent(int newCurrent) {
    if (current != -1) {
        parts[current].convex.setOutlineColor(sf::Color::Magenta);
    }
    current = newCurrent;
    if (current != -1) {
        parts[current].convex.setOutlineColor(sf::Color::Green);
    }
}

int Model::getCurrent() const {
    return current;
}

void Model::setDrawConvex(bool newDrawConvex) {
    drawConvex = newDrawConvex;
}

const std::vector<Model::Part>& Model::getParts() const {
    return parts;
}

void Model::setName(const std::string& newName) {
    name = newName;
}

sf::Vector2u Model::loadTexture() {
    if (!modelTexture.loadFromFile("../models/" + name + "/" + name + ".png")) {
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

bool Model::serialize() {
    std::ofstream output;
    output.open("models/" + name + "/" + name + ".minfo"); // Open the file at models/X/X.minfo

    if (!output.is_open()) {
        return false;
    }

    output << scale << '\n'; // Serialize the parts for the model
    output << parts.size() << '\n';
    for (const auto& part : parts) {
        output << partTypes[part.type] << '\n' << part.convex.getPointCount() << '\n';
        for (int i = 0; i < part.convex.getPointCount(); ++i) {
            output << part.convex.getPoint(i).x << ' ' << part.convex.getPoint(i).y << ' ';
        }
        output << "\n\n";
    }

    return true;
}

void Model::deserialize() {
    std::ifstream fin;
    fin.open("../models/" + name + "/" + name + ".minfo");
    if (fin.is_open()) {
        std::cout << "Found existing model, reading...\n";
        std::cout.flush();
        fin >> scale;
        int sz;
        fin >> sz;
        parts.resize(sz);
        for (int i = 0; i < sz; ++i) {
            std::string partType;
            fin >> partType;
            if (std::find(partTypes.begin(), partTypes.end(), partType) != partTypes.end()) {
                parts[i].type = std::find(partTypes.begin(), partTypes.end(), partType) - partTypes.begin();
            } else {
                parts[i].type = 0;
            }
            int nm;
            fin >> nm;
            parts[i].convex.setPointCount(nm);
            parts[i].convex.setFillColor(sf::Color::Transparent);
            parts[i].convex.setOutlineThickness(5);
            parts[i].convex.setOutlineColor(sf::Color::Magenta);
            for (int j = 0; j < nm; ++j) {
                float x, y;
                fin >> x >> y;
                parts[i].convex.setPoint(j, sf::Vector2f(x, y));
            }
        }
    }
}

void Model::setShowScale(float newShowScale) {
    showScale = newShowScale;
    for (int i = 0; i < parts.size(); ++i) {
        parts[i].convex.setScale(showScale, showScale);
    }
    modelImage.setScale(showScale, showScale);
}

void Model::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(modelImage, states);
    if (drawConvex) {
        for (int i = 0; i < parts.size(); ++i) {
            if (i != current) {
                target.draw(parts[i].convex, states);
            }
        }
        if (current != -1) {
            target.draw(parts[current].convex, states);
        }
    }
}

void Model::addConvex() {
    parts.emplace_back();
    parts.back().convex.setScale(showScale, showScale);
    parts.back().convex.setFillColor(sf::Color::Transparent);
    parts.back().convex.setOutlineThickness(5);
    switchCurrent(parts.size() - 1);
}

void Model::removeCurrentConvex() {
    parts.erase(parts.begin() + current);
    switchCurrent(-1);
}

void Model::addPointToCurrentConvex(const sf::Vector2f& point) {
    parts[current].convex.setPointCount(parts[current].convex.getPointCount() + 1);
    parts[current].convex.setPoint(parts[current].convex.getPointCount() - 1, point);
}

void Model::setCurrentPartType(int newCurrentPartType) {
    parts[current].type = newCurrentPartType;
}

const Model::Part& Model::getCurrentPart() const {
    return parts[current];
}