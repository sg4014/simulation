//
// Created by sergey on 9/9/26.
//
#include "Collisions.h"
#include "Constants.h"
#include "Body.h"

namespace Sim {
bool collidesLeftWall(const Body& body) {
  return body.getPosition().x <= body.getRadius();
}

bool collidesRightWall(const Body& body) {
  return body.getPosition().x + body.getRadius() >= Constants::windowWidth;
}

bool collidesTopWall(const Body& body) {
  return body.getPosition().y <= body.getRadius();
}

bool collidesBottomWall(const Body& body) {
  return body.getPosition().y + body.getRadius() >= Constants::windowHeight;
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

//------------------update velocities after collision---------------



void updateVelocitiesAfterCollision(Body& b1, Body& b2) {
  // The velocity of each body is broken into two components: normal and tangent.
  // Normal velocity is the velocity along the axis, which is perpendicular to the common tangent.
  // Tangent velocity is the velocity along the common tangent.
  // The formulas for post-collision normal velocities
  // are derived from the impulse and kinetic energy conservation laws.
  // There's no energy dissipation.
  const auto m1 = b1.getMass();
  const auto m2 = b2.getMass();
  const auto b1NormalBefore = b1.getVelocity().projectedOnto(axis);
  const auto b2NormalBefore = b2.getVelocity().projectedOnto(axis);

  const auto b1NormalAfter = (m1 - m2) / (m1 + m2) * b1NormalBefore
                             + (2 * m2) / (m1 + m2) * b2NormalBefore;

  const auto b2NormalAfter = b1NormalAfter + b1NormalBefore - b2NormalBefore;

  const auto b1TangentBefore = b1.getVelocity() - b1NormalBefore;
  const auto b2TangentBefore = b2.getVelocity() - b2NormalBefore;

  b1.setVelocity(b1NormalAfter + b1TangentBefore);
  b2.setVelocity(b2NormalAfter + b2TangentBefore);
}

void updatePositionsToUndoIntersection(Body& b1, Body& b2) {
  // When two bodies penetrate each other (intersect),
  // they are moved in the opposite directions along their line of centers.
  // The offset is proportional to mass: bigger mass => lesser offset, smaller mass => greater offset.
  const auto axis = b2.getPosition() - b1.getPosition();
  const auto distBetweenCenters = (b2.getPosition() - b1.getPosition()).length();
  const auto radiusSum = b1.getRadius() + b2.getRadius();
  const auto penetrationDepth = radiusSum - distBetweenCenters;
  const auto massSum = b1.getMass() + b2.getMass();
  const auto b1OffsetLength = b2.getMass() / massSum * penetrationDepth;
  const auto b2OffsetLength = b1.getMass() / massSum * penetrationDepth;
  const auto b1OffsetVector = -1.f * axis.normalized() * b1OffsetLength;
  const auto b2OffsetVector = axis.normalized() * b2OffsetLength;
  b1.move(b1OffsetVector);
  b2.move(b2OffsetVector);
}

void handleCollisionsBetweenBodies(std::vector<Body>& bodies) {
  for (std::size_t i = 0; i < bodies.size(); ++i) {
    auto& b1 = bodies[i];

    for (std::size_t j = i + 1; j < bodies.size(); ++j) {
      auto& b2 = bodies[j];

      const auto distBetweenCenters = (b2.getPosition() - b1.getPosition()).length();

      if (const auto radiusSum = b1.getRadius() + b2.getRadius();
        distBetweenCenters <= radiusSum) {
        //-------------------- handle penetration--------------
        updatePositionsToUndoIntersection(b1, b2);

        updateVelocitiesAfterCollision(b1, b2);
      }
    }
  }
}
}