//
// Created by sergey on 9/9/26.
//
#include "Collisions.h"

#include <complex>

#include "Constants.h"
#include "Body.h"

namespace Sim {
bool collidesLeftWall(const Body& body) {
  return body.getPosition().x <= body.getRadius();
}

bool collidesRightWall(const Body& body) {
  return body.getPosition().x + body.getRadius() >= Constants::wWidth;
}

bool collidesTopWall(const Body& body) {
  return body.getPosition().y <= body.getRadius();
}

bool collidesBottomWall(const Body& body) {
  return body.getPosition().y + body.getRadius() >= Constants::wHeight;
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
      body.setPosition({Constants::wWidth - body.getRadius(), body.getPosition().y});
    }

    if (collidesTopWall(body)) {
      body.setVelocity(body.getVelocity().componentWiseMul({1, -1}));
      body.setPosition({body.getPosition().x, body.getRadius()});
    } else if (collidesBottomWall(body)) {
      body.setVelocity(body.getVelocity().componentWiseMul({1, -1}));
      body.setPosition({body.getPosition().x, Constants::wHeight - body.getRadius()});
    }
  }
}

void updateVelocitiesAfterCollision(Body& b1, Body& b2) {
  // The velocity of each body is broken into two components: normal and tangent.
  // Normal velocity is the velocity along the axis, which is perpendicular to the common tangent.
  // Tangent velocity is the velocity along the common tangent.
  // The formulas for post-collision normal velocities
  // are derived from the impulse and kinetic energy conservation laws.
  // There's no energy dissipation.
  const auto m1 = b1.getMass();
  const auto m2 = b2.getMass();
  auto axis = b2.getPosition() - b1.getPosition();
  if (axis == sf::Vector2f{}) {
    // axis is 0, since the bodies have the same position.
    // Define a fallback axis.
    axis = sf::Vector2f{1, 0};
  }
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
  const auto distBetweenCenters = (b2.getPosition() - b1.getPosition()).length();
  const auto radiusSum = b1.getRadius() + b2.getRadius();
  const auto penetrationDepth = radiusSum - distBetweenCenters;
  const auto massSum = b1.getMass() + b2.getMass();
  const auto offsetLengthB1 = b2.getMass() / massSum * penetrationDepth;
  const auto offsetLengthB2 = b1.getMass() / massSum * penetrationDepth;
  const auto normalizedAxis = (b2.getPosition() - b1.getPosition()).normalized();
  const auto offsetVectorB1 = -1.f * normalizedAxis * offsetLengthB1;
  const auto offsetVectorB2 = normalizedAxis * offsetLengthB2;
  b1.move(offsetVectorB1);
  b2.move(offsetVectorB2);
}

bool areApproachingEachOther(const Body& b1, const Body& b2) {
  // Two bodies are approaching each other if and only if
  // the projection of their relative velocity (v2 - v1) onto the vector that
  // connects their positions (pos2 - pos1) is negative.
  // We'll calculate the dot product instead of projection,
  // because it has the same sign and is faster to calculate.
  // Reminder: projection of u onto v = dot(u, v) / length(v), where v != 0.
  // length(v) > 0, so it doesn't affect the sign.
  const auto relativeVelocity = b2.getVelocity() - b1.getVelocity();
  const auto axis = b1.getDirectionTo(b2);
  assert(axis.lengthSquared() != 0 && "Error: two bodies have the same position.");

  return relativeVelocity.dot(axis) < 0;
}


void handleCollisionsBetweenBodies(std::vector<Body>& bodies) {
  for (std::size_t i = 0; i < bodies.size(); ++i) {
    auto& b1 = bodies[i];

    for (std::size_t j = i + 1; j < bodies.size(); ++j) {
      auto& b2 = bodies[j];

      if (areApproachingEachOther(b1, b2) && collide(b1, b2)) {
        updatePositionsToUndoIntersection(b1, b2);
        updateVelocitiesAfterCollision(b1, b2);
      }
    }
  }
}
}