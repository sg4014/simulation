//
// Created by sergey on 9/9/26.
//

#ifndef SIMULATION_UTIL_H
#define SIMULATION_UTIL_H
#include "Body.h"
#include "SFML/Graphics.hpp"
#include <string>

std::string vecToString(sf::Vector2f vec);

sf::Vector2f getWindowCenter(const sf::RenderWindow& window);

[[maybe_unused]] void printPosition(const Sim::Body& body);
#endif //SIMULATION_UTIL_H
