#include "Constants.h"
#include "Body.h"
#include "Collisions.h"
#include "Logger.h"
#include "imgui.h"
#include "imgui-SFML.h"
#include "SFML/Graphics.hpp"
#include <cmath>

#include "BodyUtil.h"

namespace Sim {
std::vector<Body> g_bodies{};
}

namespace Sim::ui {
bool isHidden = false;
bool isDisplayName = true;
int bodySides = 30;
int activeBodyIdx{};
float radius = 100;
float velocity[2];
float imguiColor[3]{0, 1, 1};
char nameBuffer[255];

// const char* names[3]{"earth", "sun", "saturn"};
std::vector names{"earth", "sun", "saturn"};

constexpr std::array backgrounds{sf::Color::Black, sf::Color::White};
std::size_t bgIndex = 0;

sf::Vector2f arrToVec(const float arr[2]) {
    return {arr[0], arr[1]};
}

// The ImGui color {r, g, b} wheel requires floats from 0 to 1.
// But SFML requires std::uint8_t from 0 to 255.
sf::Color toSFMLColor(const float col[3]) {
    return sf::Color{
        static_cast<std::uint8_t>(col[0] * 255),
        static_cast<std::uint8_t>(col[1] * 255),
        static_cast<std::uint8_t>(col[2] * 255)
    };
}

void resetParameters() {
    auto& activeBody = g_bodies[activeBodyIdx];
    velocity[0] = activeBody.getVelocity().x;
    velocity[1] = activeBody.getVelocity().y;
    isDisplayName = activeBody.isNameDisplayed();
    isHidden = activeBody.isHidden();
    bodySides = static_cast<int>(activeBody.getPointCount());
    radius = activeBody.getRadius();
    const auto sfmlColor = activeBody.getFillColor();
    imguiColor[0] = static_cast<float>(sfmlColor.r / 255);
    imguiColor[1] = static_cast<float>(sfmlColor.g / 255);
    imguiColor[2] = static_cast<float>(sfmlColor.b / 255);
}
}


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
    for (std::size_t i = 0; i < g_bodies.size(); ++i) {
        if (isWithinBody(clickPosition, g_bodies[i])) {
            ui::activeBodyIdx = static_cast<int>(i);
            ui::resetParameters();
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

bool initImGui(sf::RenderWindow& window) {
    if (!ImGui::SFML::Init(window)) {
        return false;
    }

    // Scale ImGui UI and text size by 2
    ImGui::GetStyle().ScaleAllSizes(2.0f);
    ImGui::GetIO().FontGlobalScale = 2.0f;
    return true;
}

void definePropsTabItem() {
    if (ImGui::BeginTabItem("Shape props##s1")) {
        if (ImGui::Combo("Shape",
                         &ui::activeBodyIdx,
                         ui::names.data(),
                         static_cast<int>(ui::names.size()))) {
            ui::resetParameters();
        }

        auto& activeBody = g_bodies[ui::activeBodyIdx];

        if (ImGui::Checkbox("Hidden", &ui::isHidden)) {
            activeBody.setHidden(ui::isHidden);
        }
        if (ImGui::SliderInt("Sides", &ui::bodySides, 3, 64)) {
            activeBody.setPointCount(ui::bodySides);
        }
        if (ImGui::SliderFloat("Radius", &ui::radius, 10.0f, 200.0f)) {
            activeBody.setRadius(ui::radius);
        }
        if (ImGui::SliderFloat2("Velocity", ui::velocity, -2000.0f, 2000.0f)) {
            activeBody.setVelocity(ui::arrToVec(ui::velocity));
        }
        if (ImGui::ColorEdit3("Color", ui::imguiColor)) {
            activeBody.setFillColor(ui::toSFMLColor(ui::imguiColor));
        }
        if (ImGui::Checkbox("Display name", &ui::isDisplayName)) {
            activeBody.setIsNameDisplayed(ui::isDisplayName);
        }
        constexpr std::size_t bufferSize = 64;
        static char buffer[bufferSize];
        if (ImGui::InputText("Name", buffer, bufferSize)) {
            activeBody.setName(buffer);
            ui::names[ui::activeBodyIdx] = buffer;
        }
        if (ImGui::Button("Switch Theme")) {
            ui::bgIndex = (ui::bgIndex + 1) % ui::backgrounds.size();
        }

        ImGui::EndTabItem();
    }
}

void defineFruitsTabItem() {
    if (ImGui::BeginTabItem("Fruits")) {
        ImGui::Text("Fruits tab");
        ImGui::EndTabItem();
    }
}

void defineUI() {
    ImGui::Begin("UI bruh");

    if (ImGui::BeginTabBar("My Tabs")) {
        definePropsTabItem();
        defineFruitsTabItem();

        ImGui::EndTabBar();
    }

    ImGui::End();
}

void update(sf::RenderWindow& window, sf::Time dt) {
    handleWallCollisions(Sim::g_bodies);
    handleCollisionsBetweenBodies(Sim::g_bodies);
    updatePositions(Sim::g_bodies, dt);

    ImGui::SFML::Update(window, dt);
}

void render(sf::RenderWindow& window) {
    window.clear(ui::backgrounds[ui::bgIndex]);
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

    Sim::initBodies(Sim::g_bodies, font);

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