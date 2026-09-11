//
// Created by sergey on 9/11/26.
//

#include "Global.h"
#include <vector>

namespace Sim::UI {
bool g_isHidden = false;
bool g_isDisplayName = true;
int g_bodySides = 30;
int g_activeBodyIdx{};
float g_radius = 100;
float g_velocity[2];
float g_imguiColor[3]{0, 1, 1};
char g_nameBuffer[255];

std::vector<Body> g_bodies{};

// const char* names[3]{"earth", "sun", "saturn"};
std::vector<const char*> g_names{"earth", "sun", "saturn"};

std::size_t g_bgIndex = 0;
}
