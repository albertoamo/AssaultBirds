#ifndef GOUNDOBJECTIVE_H
#define GOUNDOBJECTIVE_H

#include "utils.h"
#include "framework.h"
#include "EntityMesh.h"

enum SUBTYPE_GR_OB
{
	ANTENNA = 0,
	RADAR = 1
};

class GroundObjective : public EntityMesh
{
public:
	int subtype;
	float life;

	float fallVel;

	GroundObjective();
	GroundObjective(const char* name, Material * material, Vector3 origin);

	float dieTime;
	//void die(float elapsed_time);

	virtual void update(float elapsed_time);
	virtual void bulletCollision(Bullet & bullet);
	virtual void onCollision();
};

#endif