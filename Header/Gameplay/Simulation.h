#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <vector>

#include "Formation.h"

class Simulation
{
public:
    Simulation();
    ~Simulation();

    bool Start();
    bool Update();
    bool Close();
private:
    void CheckInputs();

    std::shared_ptr<sf::RenderWindow> window;
    std::shared_ptr<sf::View> camera;
    std::vector<std::shared_ptr<Formation>> formations;

    bool holdingDelete = false;
};
