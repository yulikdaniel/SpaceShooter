#include "inc.h"
#include "menu.h"
#include "models.h"

Model model;

std::string lastComment = "";
sf::Clock commentStartTime;
sf::Text text;

void setNewComment(const std::string& comment) {
    lastComment = comment;
    text.setString(lastComment);
    commentStartTime.restart();
}

float showScale, bigScale;

class SwitchPartTypeAction {
    static std::shared_ptr<Button<sf::Text, SwitchPartTypeAction>> toUpd;
    friend int main();
  public:
    static void call() {
        if (model.getCurrent() == -1) {
            setNewComment("No part chosen to change type for");
        } else {
            model.setCurrentPartType((model.getCurrentPart().getType() + 1) % model.partTypes.size());
            update();
        }
    }
    static void update() {
        // std::cout << ((model.getCurrent() == -1) ? -1 : model.getCurrentPart().getType()) << std::endl;
        std::string text = (model.getCurrent() == -1) ? "none" : model.partTypes[model.getCurrentPart().getType()];
        toUpd -> normalView = makeText(text, sf::Color::Blue);
        toUpd -> highlightedView = makeText(text, sf::Color::Green);
    }
};

class AddAction { // Static class used as action for Add new convex button
    static std::shared_ptr<Info<sf::Text>> toUpd;
    friend int main();
  public:
    static void call() {
        setNewComment("Adding new convex");
        model.addConvex();

        toUpd -> view = makeText("Number of parts: " + std::to_string(model.getParts().size())); // Update menu info
        SwitchPartTypeAction::update();
    }
};

std::shared_ptr<Info<sf::Text>> AddAction::toUpd;

class RemoveAction { // Static class used as action for Remove convex button
    static std::shared_ptr<Info<sf::Text>> toUpd;
    friend int main();
  public:
    static void call() {
        if (model.getCurrent() == -1) {
            setNewComment("No convex chosen to erase");
        } else {
            setNewComment("Removing current convex");
            model.removeCurrentConvex();
        }
        toUpd -> view = makeText("Number of parts: " + std::to_string(model.getParts().size())); // Update menu info
        SwitchPartTypeAction::update();
    }
};

std::shared_ptr<Info<sf::Text>> RemoveAction::toUpd;

class FinishAction { // Static class used as action for Done button
  public:
    static void call() {
        setNewComment("Done with current convex");
        model.switchCurrent(-1);
    }
};

class SaveAction { // Static class used as action for Save button
  public:
    static void call() {
        setNewComment("Saving");
        model.serialize();
    }
};

class SwitchScaleAction {
    static bool currentReal;
    static std::shared_ptr<Info<sf::Text>> textToUpd;
    static std::shared_ptr<Button<sf::Text, SwitchScaleAction>> buttonToUpd;

    friend int main();
  public:
    static void call() {
        if (currentReal) {
            setNewComment("Changing to amplified scale");
        } else {
            setNewComment("Changing to real scale");
        }
        currentReal = !currentReal;
        update();
    }

    static void update() {
        if (currentReal) {
            showScale = model.getRealScale();
        } else {
            showScale = bigScale;
        }
        model.setShowScale(showScale);
        textToUpd -> view = makeText("Current scale: " + std::to_string(static_cast<int>(std::round(100 * model.getRealScale()))) + "%");
        buttonToUpd -> normalView = makeText(currentReal ? "Switch to amplified scale" : "Switch to real scale", sf::Color::Blue);
        buttonToUpd -> highlightedView = makeText(currentReal ? "Switch to amplified scale" : "Switch to real scale", sf::Color::Green);
    }
};

bool SwitchScaleAction::currentReal;
std::shared_ptr<Info<sf::Text>> SwitchScaleAction::textToUpd;
std::shared_ptr<Button<sf::Text, SwitchScaleAction>> SwitchScaleAction::buttonToUpd;

std::shared_ptr<Button<sf::Text, SwitchPartTypeAction>> SwitchPartTypeAction::toUpd;

const int WIDTH = 1200, HEIGHT = 800;

int main() {
    std::cout << "If you wish to create a model, you should have prepared a square png image to create the model from.\n";
    std::cout << "If you wish to edit an existing model, simply put its name." << '\n';
    std::cout << "In any case, if the model's name is X, there should be a folder models/X containing X.png\n\n";

    std::cout << "Please enter the name of the model: ";
    std::cout.flush();
    std::string modelName;
    if (!(std::cin >> modelName)) {
        std::cout << "Could not read model name, exiting\n";
        exit(1);
    }
    init();
    
    model.setName(modelName);

    sf::Vector2u imgsize = model.loadTexture();
    bigScale = 800.0f / imgsize.x;

    model.deserialize();
    model.setDrawConvex(true);

    text = makeText("", sf::Color::Black);
    text.setPosition(sf::Vector2f(5, 0));

    sf::RectangleShape background(sf::Vector2f(WIDTH, HEIGHT));
    background.setFillColor(sf::Color::White);

    Menu menu(sf::Vector2f(800, 20));

    addDefaultTextButton<AddAction>(menu, "Add", 0, 0);
    addDefaultTextButton<RemoveAction>(menu, "Remove", 0, 40);
    addDefaultTextButton<FinishAction>(menu, "Done", 0, 80);
    addDefaultTextButton<SaveAction>(menu, "Save", 0, 120);
    SwitchScaleAction::buttonToUpd = addDefaultTextButton<SwitchScaleAction>(menu, "Switch to real scale", 0, 160);
    SwitchPartTypeAction::toUpd = addDefaultTextButton<SwitchPartTypeAction>(menu, "none", 0, 200);

    AddAction::toUpd = addDefaultText(menu, "Number of parts: " + std::to_string(model.getParts().size()), 0, 600);
    RemoveAction::toUpd = AddAction::toUpd;

    SwitchScaleAction::textToUpd = addDefaultText(menu, "", 0, 640);
    SwitchScaleAction::currentReal = true;
    SwitchScaleAction::update();

    setNewComment("Starting up");

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Model creator");
    window.setFramerateLimit(60);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseMoved) {
                menu.update(sf::Mouse::getPosition(window));
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                menu.onClick();

                if (model.getCurrent() != -1 && !menu.getHighlighted() && !model.getParts().empty()) {
                    model.addPointToCurrentConvex(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)) / showScale);
                }
            }
            if (event.type == sf::Event::KeyPressed) {
                for (int i = 0; i < 10; ++i) {
                    if (event.key.code == sf::Keyboard::Num0 + i) {
                        if (i == model.getCurrent()) {
                            setNewComment("Staying in convex #" + std::to_string(i));
                        } else {
                            if (i >= model.getParts().size()) {
                                setNewComment("Convex index out of range");
                            } else {
                                setNewComment("Switching to convex #" + std::to_string(i));
                                model.switchCurrent(i);
                            }
                        }
                        break;
                    }
                }
                if (event.key.code == sf::Keyboard::Equal) {
                    model.changeRealScale(0.01);
                    SwitchScaleAction::update();
                }
                if (event.key.code == sf::Keyboard::Dash) {
                    model.changeRealScale(-0.01);
                    SwitchScaleAction::update();
                }
                SwitchPartTypeAction::update();
            }
        }

        window.clear();
        window.draw(background);
        window.draw(menu);
        window.draw(model);

        if (!lastComment.empty()) {
            window.draw(text);
            if (commentStartTime.getElapsedTime() > sf::milliseconds(1500)) {
                lastComment = "";
            }
        }
        window.display();
    }
    return 0;
}