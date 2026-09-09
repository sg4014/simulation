//
// Created by sergey on 9/9/26.
//

#ifndef SIMULATION_COLLISIONS_H
#define SIMULATION_COLLISIONS_H
#include "Body.h"
#include <vector>
namespace Sim {
bool collidesLeftWall(const Body& body);

bool collidesRightWall(const Body& body);

bool collidesTopWall(const Body& body);

bool collidesBottomWall(const Body& body);

bool collide(const Body& b1, const Body& b2);

void handleWallCollisions(std::vector<Body>& bodies);

void handleCollisionsBetweenBodies(std::vector<Body>& bodies);

/**
 * Updates the bodies positions so they don't intersect (penetrate each other).
 *
 * @param b1 the first body.
 * @param b2 the second body.
 */
void updatePositionsToUndoIntersection(Body& b1, Body& b2);

/**
 * Updates velocities after collision.
 *
 * @param b1 the first body.
 * @param b2 the second body.
 */
void updateVelocitiesAfterCollision(Body& b1, Body& b2);
}
#endif //SIMULATION_COLLISIONS_H