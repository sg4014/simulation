//
// Created by sergey on 9/11/26.
//

#ifndef SIMULATION_BODYUTIL_H
#define SIMULATION_BODYUTIL_H
#include "Body.h"
#include "SFML/System/Vector2.hpp"
namespace Sim {
/**
 * Get the vector pointing from source body to target body.
 *
 * @param source The body from which the direction is calculated
 * @param target The body towards which the direction is calculated
 * @return a unit vector in the direction of the other body.
 */
sf::Vector2f getDirection(const Body& source, const Body& target);

/**
 * Check if the position (e.g. of a mouse click) is "inside" the body.
 *
 * @param position a position on a screen
 * @param body a body
 * @return true if the position on the screen is covered by the body's shape.
 */
bool isWithinBody(sf::Vector2i position, const Body& body);

bool isWithinBody(sf::Vector2f position, const Body& body);
}
#endif //SIMULATION_BODYUTIL_H
