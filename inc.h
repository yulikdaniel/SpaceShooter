#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <cmath>

std::ostream& operator<<(std::ostream& out, const sf::Vector2f& v);
std::ostream& operator<<(std::ostream& out, const sf::Vector2i& v);
std::ostream& operator<<(std::ostream& out, const sf::Vector2u& v);

std::istream& operator>>(std::istream& in, sf::Vector2f& v);
std::istream& operator>>(std::istream& in, sf::Vector2i& v);
std::istream& operator>>(std::istream& in, sf::Vector2u& v);

static sf::Font getDefaultFont();

sf::Text makeText(const std::string& str, sf::Color color = sf::Color::Blue);

void init();