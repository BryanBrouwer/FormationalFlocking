#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include "Unit.h"
#include "FlockDataManager.h"

class Formation
{
public:
    Formation();
    Formation(const glm::fvec2& spawnPosition , int xCount, int yCount, float xOffset, float yOffset);
    ~Formation();

    bool UpdateMovement(const float& dt);
    bool CheckAlive() const;
    void RenderUnits(const std::shared_ptr<sf::RenderWindow>& window);

    std::vector<std::shared_ptr<Unit>>& GetUnits();

    bool AddUnit(const std::shared_ptr<Unit>& unit);
    bool RemoveUnit(const std::shared_ptr<Unit>& unit);
    const glm::vec2& GetPosition() const;

    //Setters and Getters for variables.
    //Dangerous operation when used over long distances.
    void SetPosition(const glm::vec2& newPosition);

    void ResetFormationSpeed();
    void ResetAcceptanceDistance();

    const float& GetRotation() const;
    const float& AddRotation(const float& angle);
    void SetRotation(const float& angle);

    void SetTargetPosition(const glm::fvec2& target);
    const glm::fvec2& GetTargetPosition() const;

private:
    bool ReassignGrid();
    bool UpdateUnitFormationPositions();
    bool UpdateMovementUnits(const float& dt);
    static int GetUniqueKeyForPosition(const glm::fvec2& position, const float& cellSize);

    glm::fvec2 position{};
    glm::fvec2 targetPosition{};
    float speed{};
    float rotation{};
    float acceptanceDistance{};
    float oldRotation = 0;
    std::shared_ptr<FlockVariables> flockVariables;
    std::vector<std::shared_ptr<Unit>> units;
    std::unordered_multimap<int, std::shared_ptr<Unit>> gridUnits;
};