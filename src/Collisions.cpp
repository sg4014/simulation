//
// Created by sergey on 9/9/26.
//
#include "Collisions.h"
#include "Constants.h"
#include "Body.h"

namespace Sim {
using namespace Constants;

bool collidesLeftWall(const Body& body) {
  return body.getPosition().x < body.getRadius();
}

bool collidesRightWall(const Body& body) {
  return body.getPosition().x + body.getRadius() > windowWidth;
}

bool collidesTopWall(const Body& body) {
  return body.getPosition().y < body.getRadius();
}

bool collidesBottomWall(const Body& body) {
  return body.getPosition().y + body.getRadius() > windowHeight;
}

// Returns true if the bodies collide.
bool collide(const Body& b1, const Body& b2) {
  const auto distBetweenCentersSquared = (b2.getPosition() - b1.getPosition()).lengthSquared();
  const auto radiusSum = b1.getRadius() + b2.getRadius();
  return distBetweenCentersSquared <= radiusSum * radiusSum;
}

void handleWallCollisions(std::vector<Body>& bodies) {
  for (auto& body : bodies) {
    if (collidesLeftWall(body)) {
      body.setVelocity(body.getVelocity().componentWiseMul({-1, 1}));
      body.setPosition({body.getRadius(), body.getPosition().y});
    } else if (collidesRightWall(body)) {
      body.setVelocity(body.getVelocity().componentWiseMul({-1, 1}));
      body.setPosition({Constants::windowWidth - body.getRadius(), body.getPosition().y});
    }

    if (collidesTopWall(body)) {
      body.setVelocity(body.getVelocity().componentWiseMul({1, -1}));
      body.setPosition({body.getPosition().x, body.getRadius()});
    } else if (collidesBottomWall(body)) {
      body.setVelocity(body.getVelocity().componentWiseMul({1, -1}));
      body.setPosition({body.getPosition().x, Constants::windowHeight - body.getRadius()});
    }
  }
}

void handleCollisionsBetweenBodies(std::vector<Body>& bodies) {
  for (std::size_t i = 0; i < bodies.size(); ++i) {
    auto& b1 = bodies[i];

    for (std::size_t j = i + 1; j < bodies.size(); ++j) {
      auto& b2 = bodies[j];

      // vector from current's to other's center
      const auto axis = b2.getPosition() - b1.getPosition();

      // check collisions
      const auto distBetweenCenters = (b2.getPosition() - b1.getPosition()).length();
      if (const auto radiusSum = b1.getRadius() + b2.getRadius();
        distBetweenCenters <= radiusSum) {
        // handle penetration
        const auto penetrationDepth = radiusSum - distBetweenCenters;
        const auto massSum = b1.getMass() + b2.getMass();
        const auto b1OffsetLength = b2.getMass() / massSum * penetrationDepth;
        const auto b2OffsetLength = b1.getMass() / massSum * penetrationDepth;
        const auto b1OffsetVector = -1.f * axis.normalized() * b1OffsetLength;
        const auto b2OffsetVector = axis.normalized() * b2OffsetLength;
        b1.move(b1OffsetVector);
        b2.move(b2OffsetVector);

        // calculate normal and tangent velocities (relative to common tangent)
      }

      // if collide:
      // ==== update current ====
      // 1. reset positions

      // 2. calculate normal and tangent velocities (relative to common tangent)
      const auto normalVelocity = b1.getVelocity().projectedOnto(axis);
      const auto tangentVelocity = b1.getVelocity() - normalVelocity;
      // calculate new normal velocity based on formula derived from the conservation laws

      // ==== update other ======

      // TODO: do the math
      // if collide(current, other):
      //  reset positions so the bodies are not intersecting
      //  calculate normal velocities introduced by collision (use the conservation laws of impulse and kinetic energy)
      //
      //  normalVelocity = projection of velocity on the line of centers
      //  tangent velocity = velocity - normalVelocity
      //
      //  newNormalVelocityA = normalVelocity *
      //  newVelocity = tangentVelocity + newNormalVelocity
    }
  }
}

}
