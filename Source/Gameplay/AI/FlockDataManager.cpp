#include "FlockDataManager.h"
#include <iostream>
#include <fstream>

void FlockDataManager::SetJsonFile(const char* filePath)
{
    std::ifstream traitFile(filePath, std::ifstream::binary);
    traitFile >> flockJson;
}

float FlockDataManager::GetFormationSpeedFromJson()
{
    return flockJson["formationSpeed"].get<float>();
}

float FlockDataManager::GetFormationSpeed() const
{
    return formationSpeed;
}

void FlockDataManager::ResetFormationSpeed()
{
    formationSpeed = flockJson["formationSpeed"].get<float>();
}

float FlockDataManager::GetCellSizeFromJson()
{
    return flockJson["cellSize"].get<float>();
}

float FlockDataManager::GetCellSize() const
{
    return cellSize;
}

void FlockDataManager::ResetCellSize()
{
    cellSize = flockJson["cellSize"].get<float>();
}

float FlockDataManager::GetFlockVariableFromJson(const EnumFlockVariables neededVariable)
{
    float returnFloat = 0.f;

    switch (neededVariable)
    {
    case EnumFlockVariables::cohesionBias:
        returnFloat = flockJson["cohesionBias"].get<float>();
        break;
    case EnumFlockVariables::separationBias:
        returnFloat = flockJson["separationBias"].get<float>();
        break;
    case EnumFlockVariables::alignmentBias:
        returnFloat = flockJson["alignmentBias"].get<float>();
        break;
    case EnumFlockVariables::perceptionRadius:
        returnFloat = flockJson["perceptionRadius"].get<float>();
        break;
    case EnumFlockVariables::fieldOfView:
        returnFloat = flockJson["fieldOfView"].get<float>();
        break;
    case EnumFlockVariables::maxPerceived:
        returnFloat = flockJson["maxPerceived"].get<float>();
        break;
    case EnumFlockVariables::cellSize:
        returnFloat = flockJson["cellSize"].get<float>();
        break;
    case EnumFlockVariables::acceptanceDistance:
        returnFloat = flockJson["acceptanceDistance"].get<float>();
        break;
    case EnumFlockVariables::collisionRange:
        returnFloat = flockJson["collisionRange"].get<float>();
        break;
    case EnumFlockVariables::collisionBias:
        returnFloat = flockJson["collisionBias"].get<float>();
        break;
    case EnumFlockVariables::maxToCollide:
        returnFloat = flockJson["maxToCollide"].get<float>();
        break;
    case EnumFlockVariables::speed:
        returnFloat = flockJson["speed"].get<float>();
        break;
    }

    return returnFloat;
}

void FlockDataManager::ResetMainFlockVariables()
{
    //int test = static_cast<int>(EnumFlockVariables::alignmentBias);
    //for (int i = 0; i < 11; i++)
    //{
    //    GlobalFlockVariables.variables[i] = flockJson[i].get<float>();
    //}

    MainFlockVariables.cohesionBias = flockJson["cohesionBias"].get<float>();
    MainFlockVariables.separationBias = flockJson["separationBias"].get<float>();
    MainFlockVariables.alignmentBias = flockJson["alignmentBias"].get<float>();
    MainFlockVariables.perceptionRadius = flockJson["perceptionRadius"].get<float>();
    MainFlockVariables.fieldOfView = flockJson["fieldOfView"].get<float>();
    MainFlockVariables.maxPerceived = flockJson["maxPerceived"].get<int>();
    MainFlockVariables.cellSize = flockJson["cellSize"].get<float>();
    MainFlockVariables.acceptanceDistance = flockJson["acceptanceDistance"].get<float>();
    MainFlockVariables.collisionRange = flockJson["collisionRange"].get<float>();
    MainFlockVariables.collisionBias = flockJson["collisionBias"].get<float>();
    MainFlockVariables.maxToCollide = flockJson["maxToCollide"].get<int>();
    MainFlockVariables.speed = flockJson["speed"].get<float>();

}
