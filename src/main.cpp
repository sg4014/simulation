#include <utility>

#include "Logger.h"
#include "SFML/Graphics.hpp"

namespace {
// the gravity constant
// constexpr float G = 6.6743e-11;
constexpr float G = 1;
constexpr int windowWidth = 1000;
constexpr int windowHeight = 800;

sf::Font font;

std::string vecToString(sf::Vector2f vec) {
  return std::format("{{{}, {}}}", vec.x, vec.y);
}

class Body : public sf::Drawable {
public:
  explicit Body(float radius,
                float mass,
                std::string name,
                Logger* logger = nullptr)
    : m_shape{radius},
      m_mass{mass},
      m_name{std::move(name)},
      m_logger{logger} {
    assert(radius > 0);
    assert(mass > 0);
    m_shape.setOrigin(m_shape.getGeometricCenter());
    m_nameText.setString(m_name);
    m_nameText.setCharacterSize(20);
    m_nameText.setOrigin(m_nameText.getLocalBounds().getCenter());
    m_nameText.setFillColor(sf::Color::White);
  }

  float getRadius() const { return m_shape.getRadius(); }

  float getMass() const { return m_mass; }

  sf::Vector2f getPosition() const { return m_shape.getPosition(); }

  void setPosition(sf::Vector2f pos) {
    m_shape.setPosition(pos);
    m_nameText.setPosition(pos);
  }

  void setFillColor(sf::Color color) { m_shape.setFillColor(color); }

  void setResultant(sf::Vector2f resultant) { m_resultant = resultant; }

  sf::Vector2f getVelocity() const { return m_velocity; }

  void setVelocity(sf::Vector2f velocity) { m_velocity = velocity; }

  void updateDerivatives(float dt) {
    m_acceleration = m_resultant.componentWiseDiv({m_mass, m_mass});
    m_velocity += m_acceleration * dt;
  }

  void updatePosition(float dt) {
    setPosition(getPosition() + dt * m_velocity);
    if (m_logger) {
      m_logger->log(std::format("dt = {}; Updated position: {}", dt, vecToString(m_shape.getPosition())));
    }
  }

  //
  // void bounceHorizontally() {
  //   m_velocity.x = -m_velocity.x;
  // }
  //
  // void bounceVertically() { m_velocity.y = -m_velocity.y; }

protected:
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
    target.draw(m_shape, states);
    target.draw(m_nameText);
  }

private:
  sf::Vector2f m_resultant{};
  sf::CircleShape m_shape{};
  sf::Vector2f m_velocity{};
  sf::Vector2f m_acceleration{};
  float m_mass{};
  std::string m_name{};
  sf::Text m_nameText{font};
  Logger* m_logger{};
};


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

void updateBodies(std::vector<Body>& bodies, float dt) {
  for (std::size_t i = 0; i < bodies.size(); ++i) {
    sf::Vector2f resultant{};

    for (std::size_t j = 0; j < bodies.size(); ++j) {
      if (i == j)
        continue;

      // calculate gravity (F = G * m1 * m2 / distanceSquared), where R = abs(pos2 - pos1)
      const float distanceSquared = (bodies[j].getPosition() - bodies[i].getPosition()).lengthSquared();
      const auto forceAbs = G * bodies[i].getMass() * bodies[j].getMass() / distanceSquared;
      const auto forceDirection = (bodies[j].getPosition() - bodies[i].getPosition()).normalized();
      const sf::Vector2f force = forceAbs * forceDirection;
      resultant += force;
    }

    bodies[i].setResultant(resultant);
    bodies[i].updateDerivatives(dt);
    bodies[i].updatePosition(dt);
  }
}

void handleWallCollisions(std::vector<Body>& bodies) {
  for (auto& body : bodies) {
    if (collidesLeftWall(body)) {
      body.setVelocity(body.getVelocity().componentWiseMul({-1, 1}));
      body.setPosition({body.getRadius(), body.getPosition().y});
    } else if (collidesRightWall(body)) {
      body.setVelocity(body.getVelocity().componentWiseMul({-1, 1}));
      body.setPosition({windowWidth - body.getRadius(), body.getPosition().y});
    }

    if (collidesTopWall(body)) {
      body.setVelocity(body.getVelocity().componentWiseMul({1, -1}));
      body.setPosition({body.getPosition().x, body.getRadius()});
    }
    else if (collidesBottomWall(body)) {
      body.setVelocity(body.getVelocity().componentWiseMul({1, -1}));
      body.setPosition({body.getPosition().x, windowHeight - body.getRadius()});
    }
  }
}

void handleCollisionsBetweenBodies(std::vector<Body>& bodies) {
  for (std::size_t i = 0; i < bodies.size(); ++i) {
    auto& current = bodies[i];

    for (std::size_t j = i + 1; j < bodies.size(); ++j) {
      auto& other = bodies[j];

      // if collide(current, other):
      //
    }
  }
}

void renderBodies(const std::vector<Body>& bodies, sf::RenderWindow& window) {
  for (const auto& body : bodies) {
    window.draw(body);
  }
}

sf::Vector2f getWindowCenter(const sf::RenderWindow& window) {
  return static_cast<sf::Vector2f>(window.getSize()).componentWiseDiv({2, 2});
}

[[maybe_unused]] void printPosition(const Body& body) {
  std::cout << vecToString(body.getPosition()) << '\n';
}
}

int main() {
  // TODO: add imgui

  sf::RenderWindow window{sf::VideoMode{{windowWidth, windowHeight}}, "Simulation"};
  window.setVerticalSyncEnabled(true);

  //--------------------Font-----------------------------------------
  if (!font.openFromFile("resources/arial.ttf")) {
    std::cerr << "Couldn't open font file resources/arial.ttf\n";
    std::exit(1);
  }

  //--------------------DEFINE BODIES--------------------------------
  std::vector bodies{
      Body{60, 10, "earth"},
      Body{20, 1e7, "sun"},
      Body{100, 50, "saturn"}
  };

  // earth
  bodies[0].setFillColor(sf::Color::Blue);
  bodies[0].setPosition({200, 50});
  bodies[0].setVelocity({0, 600});

  // sun
  bodies[1].setFillColor(sf::Color::Yellow);
  bodies[1].setPosition(getWindowCenter(window));

  // saturn
  bodies[2].setFillColor({246, 223, 108, 230});
  bodies[2].setPosition({windowWidth - 100, windowHeight - 200});
  bodies[2].setVelocity({-10, -300});

  //--------------------Clock-----------------------------------------
  sf::Clock clock{};
  clock.start();
  float dt = 0; // the time between the rendering of the last frame and the one before it

  //--------------------Main loop-------------------------------------
  while (window.isOpen()) {
    while (const auto event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>())
        window.close();
    }

    window.clear();
    // ====================== Update ====================
    updateBodies(bodies, dt);
    handleWallCollisions(bodies);


    // ====================== Render ======================
    renderBodies(bodies, window);
    dt = clock.restart().asSeconds();

    window.display();
  }

  return 0;
}