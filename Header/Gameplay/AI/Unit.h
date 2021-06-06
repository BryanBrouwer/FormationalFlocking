#pragma once
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "glm/glm.hpp"
#include "FlockDataManager.h"

class Unit
{
public:
    Unit();
    Unit(const glm::fvec2& worldPosition, const glm::fvec2& formationPositionOffset, std::shared_ptr<FlockVariables> FlockVariables);
    ~Unit();

    bool UnitMovement(const std::unordered_multimap<int, std::shared_ptr<Unit>>& unitsGrid, const float& dt);
    bool UpdateShape();
    void RenderUnit(const std::shared_ptr<sf::RenderWindow>& window) const;
    void SetFormationPosOffset(const glm::fvec2& newFormationPosOffset);
    void SetTargetFormationPos(const glm::fvec2& newFormationPos);

    const glm::fvec2& GetCurrentPos() const;
    const glm::fvec2& GetVelocity() const;
    const glm::fvec2& GetFormationPosOffset() const;

    const float& RotateUnit(const glm::fvec2& nextPosition);
    const float& GetRotation() const;

    int gridKey = 0;

private:
    glm::fvec2 FlockingVelocityCalc(const std::unordered_multimap<int, std::shared_ptr<Unit>>& unitsGrid);

    sf::CircleShape shape;

    //Flocking Variables
    bool isActive = true;
    bool shouldFlock = true;
    bool inFormation = true;
    bool blocked = false;

    float rotation = 0;

    //Target position variables
    glm::fvec2 targetPos = glm::fvec2(0, 0);
    glm::fvec2 targetFormationPos{};
    glm::fvec2 formationPosOffset{};

    //Current position and velocity
    glm::fvec2 currentPos{};
    glm::fvec2 velocity = glm::fvec2(0,0);

    std::shared_ptr<FlockVariables> flockVariables;
};
