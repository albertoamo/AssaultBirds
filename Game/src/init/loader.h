#ifndef LOADER_H
#define LOADER_H
#include "../includes.h"
#include "../framework.h"
#include "../Entity.h"
#include "../EntityMesh.h"
#include "../Vehicle.h"
#include "../GroundObjective.h"

//This code parses an actual scene or collection of meshes in it's own format .axmodel
//The format is very simple, just a tree of all the meshes that were parsed from the .obj, each children has it's own material linked to it
//Basic structure to hold on temp meshes

typedef struct auxModel{

	std::string name;
	std::string material;
	unsigned int children;
	Vector3 origin;

	auxModel(std::string name, std::string material, unsigned int children, Vector3 origin){
		this->name = name;
		this->material = material;
		this->children = children;
		this->origin = origin;
	}
};

//Data reads of material and vertices
Material * loadMat(const char* filename);

//ENTITYMESH
EntityMesh* readData(FILE * file, auxModel * model);

//Main read recursive functions, this loads each children and puts it in the correct place
void readChildren(FILE * file, auxModel * model, EntityMesh* root);
EntityMesh * loadModel(Vector3 position, const char* filename);
EntityMesh * loadEntity(FILE * file, Vector3 position);

//Vehicle
Vehicle* readDataEP(FILE * file, auxModel * model);

//Main read recursive functions, this loads each children and puts it in the correct place
void readChildrenEP(FILE * file, auxModel * model, Vehicle* root);
Vehicle * loadModelEP(Vector3 position, const char* filename);
Vehicle * loadEntityEP(FILE * file, Vector3 position);


//Ground Objective
GroundObjective* readDataGO(FILE * file, auxModel * model);

//Main read recursive functions, this loads each children and puts it in the correct place
void readChildrenGO(FILE * file, auxModel * model, GroundObjective* root);
GroundObjective * loadModelGO(Vector3 position, const char* filename);
GroundObjective * loadEntityGO(FILE * file, Vector3 position);

#endif