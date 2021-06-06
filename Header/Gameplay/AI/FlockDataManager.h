#pragma once
#include <json.hpp>

enum class EnumFlockVariables
{
	cohesionBias = 0,
	separationBias = 1,
	alignmentBias = 2,
	perceptionRadius = 3,
	fieldOfView = 4,
	maxPerceived = 5,
	cellSize = 6,
	acceptanceDistance = 7,
	collisionRange = 8,
	collisionBias = 9,
	maxToCollide = 10,
	speed = 11
};

struct FlockVariables
{
	float cohesionBias;
	float separationBias;
	float alignmentBias;
	float perceptionRadius;
	float fieldOfView;
	int maxPerceived;
	float cellSize;
	float acceptanceDistance;
	float collisionRange;
	float collisionBias;
	int maxToCollide;
	float speed;
	//float variables[11];
};

//Class used to manage the data related to the flocking variables, including reading the variables from the JSON file.
class FlockDataManager
{
public:
	static FlockDataManager& getInstance()
	{
		static FlockDataManager instance; // Guaranteed to be destroyed.
							  // Instantiated on first use.
		return instance;
	}

	FlockDataManager(FlockDataManager const&) = delete;
	void operator=(FlockDataManager const&) = delete;

	//Sets the path to the JSON file used to read the flocking variables from.
	void SetJsonFile(const char* filePath);

	float GetFormationSpeedFromJson();
	float GetFormationSpeed() const;
	void ResetFormationSpeed();

	float GetCellSizeFromJson();
	float GetCellSize() const;
	void ResetCellSize();

	//Gets a specific flocking variable directly from the JSON file without updating the global variable.
    float GetFlockVariableFromJson(EnumFlockVariables neededVariable);

	//Reads the JSON file in order to reset/update all global flocking variables.
	void ResetMainFlockVariables();

	FlockVariables MainFlockVariables{};
private:
    FlockDataManager() = default;
	float formationSpeed{};
	float cellSize{};
	nlohmann::json flockJson;
};
