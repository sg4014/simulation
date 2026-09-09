//
// Created by sergey on 9/9/26.
//

#ifndef SIMULATION_COLLISIONS_H
#define SIMULATION_COLLISIONS_H
#include "Body.h"
#include <vector>

bool collidesLeftWall(const Sim::Body& body);

bool collidesRightWall(const Sim::Body& body);

bool collidesTopWall(const Sim::Body& body);

bool collidesBottomWall(const Sim::Body& body);

bool collide(const Sim::Body& b1, const Sim::Body& b2);

void handleWallCollisions(std::vector<Sim::Body>& bodies);

void handleCollisionsBetweenBodies(std::vector<Sim::Body>& bodies);
#endif //SIMULATION_COLLISIONS_H