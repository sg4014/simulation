//
// Created by sergey on 9/11/26.
//

#ifndef SIMULATION_UI_H
#define SIMULATION_UI_H
#include "SFML/Graphics/RenderWindow.hpp"

namespace Sim::UI {
bool initImGui(sf::RenderWindow& window);

void definePropsTabItem();

void defineFruitsTabItem();

void defineUI();
}

#endif //SIMULATION_UI_H