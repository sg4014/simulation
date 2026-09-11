//
// Created by sergey on 9/11/26.
//

#ifndef SIMULATION_GLOBAL_H
#define SIMULATION_GLOBAL_H
#include "Body.h"
#include "SFML/Graphics/Color.hpp"
#include <array>
#include <vector>

namespace Sim::UI {
extern bool g_isHidden;
extern bool g_isDisplayName;
extern int g_bodySides;
extern int g_activeBodyIdx;
extern float g_radius;
extern float g_velocity[2];
extern float g_imguiColor[3];
extern char g_nameBuffer[255];

extern std::vector<Body> g_bodies;

extern std::vector<const char*> g_names;

extern std::size_t g_bgIndex;

constexpr std::array g_backgrounds{sf::Color::Black, sf::Color::White};
}
#endif //SIMULATION_GLOBAL_H