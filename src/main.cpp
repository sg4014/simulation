#include "Constants.h"
#include "Body.h"
#include "Collisions.h"
#include "Logger.h"
#include "SFML/Graphics.hpp"
#include <cmath>


#define DEBUG

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

  sf::RenderWindow window{sf::VideoMode{{Constants::windowWidth, Constants::windowHeight}}, "Simulation"};
  window.setVerticalSyncEnabled(true);

  const bool isDebug = true;

  //--------------------Font-----------------------------------------
  if (!Sim::font.openFromFile("resources/arial.ttf")) {
    std::cerr << "Couldn't open font file resources/arial.ttf\n";
    std::exit(1);
  }

  //--------------------DEFINE BODIES--------------------------------
  std::vector bodies{
      Sim::Body{"earth", 60, 10, Sim::font},
      Sim::Body{"sun", 20, 1e6, Sim::font},
      Sim::Body{"saturn", 100, 50, Sim::font}
  };

  // earth
  bodies[0].setFillColor(sf::Color::Blue);
  bodies[0].setPosition({200, 50});
  bodies[0].setVelocity({0, 600});

  // sun
  bodies[1].setFillColor(sf::Color::Yellow);
  bodies[1].setPosition(Sim::getWindowCenter(window));

  // saturn
  bodies[2].setFillColor({246, 223, 108, 230});
  bodies[2].setPosition({Constants::windowWidth - 100, Constants::windowHeight - 200});
  bodies[2].setVelocity({-10, -300});

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