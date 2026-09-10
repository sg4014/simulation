#include "Constants.h"
#include "Ui.h"
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
  for (std::size_t i = 0; i < bodies.size(); ++i) {
    if (i == 0 && !ui::drawFirstBody)
      continue;

    window.draw(bodies[i]);
  }
  // for (const auto& body : bodies) {
  //   window.draw(body);
  // }
}

void updatePositions(std::vector<Body>& bodies, sf::Time dt) {
  for (auto& body : bodies) {
    body.updatePosition(dt.asSeconds());
  }
}

void initBodies(std::vector<Body>& bodies, const sf::Font& font) {
  bodies.reserve(3);
  bodies.emplace_back("earth", 80, 80, font);
  bodies.emplace_back("sun", 20, 20, font);
  bodies.emplace_back("saturn", 120, 120, font);

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
  bodies[1].setPosition(Constants::wCenter);

  // saturn
  bodies[2].setFillColor(green);
  bodies[2].setPosition({
      Constants::wWidth - bodies[2].getRadius(),
      Constants::wHeight / 2.f
  });
  bodies[2].setVelocity({-200, 0});
}
}

int main() {
  namespace Constants = Sim::Constants;

  sf::RenderWindow window{sf::VideoMode{{Constants::wWidth, Constants::wHeight}}, "Simulation"};
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
  [[maybe_unused]] float imguiColor[3] = {0.0f, 1.0f, 1.0f};

  //--------------------Font-----------------------------------------
  sf::Font font;

  if (!font.openFromFile("resources/arial.ttf")) {
    std::cerr << "Couldn't open font file resources/arial.ttf\n";
    std::exit(1);
  }

  //--------------------DEFINE BODIES--------------------------------
  std::vector<Sim::Body> bodies{};
  Sim::initBodies(bodies, font);



  //--------------------Clock-----------------------------------------
  sf::Clock deltaClock{};
  deltaClock.start();
  sf::Time dt{}; // the time between the rendering of the last frame and the one before it

  constexpr std::array backgrounds{sf::Color::Black, sf::Color::White};
  std::size_t bgIndex = 0;

  //--------------------Main loop-------------------------------------
  while (window.isOpen()) {
    while (const auto event = window.pollEvent()) {
      ImGui::SFML::ProcessEvent(window, *event);

      if (event->is<sf::Event::Closed>())
        window.close();
      else if (const auto mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
        for (std::size_t i = 0; i < bodies.size(); ++i) {
          if (bodies[i].contains(mouseButtonPressed->position)) {
            Sim::ui::activeBodyIdx = i;
            break;
          }
        }
      }
    }


    // ====================== Update ====================
    // update ui toggled
    Sim::Body& activeBody = bodies[Sim::ui::activeBodyIdx];
    activeBody.setIsDisplayName(Sim::ui::IsDisplayName);
    activeBody.setPointCount(Sim::ui::circlePointCount);
    activeBody.setRadius(Sim::ui::radius); // IMPORTANT: update geometry _before_ handling collisions
    activeBody.setVelocity({Sim::ui::velocityX, Sim::ui::velocityY});

    handleWallCollisions(bodies);
    handleCollisionsBetweenBodies(bodies);
    updatePositions(bodies, dt);

    ImGui::SFML::Update(window, dt);

    // ======Clear========
    window.clear(backgrounds[bgIndex]);

    // ====================== Render ======================
    renderBodies(bodies, window);
    // -----render imgui start-----
    ImGui::Begin("Window title");
    ImGui::Text("window text");
    ImGui::Checkbox("Draw 1st body", &Sim::ui::drawFirstBody);
    ImGui::SameLine();
    ImGui::Checkbox("Display name", &Sim::ui::IsDisplayName);
    ImGui::SliderInt("Sides", &Sim::ui::circlePointCount, 3, 64);
    ImGui::SliderFloat("Radius", &Sim::ui::radius, 10.0f, 200.0f);
    ImGui::Text("Velocity:");
    ImGui::SliderFloat("x", &Sim::ui::velocityX, -300.0f, 300.0f, "%.1f");
    ImGui::SliderFloat("y", &Sim::ui::velocityY, -300.0f, 300.0f, "%.1f");
    if (ImGui::Button("Switch Theme")) {
      bgIndex = (bgIndex + 1) % backgrounds.size();
    }
    ImGui::End();
    ImGui::SFML::Render(window);

    // -----render imgui end-----
    dt = deltaClock.restart();

    window.display();
  }

  return 0;
}