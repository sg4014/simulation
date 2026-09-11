//
// Created by sergey on 9/9/26.
//

#ifndef SIMULATION_UTIL_H
#define SIMULATION_UTIL_H
#include "Body.h"
#include "SFML/Graphics.hpp"
#include <string>
namespace Sim {
std::string vecToString(sf::Vector2f vec);

sf::Vector2f getWindowCenter(const sf::RenderWindow& window);

[[maybe_unused]] void printPosition(const Body& body);
}

namespace Sim::UI {
sf::Vector2f arrToVec(const float arr[2]);

// The ImGui color {r, g, b} wheel requires floats from 0 to 1.
// But SFML requires std::uint8_t from 0 to 255.
sf::Color toSFMLColor(const float col[3]);

void resetParameters();
}
#endif //SIMULATION_UTIL_H
