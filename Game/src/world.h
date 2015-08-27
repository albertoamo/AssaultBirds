#ifndef WORLD_H
#define WORLD_H
#include "includes.h"

#include "Entity.h"
#include "EntityLight.h"
#include "EntityMesh.h"
#include "material.h"
#include "camera.h"
#include "ControllerManager.h"
#include "BulletManager.h"
#include "Billboard.h"
#include "Vehicle.h"
#include "GroundObjective.h"
#include "AirplaneFactory.h"
#include "TurretFactory.h"
#include "ShipFactory.h"
#include "Technique.h"
#include <set>

//Structs for fog and ambient
typedef struct Fog{

	float fogDen;
	Vector3 fogColor;

};

typedef struct Ambient{

	float ambCons;
	Vector3 ambColor;

};

//This class should be Singleton pattern (IMPLEMENT THIS IN THE FUTURE)

class World
{
public:

	bool toDelete;
	int deaths;
	int enemyCommunications;
	int enemyHeadcuarters;
	int enemyBombardiers;
	int alliedShips;

	int TotalAllies;
	int TotalEnemies;

	const char* name;
	std::vector< EntityLight* > lights; //here we store the entities
	std::vector< EntityMesh* > models; //here we store the entities

	std::vector< EntityMesh* > dynamicModels;
	std::vector< EntityMesh* > staticModels;
	std::set< EntityMesh* > entitiesToDelete;

	ControllerManager controllersManager;

	BulletManager * bulletM;

	std::vector< Technique* > techniques;
	std::vector< ScriptPoint* > nodes;
	DebugParticle* debugParticle;
	Sprite * cloud;

	Fog fog;
	Ambient ambient;

	~World();

	static World* Instance();
	void Init(const char* name);
	void update(double elapsedTime);
	void loadWorld(int window_width, int window_height);
	void renderWorld(Camera * camera, Renderer * renderer);
	void doSpawn(Vector3 position = Vector3(0,0,0));

	void setExpFog(float fogDen, Vector3 color);
	void setAmbient(float ambCons, Vector3 ambColor);

	private:
		World();
		World(World const&);              // Don't Implement.
		void operator=(World const&); // Don't implement

		static World * instance;

		void updateModels(float elapsedTime);

		void fillModelsVectors();

		void checkDynamicCollisions();
		void checkStaticCollisions();

		void checkBulletCollision();
		void deleteModels();
};

#endif WORLD_H