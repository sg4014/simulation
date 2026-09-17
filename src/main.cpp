#include <imgui.h>

#include "Constants.h"
#include "Body.h"
#include "Collisions.h"
#include "Util.h"
#include "MyImgui.h"
#include "BodyUtil.h"
#include "Global.h"
#include "Random.h"
#include "imgui-SFML.h"
#include "SFML/Graphics.hpp"
#include <cmath>
#include <iostream>


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

void initBodies(std::vector<Body>& bodies, int n, const sf::Font& font) {
    bodies.reserve(n);
    for (int i = 0; i < n; ++i) {
        bodies.emplace_back(getRandomName(), Random::get(10, 40), Random::get(5, 150), font);
        auto& body = bodies[i];
        body.setFillColor(getRandomColor());
        body.setPosition(getRandomPosition(body));
        body.setVelocity({
            static_cast<float>(Random::get(-1000, 1000)),
            static_cast<float>(Random::get(-800, 800))
        });
    }
}

void selectClickedBody(sf::Vector2i clickPosition) {
    for (std::size_t i = 0; i < g_bodies.size(); ++i) {
        if (isWithinBody(clickPosition, g_bodies[i])) {
            g_activeBodyIdx = static_cast<int>(i);
            resetParameters();
        }
    }
}

void processEvents(sf::RenderWindow& window) {
    while (const auto event = window.pollEvent()) {
        ImGui::SFML::ProcessEvent(window, *event);

        if (event->is<sf::Event::Closed>())
            window.close();
        else if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (!ImGui::GetIO().WantCaptureMouse) {
                // select body only if it wasn't clicked "through" an ImGui window
                selectClickedBody(mouseButtonPressed->position);
            }
        } else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            if (keyPressed->scancode == sf::Keyboard::Scancode::Space) {
                g_isPaused = !g_isPaused;
            }
        }
    }
}

void updateBodies(sf::Time dt) {
    handleWallCollisions(g_bodies);
    handleCollisionsBetweenBodies(g_bodies);
    updatePositions(g_bodies, dt);
}

void render(sf::RenderWindow& window) {
    window.clear(g_backgrounds[g_bgIndex]);
    defineUI();
    renderBodies(g_bodies, window);
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

    if (!Sim::initImGui(window)) {
        std::cerr << "Couldn't initialize ImGui.\n";
        std::exit(1);
    }

    sf::Font font;
    if (!font.openFromFile("resources/arial.ttf")) {
        std::cerr << "Couldn't open font file resources/arial.ttf\n";
        std::exit(1);
    }

    Sim::initBodies(Sim::g_bodies, 1000, font);

    //--------------------Clock-----------------------------------------
    sf::Clock deltaClock{};
    deltaClock.start();
    sf::Time dt{}; // the time between the rendering of the last frame and the one before it

    //--------------------Main loop-------------------------------------
    while (window.isOpen()) {
        Sim::processEvents(window);
        if (!Sim::g_isPaused) {
            Sim::updateBodies(dt);
        }
        ImGui::SFML::Update(window, dt);
        Sim::render(window);
        dt = deltaClock.restart();
    }

    return 0;
}