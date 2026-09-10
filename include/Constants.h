//
// Created by sergey on 9/9/26.
//

#ifndef SIMULATION_CONSTANTS_H
#define SIMULATION_CONSTANTS_H
#include "SFML/Graphics.hpp"

namespace Sim::Constants {
  constexpr int wWidth = 1600; // window width
  constexpr int wHeight = 1200; // window height
  constexpr sf::Vector2f wCenter {wWidth / 2.0f, wHeight / 2.0f};
}

#endif //SIMULATION_CONSTANTS_H
