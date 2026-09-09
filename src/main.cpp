#include "Constants.h"
#include "Util.h"
#include "Body.h"
#include "Collisions.h"
#include "Logger.h"
#include "imgui.h"
#include "imgui-SFML.h"
#include "SFML/Graphics.hpp"
#include <cmath>

#undef DEBUG

namespace Sim {
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
  namespace Constants = Sim::Constants;

  sf::RenderWindow window{sf::VideoMode{{Constants::wWidth, Constants::wHeight}}, "Simulation"};
  // window.setVerticalSyncEnabled(true);
  window.setFramerateLimit(75);

  //--------------------ImGui--------------------------------------
  if (!ImGui::SFML::Init(window)) {
    std::cerr << "Couldn't initialize ImGui!\n";
    std::exit(1);
  }

  // Scale ImGui UI and text size by 2
  ImGui::GetStyle().ScaleAllSizes(2.0f);
  ImGui::GetIO().FontGlobalScale = 2.0f;

  // The ImGui color {r, g, b} wheel requires floats from 0 to 1.
  // But SFML requires std::uint8_t from 0 to 255.
  // That's why we can't easily use sf::Color for imgui.
  // float imguiColor[3] = {0.0f, 1.0f, 1.0f};

  //--------------------Font-----------------------------------------
  sf::Font font;

  if (!font.openFromFile("resources/arial.ttf")) {
    std::cerr << "Couldn't open font file resources/arial.ttf\n";
    std::exit(1);
  }

  //--------------------DEFINE BODIES--------------------------------
  std::vector bodies{
      Sim::Body{"earth", 80, 80, font},
      Sim::Body{"sun", 20, 20, font},
      Sim::Body{"saturn", 150, 150, font}
  };

  const std::uint8_t opacity = 100;
  const sf::Color blue{0, 0, 255, opacity};
  const sf::Color red{255, 0, 0, opacity};
  const sf::Color green{0, 255, 0, opacity};
  // earth
  bodies[0].setFillColor(blue);
  bodies[0].setPosition({
      bodies[0].getRadius(),
      Constants::wHeight / 2.f
  });
  bodies[0].setVelocity({200, 400});

  // sun
  bodies[1].setFillColor(red);
  bodies[1].setPosition(Sim::getWindowCenter(window));

  // saturn
  bodies[2].setFillColor(green);
  bodies[2].setPosition({
      Constants::wWidth - bodies[2].getRadius(),
      Constants::wHeight / 2.f
  });
  bodies[2].setVelocity({-200, 0});

  //--------------------Clock-----------------------------------------
  sf::Clock deltaClock{};
  deltaClock.start();
  sf::Time dt; // the time between the rendering of the last frame and the one before it


#ifdef DEBUG
  dt = 1.f / 25.f;
#endif

  //--------------------Main loop-------------------------------------
  while (window.isOpen()) {
    while (const auto event = window.pollEvent()) {
      ImGui::SFML::ProcessEvent(window, *event);

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
    // ====================== Update ====================
    handleWallCollisions(bodies);
    handleCollisionsBetweenBodies(bodies);
    updatePositions(bodies, dt.asSeconds());
    ImGui::SFML::Update(window, dt);

    // ======Clear========
    window.clear();
    // ==================-

    // ====================== Render ======================
    renderBodies(bodies, window);
    // -----render imgui start-----
    ImGui::Begin("Window title");
    ImGui::Text("window text");
    ImGui::End();
    ImGui::SFML::Render(window);

    // -----render imgui end-----
    dt = deltaClock.restart();
#endif

    window.display();
  }

  return 0;
}