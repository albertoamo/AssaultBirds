#include "world.h"
#include "game.h"
#include <ctime>
#include <algorithm>
#include "init/loader.h"
#include "NodeFactory.h"
#include "Billboard.h"
#include "WorldFactory.h"

//We must changed this before deployment.
int wa_width;
int wa_height;

World::World(){}

World* World::Instance()
{
	if (!instance)
		instance = new World;

	return instance;
}

World::~World(){

	instance = NULL;

	for (auto it = models.begin(); it != models.end(); ++it)
		(*it)->deleteChildren();

	for (auto it = lights.begin(); it != lights.end(); ++it)
		delete *it;
	
	models.clear();
	lights.clear();

	Mesh::deleteMeshes();

	//delete(controllersManager);
	//delete(bulletM);
	//delete(cloud);
	//delete(debugParticle);
}

void World::Init(const char* name){

	toDelete = false;
	deaths = 0;
	enemyBombardiers = 0;
	enemyCommunications = 0;
	enemyHeadcuarters = 0;
	alliedShips = 0;

	this->name = name;
	lights = std::vector< EntityLight* >();
	models = std::vector< EntityMesh* >();
	dynamicModels = std::vector< EntityMesh* >();
	staticModels = std::vector< EntityMesh* >();

	//Different rendering techniques here
	debugParticle = new DebugParticle();
	debugParticle->loadShader("particle.vs", "particle.ps");

	bulletM = new BulletManager();
	cloud = new Sprite();
	cloud->setMaterial("clouds.tga", 2, 2);
	cloud->addStorm(Vector3(-23000, 4400, -23000), Vector3(23000, 6800, 23000), 53);
	
	ScriptPoint * node2 = NodeFactory::Spawn("spawn_allies");
	ScriptPoint * node1 = NodeFactory::Spawn("script_origin");
	//node->attachNode(Vector3(0, 0, 0), Vector3(0, 1, 0));
	//node->attachNode(Vector3(100, 0, 0), Vector3(0, 1, 0));
	nodes.push_back(node1);
	nodes.push_back(node2);

	//controllerManager
	//World::Instance()->nodes[0]->attachNode(Vector3(100, 0, 0), Vector3(0, 1, 0));


	//Here we should intiialize everything from the world
	//Such ass Effects
	//Such ass Sound
	//Such ass Gametype coding

	//The idea comes up when, different worlds might have differents parameters (different fog, diferent ambient)
	//This might load in the future a file containing it's current entitiy positions, effets and other options
	//By now this is coded by hand.

	setExpFog(0.0001, Vector3(0.5, 0.5, 0.5));
	setAmbient(0.4, Vector3(0.9, 0.9, 0.9));

}

void World::update(double elapsedTime)
{
	
	//EntityMesh::rotateAll();

	debugParticle->mesh->vertices.clear();

	//Hacemos los updates de todas las Entities
	updateModels(elapsedTime);

	//Actualizamos todas las balas
	bulletM->update(elapsedTime);

	//llenamos los vectores dynamic i static
	fillModelsVectors();

	//chequeamos colisiones entre entidades
	checkDynamicCollisions();
	checkStaticCollisions();
	checkBulletCollision();

	controllersManager.update(elapsedTime);

	//Actualizamos la orientacion de las nuves
	cloud->update();

	deleteModels();


	//state in this frame
	TotalAllies = alliedShips + (4-deaths);
	TotalEnemies = enemyBombardiers + enemyCommunications + enemyHeadcuarters;

}

void World::loadWorld(int window_width, int window_height){


	//Lights loaded here
	EntityLight * light = new EntityLight(0.5f, 0.3f, Vector3(-6171, 7689, -1441));
	light->setColor(Vector3(0.9, 0.9, 0.9));
	this->lights.push_back(light);

	//Basic models loaded here
	WorldFactory::Spawn(Vector3(0, 0, 0), "ax_skybox");
	WorldFactory::SpawnCollidable(Vector3(0, 0, 0), "ax_island");
	WorldFactory::SpawnCollidable(Vector3(0, 0, 9000), "ax_island");
	WorldFactory::Spawn(Vector3(0, -360, 0), "ax_fondo");
	WorldFactory::SpawnCollidable(Vector3(0, 0, 0), "ax_maria");
	EntityMesh * stalin = WorldFactory::SpawnMeCollidable(Vector3(161, 234, 7679), "ax_stalin");
	stalin->rotateLocal(3.1415, Vector3(0, 1, 0));
	//Bases a destruir
	GroundObjective * firstBase = WorldFactory::SpawnMeGOCollidable(Vector3(1635, 0, 2250), "ax_base");
	firstBase->totalRadius = 50.0;
	enemyHeadcuarters++;
	
	GroundObjective * secondBase = WorldFactory::SpawnMeGOCollidable(Vector3(2422, 0, 8177), "ax_base");
	secondBase->totalRadius = 50.0;
	enemyHeadcuarters++;

	WorldFactory::SpawnGOCollidable(Vector3(-3722,0, 5621), "ax_base");
	firstBase->rotateLocal(3.1415, Vector3(0, 1, 0));
	secondBase->rotateLocal(3.1415, Vector3(0, 1, 0));
	enemyHeadcuarters++;
	

	//Antenas a destruir
	GroundObjective * antenna1 = WorldFactory::SpawnMeGOCollidable(Vector3(-4535, 416, -3172), "ax_antenna");
	antenna1->fallVel = 12.0;
	enemyCommunications++;
	GroundObjective * antenna2 = WorldFactory::SpawnMeGOCollidable(Vector3(-535, 518, 214), "ax_antenna");
	antenna2->fallVel = 15.0;
	enemyCommunications++;
	

	//Radares a destruir
	WorldFactory::SpawnGOCollidable(Vector3(3118, 388, 70), "ax_radar");enemyCommunications++;
	WorldFactory::SpawnGOCollidable(Vector3(-2574, 343, -3058), "ax_radar");enemyCommunications++;
	WorldFactory::SpawnGOCollidable(Vector3(-1507, 434, 8050), "ax_radar");enemyCommunications++;
	WorldFactory::SpawnGOCollidable(Vector3(-4522, 409, 5808), "ax_radar");enemyCommunications++;

}

void World::doSpawn(Vector3 position){
	
	//Here the players are spawned
	for(unsigned int i = 0; i < 3; i++)
	{
		Vector3 aleatori;
		aleatori.random(500);
		aleatori = aleatori + Vector3(0,1000,0);
		Vehicle * plane2 = AirplaneFactory::newF18Plane(aleatori, Vector3(0,1,0),2 * M_PI, "Allied");
		models.push_back(plane2);
		controllersManager.setJetControl(plane2, 1);
	}

	for(unsigned int i = 0; i < 10; i++)
	{
		Vector3 aleatori;
		aleatori.random(500);
		aleatori = aleatori + Vector3(0,1000,0);
		Vehicle * plane2 = AirplaneFactory::newF18Plane(aleatori, Vector3(0,1,0),2 * M_PI, "Enemy");
		models.push_back(plane2);
		controllersManager.setJetControl(plane2, 2);
	}
	
	Vehicle * ship4 = ShipFactory::newCarrierShip(Vector3(1775, -20, -3823), Vector3(0,1,0), (2 * M_PI), "base");
	models.push_back(ship4);

	Vehicle * plane = AirplaneFactory::newF18Plane(ship4->Model.getTranslationOnly() + Vector3(2, 18, -198), Vector3(0,1,0), (2 * M_PI)-0.1, "Allied");
	models.push_back(plane);
	controllersManager.setPlayerControl(plane, 1);
	
	Vehicle * bombardier = AirplaneFactory::newN12Plane(Vector3(0, 1000, -2500), Vector3(0,1,0), 2 * M_PI, "Objective");
	models.push_back(bombardier);
	controllersManager.setBombardierControl(bombardier, 2);
	enemyBombardiers++;

	Vehicle * bombardier2 = AirplaneFactory::newN12Plane(Vector3(100, 1000, -2450), Vector3(0,1,0), 2 * M_PI, "Objective");
	models.push_back(bombardier2);
	controllersManager.setBombardierControl(bombardier2, 2);
	enemyBombardiers++;

	Vehicle * bombardier3 = AirplaneFactory::newN12Plane(Vector3(-100, 1000, -2450), Vector3(0,1,0), 2 * M_PI, "Objective");
	models.push_back(bombardier3);
	controllersManager.setBombardierControl(bombardier3, 2);
	enemyBombardiers++;
	
	Vehicle * ship = ShipFactory::newWaspShip(Vector3(5500.5, -20, 5480), Vector3(0,1,0),2 * M_PI, "toDefend");
	models.push_back(ship);
	controllersManager.setShipController(ship, 1);
	alliedShips++;

	Vehicle * ship2 = ShipFactory::newFragataShip(Vector3(6070.5, -20, 5480), Vector3(0,1,0),2 * M_PI, "toDefend");
	models.push_back(ship2);
	controllersManager.setShipController(ship2, 1);
	alliedShips++;

	Vehicle * ship3 = ShipFactory::newFragataShip(Vector3(6500, -20, 5480), Vector3(0,1,0),2 * M_PI, "toDefend");
	models.push_back(ship3);
	controllersManager.setShipController(ship3, 1);
	alliedShips++;

	std::vector<Vector3> PatrolPoints; PatrolPoints.push_back(Vector3(-6776, -202, 1459));
	ship->control->setPatrolPoints(PatrolPoints);
	ship2->control->setPatrolPoints(PatrolPoints);
	ship3->control->setPatrolPoints(PatrolPoints);
	
	Vehicle * turret = TurretFactory::newBasicTurret(Vector3(-541, 6, 2431), Vector3(0,1,0),2 * M_PI, "Turret");
	models.push_back(turret);
	controllersManager.setTurretControl(turret, 2);
	turret->control->setEnemy(plane);
	
	Vehicle * turret1 = TurretFactory::newBasicTurret(Vector3(-739, -5, 2408), Vector3(0,1,0),2 * M_PI, "Turret");
	models.push_back(turret1);
	controllersManager.setTurretControl(turret1, 2);

	Vehicle * turret2 = TurretFactory::newBasicTurret(Vector3(-959, 2, 2382), Vector3(0,1,0),2 * M_PI, "Turret");
	models.push_back(turret2);
	controllersManager.setTurretControl(turret2, 2);

	Vehicle * turret3 = TurretFactory::newBasicTurret(Vector3(-2467, 10, 5127), Vector3(0,1,0),2 * M_PI, "Turret");
	models.push_back(turret3);
	controllersManager.setTurretControl(turret3, 2);

	Vehicle * turret4 = TurretFactory::newBasicTurret(Vector3(-2880, 10, 4965), Vector3(0,1,0),2 * M_PI, "Turret");
	models.push_back(turret4);
	controllersManager.setTurretControl(turret4, 2);
	
	//Special entitymeshes with special techniques loaded here.
	EntityMesh * water = loadModel(Vector3(0, -30, 0), "ax_water.axmodel");
	this->models.push_back(water);
	
	//ShadowMapping * techShadow = new ShadowMapping(GL_TRIANGLES, "simple");
	//techShadow->init(this->models, this->lights);
	//techniques.push_back(techShadow);

	Reflection * techWater = new Reflection(GL_TRIANGLES, "reflector");
	techWater->init(Game::instance->camera, water); 
	techniques.push_back(techWater);

	ForwardRendering * mainRender = new ForwardRendering(GL_TRIANGLES, "simple");
	mainRender->init(this);
	techniques.push_back(mainRender);

	MinimapRendering * minimapRender = new MinimapRendering(GL_TRIANGLES, "minimap");
	minimapRender->init(this);
	techniques.push_back(minimapRender);

}

void World::renderWorld(Camera * camera, Renderer * renderer){

	for (unsigned int x = 0; x < techniques.size(); x++){
		techniques[x]->update(Game::instance->camera); //update it to avoid blinking 
		techniques[x]->render(renderer); //Rendering the technique
	}

}

void World::setExpFog(float fogDen, Vector3 color){

	this->fog.fogDen = fogDen;
	this->fog.fogColor = color;
}
void World::setAmbient(float ambCons, Vector3 ambColor){

	this->ambient.ambCons = ambCons;
	this->ambient.ambColor = ambColor;
}

void World::checkDynamicCollisions()
{
	//Recorremos todas las entidades comprobando solo una vez cada pareja
	for(unsigned int i = 0; i < dynamicModels.size(); i++){
		for(unsigned int j = i+1; j < dynamicModels.size(); j++){
			//Si la priemra tiene CollisionModel (queremos que sea minimamente preciso)
			if(dynamicModels[i]->collision_model != NULL)
			{
				if(dynamicModels[i]->collision_model->sphereCollision(dynamicModels[j]->origin.v, dynamicModels[j]->totalRadius))
				{
					dynamicModels[i]->onCollision(dynamicModels[j]);
					dynamicModels[j]->onCollision(dynamicModels[i]);
				}
			}
			else{
				//Si la segunda tiene CollisionModel (queremos que sea minimamente preciso)
				if(dynamicModels[j]->collision_model != NULL){
					if(dynamicModels[j]->collision_model->sphereCollision(dynamicModels[i]->origin.v, dynamicModels[i]->totalRadius))
					{
						dynamicModels[i]->onCollision(dynamicModels[j]);
						dynamicModels[j]->onCollision(dynamicModels[i]);
					}
				}
				else
					//Si ninguna de ellas tiene colision model comprobamos colision con las esferas
					if(SphereSphereCollision(dynamicModels[i]->origin.v, dynamicModels[i]->totalRadius, dynamicModels[j]->origin.v, dynamicModels[j]->totalRadius, Vector3(0,0,0).v))
					{
						dynamicModels[i]->onCollision(dynamicModels[j]);
						dynamicModels[j]->onCollision(dynamicModels[i]);
					}
			}
		}
	}
}

void World::checkStaticCollisions()
{
	for(unsigned int i = 0; i < dynamicModels.size(); i++)
		for(unsigned int j = 0; j < staticModels.size(); j++)
			if(staticModels[j]->collision_model!=NULL){
				if(staticModels[j]->collision_model->sphereCollision(dynamicModels[i]->origin.v, dynamicModels[i]->totalRadius))
				{
					dynamicModels[i]->onCollision(staticModels[j]);
					staticModels[j]->onCollision();
				}
				if(dynamicModels[i]->origin.y < -25.0f)
				{
					dynamicModels[i]->onCollision(staticModels[j]);
				}
			}
}

void World::checkBulletCollision()
{
	for(auto bullet = bulletM->bullets_vector.begin(); bullet != bulletM->bullets_vector.end(); bullet++)
	{
		if(bullet->ttl < 0.0f)
			continue;

		Vector3 direction = (bullet->lastPosition - bullet->position).normalize();
		Vector3 positionCol;

		for(unsigned int i = 0; i < staticModels.size(); i++)
		{
			if(staticModels[i]->collision_model != NULL)
			{
				if(staticModels[i]->collision_model->rayCollision(bullet->lastPosition.v, direction.v, true, 0.0f, (bullet->position - bullet->lastPosition).length()))
				{
					staticModels[i]->bulletCollision(*bullet);
					staticModels[i]->collision_model->getCollisionPoint(positionCol.v, false);
					debugParticle->addParticle(positionCol);
					bullet->ttl = -1.0f;
				}
			}
		}
		
		for(unsigned int i = 0; i < dynamicModels.size(); i++)
		{
			if(SphereSegmentCollision(dynamicModels[i]->origin.v, dynamicModels[i]->totalRadius, bullet->lastPosition.v, direction.v, bullet->position.v, positionCol.v) && dynamicModels[i]->id != bullet->author)
			{
				if(controllersManager.checkIsPlayer(bullet->author))
				{
					SoundManager::Instance()->playSound(HIT);
				}
				debugParticle->addParticle(positionCol);
 				dynamicModels[i]->bulletCollision(*bullet);
			}
		}
	}
}

void World::deleteModels()
{
	std::set<unsigned int> aux;
    for(unsigned int i = 0; i < models.size(); i++)
    {
        if(models[i]->toDelete){
			entitiesToDelete.insert(models[i]);
			aux.insert(i);
			controllersManager.deleteController(models[i]->id);
		}
    }

	for(auto it = aux.begin(); it != aux.end(); it++)
	{
		std::string name = std::string(models[*it]->Name);
		if(name == "Allied")
		{
			Vehicle * veh = (Vehicle*)models[*it];
			if(veh->subType == PLANE)
				deaths++;
		}
		if(name == "ax_base")
			enemyHeadcuarters--;
		if(name == "Objective")
			enemyBombardiers--;
		if(name == "toDefend")
			alliedShips--;
		if(name == "ax_antenna" || name == "ax_radar")
			enemyCommunications--;
		delete(models[*it]);
	}

    for(auto it = entitiesToDelete.begin(); it != entitiesToDelete.end(); it++)
	{
		models.erase(std::remove(models.begin(), models.end(), *it), models.end());
	}

	entitiesToDelete.clear();
	aux.clear();
}

void World::updateModels(float elapsedTime)
{
	for(auto it = models.begin(); it != models.end(); it++)
	{
		(*it)->update(elapsedTime);
	}

}

void World::fillModelsVectors()
{
	dynamicModels.clear();
	staticModels.clear();

	for (unsigned int i = 0; i < models.size(); i++)
	{
		if(models[i]->options & 0x02)
			dynamicModels.push_back(models[i]);
		else
			staticModels.push_back(models[i]);
	}	
}
