//
// Created by sergey on 9/9/26.
//

#ifndef SIMULATION_BODY_H
#define SIMULATION_BODY_H
#include "Logger.h"
#include "SFML/Graphics.hpp"
#include <cassert>

namespace Sim {
class Body : public sf::Drawable {
  explicit Body(std::string name,
                float radius,
                float mass,
                const sf::Font& font)
  // Logger* logger = nullptr)
    : m_shape{radius},
      m_nameText{font},
      m_name{std::move(name)},
      m_mass{mass} {
    // m_logger{logger}
    assert(radius > 0);
    assert(mass > 0);
    m_shape.setOrigin(m_shape.getGeometricCenter());
    initText();
  }

public:
  float getRadius() const { return m_shape.getRadius(); }

  sf::Vector2f getPosition() const { return m_shape.getPosition(); }

  void setPosition(sf::Vector2f pos) {
    m_shape.setPosition(pos);
    m_nameText.setPosition(pos);
  }

  void move(sf::Vector2f offset) {
    setPosition(getPosition() + offset);
  }

  void setFillColor(sf::Color color) { m_shape.setFillColor(color); }

  sf::Vector2f getVelocity() const { return m_velocity; }

  float getMass() const { return m_mass; }

  void setVelocity(sf::Vector2f velocity) { m_velocity = velocity; }

  void updatePosition(float dt) {
    setPosition(getPosition() + dt * m_velocity);
    // if (m_logger) {
    //   m_logger->log(std::format("dt = {}; Updated position: {}", dt, vecToString(m_shape.getPosition())));
    // }
  }

protected:
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
    target.draw(m_shape, states);
    target.draw(m_nameText);
  }

private:
  sf::CircleShape m_shape{};
  sf::Text m_nameText;
  std::string m_name{};
  sf::Vector2f m_velocity{};
  float m_mass{};
  // Logger* m_logger{};

  void initText() {
    m_nameText.setString(m_name);
    m_nameText.setCharacterSize(20);
    m_nameText.setOrigin(m_nameText.getLocalBounds().getCenter());
    m_nameText.setFillColor(sf::Color::White);
  }
};
}

#endif //SIMULATION_BODY_H