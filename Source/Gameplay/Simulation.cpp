#include <algorithm>
#include "Simulation.h"
#include "FlockDataManager.h"

Simulation::Simulation()
{
}

Simulation::~Simulation()
{
    formations.erase(formations.begin(), formations.end());
}

bool Simulation::Start()
{
    window = std::make_shared<sf::RenderWindow>(sf::VideoMode(1920, 1080), "NPC_Dialogue!");
    //camera = std::make_shared<sf::View>(sf::Vector2f(640, 360), sf::Vector2f(1280, 720));
    camera = std::make_shared<sf::View>(sf::Vector2f(0, 0), sf::Vector2f(1280, 720));
    camera->zoom(0.5f);
    window->setView(*camera);

    window->setFramerateLimit(60);

    FlockDataManager::getInstance().SetJsonFile("JSON/FlockVariables.json");
    FlockDataManager::getInstance().ResetMainFlockVariables();
    FlockDataManager::getInstance().ResetCellSize();
    FlockDataManager::getInstance().ResetFormationSpeed();

    formations.push_back(std::make_shared<Formation>(glm::fvec2(100, 70), 8, 12, 7, 7));
    //formations.push_back(std::make_shared<Formation>(glm::fvec2(100, 70), 10, 10, 6, 6));
    //formations.push_back(std::make_shared<Formation>(glm::fvec2(100, 70), 10, 10, 7, 7));
    //formations.push_back(std::make_shared<Formation>(glm::fvec2(100, 70), 10, 10, 8, 8));

    //formations.push_back(std::make_shared<Formation>(glm::fvec2(100, 70), 10, 10, 9, 9));
    //formations.push_back(std::make_shared<Formation>(glm::fvec2(100, 70), 10, 10, 10, 10));
    //formations.push_back(std::make_shared<Formation>(glm::fvec2(100, 70), 10, 10, 11, 11));
    //formations.push_back(std::make_shared<Formation>(glm::fvec2(100, 70), 10, 10, 12, 12));

    //formations.push_back(std::make_shared<Formation>(glm::fvec2(100, 70), 10, 10, 13, 13));
    //formations.push_back(std::make_shared<Formation>(glm::fvec2(100, 70), 10, 10, 14, 14));
    //formations.push_back(std::make_shared<Formation>(glm::fvec2(100, 70), 10, 10, 15, 15));
    //formations.push_back(std::make_shared<Formation>(glm::fvec2(100, 70), 10, 10, 16, 16));

    //formations.push_back(std::make_shared<Formation>(glm::fvec2(100, 70), 10, 10, 17, 17));
    //formations.push_back(std::make_shared<Formation>(glm::fvec2(100, 70), 10, 10, 18, 18));
    //formations.push_back(std::make_shared<Formation>(glm::fvec2(100, 70), 10, 10, 19, 19));
    //formations.push_back(std::make_shared<Formation>(glm::fvec2(100, 70), 10, 10, 20, 20));

    return true;
}

bool Simulation::Update()
{
    sf::Clock clock;
    while (window->isOpen())
    {
        sf::Time time = clock.restart();
        float deltaTime = time.asSeconds();

        //sfml check if window hasnt closed yet.
        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window->close();
        }
        //Clear old window
        window->clear();        
        // inside the main loop, between window.clear() and window.display()

        //Checking for possible inputs
        if (window->hasFocus())
        {
            CheckInputs();
        }

        for (const std::shared_ptr<Formation>& formation : formations)
        {
            if (formation->CheckAlive())
            {
                formation->UpdateMovement(deltaTime);
                formation->RenderUnits(window);
            }
            else
            {
                formations.erase(std::remove(formations.begin(), formations.end(), formation), formations.end());
            }
        }

        //
        window->display();
    }
    return true;
}

bool Simulation::Close()
{
    return true;
}

void Simulation::CheckInputs()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Delete) && !holdingDelete)
    {
        holdingDelete = true;
        if (!formations.empty())
        {
            formations[0]->RemoveUnit(formations[0]->GetUnits().back());
        }
    }
    else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Delete) && holdingDelete)
    {
        holdingDelete = false;
    }

    //Make formation move towards mouse while left mouse button is pressed.
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        // get the current mouse position in the window
        const sf::Vector2i pixelPos = sf::Mouse::getPosition(*window);
        // convert it to world coordinates
        const sf::Vector2f worldPos = window->mapPixelToCoords(pixelPos);
        //converting it to GLM vector
        const glm::fvec2 mousePosition = glm::fvec2(worldPos.x, worldPos.y);

        for (const std::shared_ptr<Formation>& formation : formations)
        {
            formation->SetTargetPosition(mousePosition);
        }

    }
}
