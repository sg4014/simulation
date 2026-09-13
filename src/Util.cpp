//
// Created by sergey on 9/9/26.
//

#include "Util.h"
#include "Body.h"
#include "Global.h"
#include "Constants.h"
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

std::string getRandomName() {
    constexpr std::array adjectives{"funky", "elated", "pitiful", "joyful", "sad", "little", "cranky",
                                    "drowsy",
                                    "shiny", "thoughtful", "silly"};

    constexpr std::array firstNames{"caspar", "dmitry", "rak", "wlad", "michael", "johnny", "vitaly", "suzy",
                                    "chuan"};

    const auto adjectiveIndex = Random::get<int>(0, std::ssize(adjectives) - 1);
    const auto nameIndex = Random::get<int>(0, std::ssize(firstNames) - 1);
    return std::string{adjectives[adjectiveIndex]} + " " + std::string{firstNames[nameIndex]};
}

sf::Vector2f getRandomPosition(const Body& body) {
    // In simple words: assume the Circle is a little larger than it is when picking its position to avoid spawning beyond a wall.
    // Ceil, not floor, the radius to ensure the circle won't spawn with its edge beyond a wall.
    // For example, if the radius is 10.89, and we floor the radius, and the position is {radius, radius},
    // the circle's topmost and leftmost points will be beyond the top and left walls respectively by 0.89.
    const auto radius = static_cast<int>(std::ceil(body.getRadius()));
    return {
        static_cast<float>(Random::get(radius, Constants::wWidth - radius)),
        static_cast<float>(Random::get(radius, Constants::wHeight - radius))
    };
}

// The ImGui color {r, g, b} wheel requires floats from 0 to 1.
// But SFML requires std::uint8_t from 0 to 255.
sf::Color toSFMLColor(const float col[3]) {
    return sf::Color{
        static_cast<std::uint8_t>(col[0] * 255),
        static_cast<std::uint8_t>(col[1] * 255),
        static_cast<std::uint8_t>(col[2] * 255)
    };
}

void resetParameters() {
    auto& activeBody = g_bodies[g_activeBodyIdx];
    g_velocity[0] = activeBody.getVelocity().x;
    g_velocity[1] = activeBody.getVelocity().y;
    g_isDisplayName = activeBody.isNameDisplayed();
    g_isHidden = activeBody.isHidden();
    g_bodySides = static_cast<int>(activeBody.getPointCount());
    g_radius = activeBody.getRadius();
    const auto sfmlColor = activeBody.getFillColor();
    g_imguiColor[0] = static_cast<float>(sfmlColor.r / 255);
    g_imguiColor[1] = static_cast<float>(sfmlColor.g / 255);
    g_imguiColor[2] = static_cast<float>(sfmlColor.b / 255);
}
}

