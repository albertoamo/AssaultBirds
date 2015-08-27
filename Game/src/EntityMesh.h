#ifndef ENTITYMESH_H
#define ENTITYMESH_H

#include "utils.h"
#include "Entity.h"
#include "framework.h"
#include "mesh.h"
#include "includes.h"
#include "shader.h"
#include "camera.h"
#include "EntityLight.h"
#include "texture.h"
#include "material.h"


#include "coldet/coldet.h"

class Bullet;
class RenderStack;
class EntityMesh;

typedef struct{
	EntityMesh * entity;
	Vector3 orient;
	float time;
	float previoustime;
}Rotaters;

class EntityMesh : public Entity{

public: 
	//Atributos
	Mesh * mesh;
	Mesh * lowMesh;
	Material * material;

	static std::vector<Rotaters> rotaters;

	//Bounding boxes struct
	struct{
		Mesh BoundingBoxMesh;
		float totalRadius;
		Vector3 maxs;
		Vector3 mins;
		Vector3 BBcolor;
	};

	CollisionModel3D* collision_model;

	EntityMesh(){}
	EntityMesh(const char* name, Material * material, Vector3 origin);

	~EntityMesh();

	bool loadMesh(const char* filename);
	bool loadLowMesh(const char* filename);

	//Flag methods
	void enableFlags();
	void disableFlags();

	//Basic drawing methods
	void drawNormals(Vector3 color, float distance);
	void render(int primitives, Camera * camera, EntityLight light);
	void render(int primitives, RenderStack * stack);
	virtual void update(float elapsedTime){actualizeCollisionModel();};

	void calculateBB();
	void calculateTotalRadius();
	void drawBB(Matrix44 viewproj);

	void buildCollisionTree();
	void actualizeCollisionModel();

	virtual void bulletCollision(Bullet & bullet);
	virtual void onCollision(EntityMesh * _entity){}
	virtual void onCollision(){}

	static void setRotation(EntityMesh * entity, Vector3 direction, float time);
	static void rotateAll();
	void rotateOverTime(float & previous, float time, Vector3 direction);
	void rotateOverAngle(float angle, Vector3 direction);
};

#endif