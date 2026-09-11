//
// Created by sergey on 9/9/26.
//

#include "Util.h"
#include "Body.h"
#include "Global.h"
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

namespace Sim::UI {
sf::Vector2f arrToVec(const float arr[2]) {
    return {arr[0], arr[1]};
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

