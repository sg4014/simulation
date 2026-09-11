#include "Constants.h"
#include "Body.h"
#include "Collisions.h"
#include "Util.h"
#include "MyImgui.h"
#include "imgui.h"
#include "imgui-SFML.h"
#include "SFML/Graphics.hpp"
#include <cmath>
#include <iostream>

#include "BodyUtil.h"
#include "Global.h"

namespace Sim {
void renderBodies(const std::vector<Body>& bodies, sf::RenderWindow& window) {
    for (const auto& body : bodies) {
        if (!body.isHidden()) {
            window.draw(body);
        }
    }
}

void updatePositions(std::vector<Body>& bodies, sf::Time dt) {
    for (auto& body : bodies) {
        if (!body.isHidden()) {
            body.updatePosition(dt.asSeconds());
        }
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

bool selectClickedBody(sf::Vector2i clickPosition) {
    for (std::size_t i = 0; i < UI::g_bodies.size(); ++i) {
        if (isWithinBody(clickPosition, UI::g_bodies[i])) {
            UI::g_activeBodyIdx = static_cast<int>(i);
            UI::resetParameters();
            return true;
        }
    }
    return false;
}

void processEvents(sf::RenderWindow& window) {
    while (const auto event = window.pollEvent()) {
        ImGui::SFML::ProcessEvent(window, *event);

        if (event->is<sf::Event::Closed>())
            window.close();
        else if (const auto mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
            selectClickedBody(mouseButtonPressed->position);
        }
    }
}

void update(sf::RenderWindow& window, sf::Time dt) {
    handleWallCollisions(UI::g_bodies);
    handleCollisionsBetweenBodies(UI::g_bodies);
    updatePositions(UI::g_bodies, dt);

    ImGui::SFML::Update(window, dt);
}

void render(sf::RenderWindow& window) {
    window.clear(UI::g_backgrounds[UI::g_bgIndex]);
    UI::defineUI();
    renderBodies(UI::g_bodies, window);
    ImGui::SFML::Render(window);
    window.display();
}

}

int main() {
    sf::RenderWindow window{sf::VideoMode{
                                {Sim::Constants::wWidth, Sim::Constants::wHeight}
                            },
                            "Simulation"};
    window.setVerticalSyncEnabled(true);

    if (!Sim::UI::initImGui(window)) {
        std::cerr << "Couldn't initialize ImGui.\n";
        std::exit(1);
    }

    sf::Font font;
    if (!font.openFromFile("resources/arial.ttf")) {
        std::cerr << "Couldn't open font file resources/arial.ttf\n";
        std::exit(1);
    }

    Sim::initBodies(Sim::UI::g_bodies, font);

    //--------------------Clock-----------------------------------------
    sf::Clock deltaClock{};
    deltaClock.start();
    sf::Time dt{}; // the time between the rendering of the last frame and the one before it

    //--------------------Main loop-------------------------------------
    while (window.isOpen()) {
        Sim::processEvents(window);
        Sim::update(window, dt);
        Sim::render(window);
        dt = deltaClock.restart();
    }

    return 0;
}