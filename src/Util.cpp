//
// Created by sergey on 9/9/26.
//

#include "Util.h"
#include "Body.h"
#include "SFML/Graphics.hpp"
#include <string>
#include <format>
#include <iostream>

namespace Sim {
std::string vecToString(sf::Vector2f vec) {
  return std::format("{{{}, {}}}", vec.x, vec.y);
}

sf::Vector2f getWindowCenter(const sf::RenderWindow& window) {
  return static_cast<sf::Vector2f>(window.getSize()).componentWiseDiv({2, 2});
}

[[maybe_unused]] void printPosition(const Body& body) {
  std::cout << vecToString(body.getPosition()) << '\n';
}
}

