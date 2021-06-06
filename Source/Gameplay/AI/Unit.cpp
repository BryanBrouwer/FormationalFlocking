#include "Unit.h"

#include <utility>

Unit::Unit()
{

}

Unit::Unit(const glm::fvec2& worldPosition, const glm::fvec2& formationPositionOffset, std::shared_ptr<FlockVariables> FlockVariables)
{
    targetFormationPos = worldPosition;
    currentPos = worldPosition;
    formationPosOffset = formationPositionOffset;
    flockVariables = std::move(FlockVariables);
    //myFormation = formation;
    shape = sf::CircleShape(2.f);
    shape.setFillColor(sf::Color::Green);
    UpdateShape();
}

Unit::~Unit()
{
}

bool Unit::UnitMovement(const std::unordered_multimap<int, std::shared_ptr<Unit>>& unitsGrid, const float& dt)
{
    if (glm::distance(currentPos, targetFormationPos) > flockVariables->acceptanceDistance)
    {
        velocity = FlockingVelocityCalc(unitsGrid);
        const auto nextPosition = currentPos + (velocity * flockVariables->speed * glm::clamp(dt, 0.00f, 0.5f));
        //RotateUnit(nextPosition);
        currentPos = nextPosition;
        UpdateShape();
    }
    return true;
}

bool Unit::UpdateShape()
{
    shape.setPosition(sf::Vector2<float>(currentPos.x, currentPos.y));
    return true;
}

void Unit::RenderUnit(const std::shared_ptr<sf::RenderWindow>& window) const
{
    window->draw(shape);
}

void Unit::SetFormationPosOffset(const glm::fvec2& newFormationPosOffset)
{
    formationPosOffset = newFormationPosOffset;
}

void Unit::SetTargetFormationPos(const glm::fvec2& newFormationPos)
{
    targetFormationPos = newFormationPos;
}

const glm::fvec2& Unit::GetCurrentPos() const
{
    return currentPos;
}

const glm::fvec2& Unit::GetVelocity() const
{
    return velocity;
}

const glm::fvec2& Unit::GetFormationPosOffset() const
{
    return formationPosOffset;
}

const float& Unit::RotateUnit(const glm::fvec2& nextPosition)
{
    const float PI = 3.14159265;
    float dx = nextPosition.x - currentPos.x;
    float dy = nextPosition.y - currentPos.y;
    rotation = (atan2(dy, dx)) * 180 / PI;
    rotation = rotation + 90;
    shape.setRotation(rotation);
    return rotation;
}

const float& Unit::GetRotation() const
{
    return rotation;
}

glm::fvec2 Unit::FlockingVelocityCalc(const std::unordered_multimap<int, std::shared_ptr<Unit>>& unitsGrid)
{
    int amount = 0;
    float angle = 360;
    glm::fvec2 separation = glm::fvec2(0, 0);
    glm::fvec2 alignment = glm::fvec2(0, 0);
    glm::fvec2 cohesion = glm::fvec2(0, 0);

    //For loop that loops over all the units in the current units grid cell
    const auto range = unitsGrid.equal_range(gridKey);
    for (auto pair = range.first; pair != range.second; ++pair) 
    {
        //Check if other unit isn't the same as current unit, and do distance check.
        if (this != pair->second.get() && glm::distance(currentPos, pair->second->GetCurrentPos()) <= flockVariables->perceptionRadius)
        {
            //Calculate angle between the 2 units, and do a field of view check.
            auto otherPos = pair->second->GetCurrentPos();
            angle = glm::acos(glm::dot(velocity, (otherPos - currentPos)) / (glm::length(velocity) * glm::length(otherPos - currentPos)));
            if (glm::abs(angle) <= flockVariables->fieldOfView)
            {
                if (amount >= flockVariables->maxPerceived)
                {
                    break;
                }
                glm::fvec2 directionOtherToOwn = currentPos - otherPos;
                separation += (directionOtherToOwn / glm::distance(currentPos, otherPos));
                alignment += pair->second->GetVelocity();
                amount++;
            }
        }
    }

    //Avarage forces and multiply by force bias
    if (amount > 0)
    {
        //printf("\r\n%i", amount);
        alignment = (alignment / static_cast<float>(amount)) - velocity;
        alignment = glm::normalize(alignment) * flockVariables->alignmentBias;
        separation = (separation / static_cast<float>(amount)) - velocity;
        separation = glm::normalize(separation) * flockVariables->separationBias;
    }

    //if not inFormation it will use a target position set outside of this script to move towards
    if (inFormation)
    {
        cohesion = glm::normalize(targetFormationPos - currentPos) * glm::clamp(glm::distance(currentPos, targetFormationPos), 0.00f, 10.0f) * flockVariables->cohesionBias;
    }
    else
    {
        cohesion = glm::normalize(targetPos - currentPos) * glm::clamp(glm::distance(currentPos, targetPos), 0.00f, 1.0f) * flockVariables->cohesionBias;
    }

    glm::fvec2 forces = (cohesion + separation + alignment);
    velocity = velocity + forces;
    if (velocity != glm::fvec2(0,0))
    {
        velocity = glm::normalize(velocity);
    }

    //printf("\r\n%i", amount);
    return velocity;
}
