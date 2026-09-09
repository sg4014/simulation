#include "Constants.h"
#include "Util.h"
#include "Body.h"
#include "Collisions.h"
#include "Logger.h"
#include "SFML/Graphics.hpp"
#include <cmath>


#undef DEBUG

namespace Sim {
sf::Font font;

void renderBodies(const std::vector<Body>& bodies, sf::RenderWindow& window) {
  for (const auto& body : bodies) {
    window.draw(body);
  }
}

void updatePositions(std::vector<Body>& bodies, float dt) {
  for (auto& body : bodies) {
    body.updatePosition(dt);
  }
}
}

int main() {
  // TODO: add imgui
  namespace Constants = Sim::Constants;

  sf::RenderWindow window{sf::VideoMode{{Constants::windowWidth, Constants::windowHeight}}, "Simulation"};
  window.setVerticalSyncEnabled(true);

  //--------------------Font-----------------------------------------
  if (!Sim::font.openFromFile("resources/arial.ttf")) {
    std::cerr << "Couldn't open font file resources/arial.ttf\n";
    std::exit(1);
  }

  //--------------------DEFINE BODIES--------------------------------
  std::vector bodies{
      Sim::Body{"earth", 80, 80, Sim::font},
      Sim::Body{"sun", 20, 10, Sim::font},
      Sim::Body{"saturn", 150, 150, Sim::font}
  };

  const std::uint8_t opacity = 100;
  const sf::Color blue{0, 0, 255, opacity};
  const sf::Color red{255, 0, 0, opacity};
  const sf::Color green{0, 255, 0, opacity};
  // earth
  bodies[0].setFillColor(blue);
  bodies[0].setPosition({
      bodies[0].getRadius(),
      Constants::windowHeight / 2.f
  });
  bodies[0].setVelocity({200, 400});

  // sun
  bodies[1].setFillColor(red);
  bodies[1].setPosition(Sim::getWindowCenter(window));

  // saturn
  bodies[2].setFillColor(green);
  bodies[2].setPosition({
      Constants::windowWidth - bodies[2].getRadius(),
      Constants::windowHeight / 2.f
  });
  bodies[2].setVelocity({-200, 0});

  //--------------------Clock-----------------------------------------
  sf::Clock clock{};
  clock.start();
  float dt = 0; // the time between the rendering of the last frame and the one before it

#ifdef DEBUG
  dt = 1.f / 25.f;
#endif

  //--------------------Main loop-------------------------------------
  while (window.isOpen()) {
    while (const auto event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>())
        window.close();
#ifdef DEBUG
      else if (const auto keyPressed = event->getIf<sf::Event::KeyPressed>()) {
        switch (keyPressed->code) {
          case sf::Keyboard::Key::F:
            dt = 1.f;
            break;
          case sf::Keyboard::Key::E:
            dt = 1.f / 25.f;
            break;
          case sf::Keyboard::Key::W:
            dt = 1.f / 60.f;
            break;
          default:
            dt = 1.f / 60.f;
        }
        window.clear(sf::Color::White);
        handleWallCollisions(bodies);
        handleCollisionsBetweenBodies(bodies);
        updatePositions(bodies, dt);
        renderBodies(bodies, window);
      }
#endif
    }

#ifndef DEBUG
    window.clear();
    // ====================== Update ====================
    handleWallCollisions(bodies);
    handleCollisionsBetweenBodies(bodies);
    updatePositions(bodies, dt);

    // ====================== Render ======================
    renderBodies(bodies, window);
    dt = clock.restart().asSeconds();
#endif

    window.display();
  }

  return 0;
}