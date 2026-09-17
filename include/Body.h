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
    Body(sf::String name,
         float radius,
         float mass,
         const sf::Font& font)
        : m_shape{radius},
          m_nameText{font, std::move(name)},
          m_mass{mass} {
        assert(radius > 0);
        assert(mass > 0);
        m_shape.setOrigin(m_shape.getGeometricCenter());
        m_nameText.setCharacterSize(20);
        m_nameText.setOrigin(m_nameText.getLocalBounds().getCenter());
        m_nameText.setFillColor(sf::Color::Black);
    }

    [[nodiscard]] const sf::String& getName() const {
        return m_nameText.getString();
    }

    void setName(const sf::String& name) {
        m_nameText.setString(name);
        m_nameText.setOrigin(m_nameText.getLocalBounds().getCenter());
    }


    [[nodiscard]] float getRadius() const {
        return m_shape.getRadius();
    }

    void setRadius(float r) {
        m_shape.setRadius(r);
        m_shape.setOrigin(m_shape.getGeometricCenter());
    }

    [[nodiscard]] sf::Vector2f getPosition() const {
        return m_shape.getPosition();
    }

    void setPosition(sf::Vector2f pos) {
        m_shape.setPosition(pos);
        m_nameText.setPosition(pos);
    }

    void move(sf::Vector2f offset) {
        setPosition(getPosition() + offset);
    }

    [[nodiscard]] sf::Color getFillColor() const {
        return m_shape.getFillColor();
    }

    void setFillColor(sf::Color color) {
        m_shape.setFillColor(color);
    }

    [[nodiscard]] sf::Vector2f getVelocity() const {
        return m_velocity;
    }

    [[nodiscard]] float getMass() const {
        return m_mass;
    }

    void setVelocity(sf::Vector2f velocity) {
        m_velocity = velocity;
    }

    [[nodiscard]] std::size_t getPointCount() const {
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

    [[nodiscard]] bool isNameDisplayed() const {
        return m_isNameDisplayed;
    }

    [[nodiscard]] bool isHidden() const {
        return m_isHidden;
    }

    void setHidden(bool hidden) {
        m_isHidden = hidden;
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
    sf::Vector2f m_velocity{};
    float m_mass{};
    bool m_isNameDisplayed = true;
    bool m_isHidden = false;
};

inline sf::Vector2f getDirection(const Body& source, const Body& target) {
    const auto posDiff = target.getPosition() - source.getPosition();
    return posDiff.lengthSquared() == 0
               ? sf::Vector2f{1, 0}
               : posDiff.normalized();
}

inline bool isWithinBody(sf::Vector2f position, const Body& body) {
    return (position - body.getPosition()).lengthSquared() <= body.getRadius() * body.getRadius();
}

inline bool isWithinBody(sf::Vector2i position, const Body& body) {
    return isWithinBody(static_cast<sf::Vector2f>(position), body);
}
}

#endif //SIMULATION_BODY_H