//
// Created by sergey on 9/10/26.
//

#ifndef SIMULATION_UI_H
#define SIMULATION_UI_H
#include <cstddef>

namespace Sim::ui {
extern bool drawFirstBody;
extern bool IsDisplayName;
extern int circlePointCount;
extern std::size_t activeBodyIdx;
extern float radius;
extern float velocityX;
extern float velocityY;
}
#endif //SIMULATION_UI_H