#ifndef WORLDFACTORY_H
#define WORLDFACTORY_H

#include "world.h"

class WorldFactory {
public:
	//EntityMeshes
	static void Spawn(Vector3 position, std::string name){
		EntityMesh * model = loadModel(position, (name + ".axmodel").c_str());
		World::Instance()->models.push_back(model);
	}
	static void SpawnCollidable(Vector3 position, std::string name){
		EntityMesh * model = loadModel(position, (name + ".axmodel").c_str());
		model->calculateTotalRadius();
		model->buildCollisionTree();
		World::Instance()->models.push_back(model);
	}
	//This ones return the object
	static EntityMesh * SpawnMe(Vector3 position, std::string name){
		EntityMesh * model = loadModel(position, (name + ".axmodel").c_str());
		World::Instance()->models.push_back(model);

		return model;
	}
	static EntityMesh * SpawnMeCollidable(Vector3 position, std::string name){
		EntityMesh * model = loadModel(position, (name + ".axmodel").c_str());
		model->calculateTotalRadius();
		model->buildCollisionTree();
		World::Instance()->models.push_back(model);

		return model;
	}

	//GroundObjectives
		//This ones do not return the object
	static void SpawnGO(Vector3 position, std::string name){
		GroundObjective * model = loadModelGO(position, (name + ".axmodel").c_str());
		World::Instance()->models.push_back(model);
	}
	static void SpawnGOCollidable(Vector3 position, std::string name){
		GroundObjective * model = loadModelGO(position, (name + ".axmodel").c_str());
		model->calculateTotalRadius();
		model->buildCollisionTree();
		World::Instance()->models.push_back(model);
	}
	//This ones return the object
	static GroundObjective * SpawnMeGO(Vector3 position, std::string name){
		GroundObjective * model = loadModelGO(position, (name + ".axmodel").c_str());
		World::Instance()->models.push_back(model);

		return model;
	}
	static GroundObjective * SpawnMeGOCollidable(Vector3 position, std::string name){
		GroundObjective * model = loadModelGO(position, (name + ".axmodel").c_str());
		model->calculateTotalRadius();
		model->buildCollisionTree();
		World::Instance()->models.push_back(model);

		return model;
	}


};
#endif
