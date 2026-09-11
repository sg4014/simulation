//
// Created by sergey on 9/11/26.
//
#include "BodyUtil.h"
#include "Body.h"
#include "SFML/System/Vector2.hpp"

namespace Sim {

sf::Vector2f getDirection(const Body& source, const Body& target) {
    return (target.getPosition() - source.getPosition()).normalized();
}

bool isWithinBody(sf::Vector2f position, const Body& body) {
    return (position - body.getPosition()).lengthSquared() <= body.getRadius() * body.getRadius();
}

bool isWithinBody(sf::Vector2i position, const Body& body) {
    return isWithinBody(static_cast<sf::Vector2f>(position), body);
}
}
