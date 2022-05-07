#include "inc.h"

class menuItem : public sf::Drawable {
  public:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
    virtual void update(const sf::Vector2i& mousPos) = 0;
    virtual void onClick() = 0;
    virtual bool getHighlighted() = 0;
    virtual ~menuItem() = default;
};

template <class Appearance, class Action>
class Button : public menuItem {
  public:
    bool highlighted;
    Appearance normalView, highlightedView;
    sf::Vector2f position;
    friend class itemUpdater;
    Button(const Appearance& normalView, const Appearance& highlightedView, const sf::Vector2f position) : highlighted(false), normalView(normalView), highlightedView(highlightedView), position(position) {
    }

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        states.transform.translate(position);
        target.draw(highlighted ? highlightedView : normalView, states);
    }

    void update(const sf::Vector2i& mousPos) override {
        highlighted = (highlighted ? highlightedView : normalView).getLocalBounds().contains(static_cast<sf::Vector2f>(mousPos) - position);
    }

    void onClick() override {
        if (highlighted) {
            Action::call();
        }
    }

    bool getHighlighted() override {
        return highlighted;
    }
};

template <class Appearance>
struct Info : public menuItem {
    public:
    bool highlighted;
    Appearance view;
    sf::Vector2f position;
    Info(const Appearance& view, const sf::Vector2f position) : highlighted(false), view(view), position(position) {
    }

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        states.transform.translate(position);
        target.draw(view, states);
    }

    void update(const sf::Vector2i& mousPos) override {
        highlighted = view.getLocalBounds().contains(static_cast<sf::Vector2f>(mousPos) - position);
    }

    void onClick() override {
    }

    bool getHighlighted() override {
        return highlighted;
    }
};

class Menu : public sf::Drawable {
    std::vector<std::shared_ptr<menuItem>> items;
    bool highlighted;
    sf::Vector2f position;

  public:
    Menu(const sf::Vector2f position) : highlighted(false), position(position) {}

    std::shared_ptr<menuItem> addItem(menuItem * newItem) {
        items.emplace_back(newItem);
        return items.back();
    }

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        states.transform.translate(position);
        for (const auto& button : items) {
            target.draw(*button, states);
        }
    }

    void update(const sf::Vector2i& mousPos) {
        highlighted = false;
        for (const auto& button : items) {
            button->update(mousPos - static_cast<sf::Vector2i>(position));
            highlighted = highlighted || button->getHighlighted();
        }
    }

    void onClick() {
        if (highlighted) {
            for (const auto& button : items) {
                button->onClick();
            }
        }
    }

    bool getHighlighted() {
        return highlighted;
    }
};

template <class Action>
std::shared_ptr<Button<sf::Text, Action>> addDefaultTextButton(Menu& menu, const std::string& str, float x, float y, sf::Color colorNormal = sf::Color::Blue, sf::Color colorHighlighted = sf::Color::Green) {
    return std::dynamic_pointer_cast<Button<sf::Text, Action>>(menu.addItem(new Button<sf::Text, Action>(makeText(str, colorNormal), makeText(str, colorHighlighted), sf::Vector2f(x, y))));
}

std::shared_ptr<Info<sf::Text>> addDefaultText(Menu& menu, const std::string& str, float x, float y, sf::Color color = sf::Color::Blue) {
    return std::dynamic_pointer_cast<Info<sf::Text>>(menu.addItem(new Info<sf::Text>(makeText(str, color), sf::Vector2f(x, y))));
}