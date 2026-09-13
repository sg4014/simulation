//
// Created by sergey on 9/9/26.
//

#ifndef SIMULATION_UTIL_H
#define SIMULATION_UTIL_H
#include <cstring>

#include "Body.h"
#include "Random.h"
#include "SFML/Graphics.hpp"
#include <string>

namespace Sim {
std::string vecToString(sf::Vector2f vec);

sf::Vector2f getWindowCenter(const sf::RenderWindow& window);

[[maybe_unused]] void printPosition(const Body& body);

std::string getRandomName();

inline sf::Color getRandomColor(std::uint8_t opacity = 100) {
    return sf::Color{
        Random::get<std::uint8_t>(0, 255),
        Random::get<std::uint8_t>(0, 255),
        Random::get<std::uint8_t>(0, 255),
        opacity
    };
}

sf::Vector2f getRandomPosition(const Body& body);

inline sf::Vector2f arrToVec(const float arr[2]) {
    return {arr[0], arr[1]};
}

// The ImGui color {r, g, b} wheel requires floats from 0 to 1.
// But SFML requires std::uint8_t from 0 to 255.
sf::Color toSFMLColor(const float col[3]);

void resetParameters();
}
#endif //SIMULATION_UTIL_H