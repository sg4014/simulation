//
// Created by sergey on 9/9/26.
//

#ifndef SIMULATION_BODY_H
#define SIMULATION_BODY_H
#include "SFML/Graphics.hpp"
#include <cassert>

namespace Sim {
class Body : public sf::Drawable {
public:
  explicit Body(std::string name,
                float radius,
                float mass,
                const sf::Font& font)
    : m_shape{radius},
      m_nameText{font},
      m_name{std::move(name)},
      m_mass{mass} {
    assert(radius > 0);
    assert(mass > 0);
    m_shape.setOrigin(m_shape.getGeometricCenter());
    initText();
  }

  std::string_view getName() const {
    return m_name;
  }

  float getRadius() const {
    return m_shape.getRadius();
  }

  void setRadius(float r) {
    m_shape.setRadius(r);
    m_shape.setOrigin(m_shape.getGeometricCenter());
  }

  sf::Vector2f getPosition() const {
    return m_shape.getPosition();
  }

  void setPosition(sf::Vector2f pos) {
    m_shape.setPosition(pos);
    m_nameText.setPosition(pos);
  }

  void move(sf::Vector2f offset) {
    setPosition(getPosition() + offset);
  }

  sf::Color getFillColor() const {
    return m_shape.getFillColor();
  }

  void setFillColor(sf::Color color) {
    m_shape.setFillColor(color);
  }

  sf::Vector2f getVelocity() const {
    return m_velocity;
  }

  float getMass() const {
    return m_mass;
  }

  void setVelocity(sf::Vector2f velocity) {
    m_velocity = velocity;
  }

  std::size_t getPointCount() const {
    return m_shape.getPointCount();
  }

  void setPointCount(std::size_t count) {
    m_shape.setPointCount(count);
  }

  void updatePosition(float dt) {
    setPosition(getPosition() + dt * m_velocity);
  }

  void setIsNameDisplayed(bool v) {
    m_isNameDisplayed = v;
  }

  bool isNameDisplayed() const {
    return m_isNameDisplayed;
  }

  bool isHidden() const {
    return m_isHidden;
  }

  void setHidden(bool hidden) {
    m_isHidden = hidden;
  }

  /**
   * Get the vector pointing from this body to the other body.
   *
   * @param other the other body.
   * @return a unit vector in the direction of the other body.
   */
  sf::Vector2f getDirectionTo(const Body& other) const {
    return (other.getPosition() - getPosition()).normalized();
  }

  template<typename T>
  bool contains(sf::Vector2<T> position) const {
    return (static_cast<sf::Vector2f>(position) - getPosition()).lengthSquared() <= getRadius() * getRadius();
  }

protected:
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
    target.draw(m_shape, states);
    if (m_isNameDisplayed) {
      target.draw(m_nameText);
    }
  }

private:
  sf::CircleShape m_shape{};
  sf::Text m_nameText;
  std::string m_name{};
  sf::Vector2f m_velocity{};
  float m_mass{};
  bool m_isNameDisplayed = true;
  bool m_isHidden = false;

  void initText() {
    m_nameText.setString(m_name);
    m_nameText.setCharacterSize(20);
    m_nameText.setOrigin(m_nameText.getLocalBounds().getCenter());
    m_nameText.setFillColor(sf::Color::White);
  }
};
}

#endif //SIMULATION_BODY_H