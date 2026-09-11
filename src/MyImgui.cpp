//
// Created by sergey on 9/11/26.
//

#include "Global.h"
#include "Util.h"
#include "MyImgui.h"
#include "imgui.h"
#include "imgui-SFML.h"

namespace Sim::UI {
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
                         &g_activeBodyIdx,
                         g_names.data(),
                         static_cast<int>(g_names.size()))) {
            resetParameters();
        }

        auto& activeBody = g_bodies[g_activeBodyIdx];

        if (ImGui::Checkbox("Hidden", &g_isHidden)) {
            activeBody.setHidden(g_isHidden);
        }
        if (ImGui::SliderInt("Sides", &g_bodySides, 3, 64)) {
            activeBody.setPointCount(g_bodySides);
        }
        if (ImGui::SliderFloat("Radius", &g_radius, 10.0f, 200.0f)) {
            activeBody.setRadius(g_radius);
        }
        if (ImGui::SliderFloat2("Velocity", g_velocity, -2000.0f, 2000.0f)) {
            activeBody.setVelocity(Sim::UI::arrToVec(g_velocity));
        }
        if (ImGui::ColorEdit3("Color", g_imguiColor)) {
            activeBody.setFillColor(toSFMLColor(g_imguiColor));
        }
        if (ImGui::Checkbox("Display name", &g_isDisplayName)) {
            activeBody.setIsNameDisplayed(g_isDisplayName);
        }
        constexpr std::size_t bufferSize = 64;
        static char buffer[bufferSize];
        if (ImGui::InputText("Name", buffer, bufferSize)) {
            activeBody.setName(buffer);
            g_names[g_activeBodyIdx] = buffer;
        }
        if (ImGui::Button("Switch Theme")) {
            g_bgIndex = (g_bgIndex + 1) % g_backgrounds.size();
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
}