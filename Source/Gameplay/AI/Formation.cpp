#include "Formation.h"

Formation::Formation()
= default;

Formation::Formation(const glm::fvec2& spawnPosition, int xCount, int yCount, float xOffset, float yOffset)
{
    position = spawnPosition;
    targetPosition = position;
    ResetFormationSpeed();
    ResetAcceptanceDistance();
    FlockVariables formationFlockVariables = FlockDataManager::getInstance().MainFlockVariables;
    flockVariables = std::make_shared<FlockVariables>(formationFlockVariables);
    for (int x = 0; x < xCount; x++)
    {
        for (int y = 0; y < yCount; y++)
        {
            const float newX = (x - (static_cast<float>(xCount) * 0.5f)) * xOffset;
            const float newY = (y - (static_cast<float>(yCount) * 0.5f)) * yOffset;

            glm::fvec2 unitPosition = glm::fvec2(position.x + newX, position.y + newY);
            Unit unit = Unit(unitPosition, glm::fvec2(newX, newY), flockVariables);
            units.push_back(std::make_shared<Unit>(unit));
        }
    }

}

Formation::~Formation()
{
    units.erase(units.begin(), units.end());
}

bool Formation::UpdateMovement(const float& dt)
{
    if (glm::distance(targetPosition, position) > acceptanceDistance)
    {
        //Movement of formation leader itself.
        //current implementation moves towards mouse position.
        glm::fvec2 direction = targetPosition - position;
        if (direction != glm::fvec2(0, 0))
        {
            direction = glm::normalize(direction);
        }

        auto distance = glm::distance(targetPosition, position);

        const glm::fvec2 nextPosition = position + (direction * speed * glm::clamp(dt, 0.00f, 0.5f));

        //Rotate the formation towards next position
        const float PI = 3.14159265f;
        const float dx = nextPosition.x - position.x;
        const float dy = nextPosition.y - position.y;
        rotation = (atan2(dy, dx)) * 180 / PI;
        rotation = rotation + 180;

        //actually setting the position to next position
        position = nextPosition;
    }

        //Movement of units in the formation.
        UpdateMovementUnits(dt);

        //should be put in updatemovementunits when not relying on mouse position anymore
        //for (const std::shared_ptr<Unit>& unit : units)
        //{
        //    unit->RotateUnit(nextPosition);
        //}
    return true;
}

bool Formation::CheckAlive() const
{
    return !units.empty();
}

void Formation::RenderUnits(const std::shared_ptr<sf::RenderWindow>& window)
{
    sf::CircleShape formationLeaderDebug = sf::CircleShape(2);
    formationLeaderDebug.setFillColor(sf::Color::Cyan);
    formationLeaderDebug.setPosition(sf::Vector2f(position.x, position.y));
    window->draw(formationLeaderDebug);

    for (const std::shared_ptr<Unit>& unit : units)
    {
        unit->RenderUnit(window);
    }
}

std::vector<std::shared_ptr<Unit>>& Formation::GetUnits()
{
    return units;
}

bool Formation::AddUnit(const std::shared_ptr<Unit>& unit)
{
    units.push_back(unit);
    return true;
}

bool Formation::RemoveUnit(const std::shared_ptr<Unit>& unit)
{
    auto temp = units.erase(std::remove(units.begin(), units.end(), unit), units.end());
    return true;
}

const glm::vec2& Formation::GetPosition() const
{
    return position;
}

void Formation::SetPosition(const glm::vec2& newPosition)
{
    position = newPosition;
}

void Formation::ResetFormationSpeed()
{
    speed = FlockDataManager::getInstance().GetFormationSpeedFromJson();
}

void Formation::ResetAcceptanceDistance()
{
    acceptanceDistance = FlockDataManager::getInstance().GetFlockVariableFromJson(EnumFlockVariables::acceptanceDistance);
}

const float& Formation::GetRotation() const
{
    return rotation;
}

const float& Formation::AddRotation(const float& angle)
{
    oldRotation = rotation;
    rotation = glm::clamp(rotation + angle, 0.f, 360.f);
    return rotation;
}

void Formation::SetRotation(const float& angle)
{
    oldRotation = rotation;
    rotation = glm::clamp(angle, 0.f, 360.f);
}

void Formation::SetTargetPosition(const glm::fvec2& target)
{
    targetPosition = target;
}

const glm::fvec2& Formation::GetTargetPosition() const
{
    return targetPosition;
}


bool Formation::ReassignGrid()
{
    gridUnits.clear();
    const float cellSize = FlockDataManager::getInstance().GetCellSize();
    for (std::shared_ptr<Unit> unit : units)
    {
        int tempKey = GetUniqueKeyForPosition(unit->GetCurrentPos(), cellSize);
        unit->gridKey = tempKey;
        gridUnits.insert(std::make_pair(tempKey, unit));
    }
    return true;
}

bool Formation::UpdateUnitFormationPositions()
{
    //Calculating the new positions in the formation for all units in the formation using a transform and rotating using the formation leader position as center point.
    const float difference = rotation - oldRotation;
    for (const std::shared_ptr<Unit>& unit : units)
    {
        const auto unitOffset = unit->GetFormationPosOffset();
        sf::Transform transform;
        transform.rotate(difference, sf::Vector2f(position.x, position.y));
        const auto pos = transform.transformPoint(sf::Vector2f(position.x, position.y) + sf::Vector2f(unitOffset.x, unitOffset.y));
        unit->SetTargetFormationPos(glm::fvec2(pos.x, pos.y));
    }
    return true;
}

bool Formation::UpdateMovementUnits(const float& dt)
{
    ReassignGrid();
    UpdateUnitFormationPositions();

    for (const std::shared_ptr<Unit>& unit : units)
    {
        if(!unit->UnitMovement(gridUnits, dt))
        {
            return false;
        }
    }
    return true;
}

int Formation::GetUniqueKeyForPosition(const glm::fvec2& position, const float& cellSize)
{
    return static_cast<int>((15 * glm::floor(position.x / cellSize)) + (17 * glm::floor(position.y / cellSize)));
}
