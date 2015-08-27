#ifndef AIRPLANE_H
#define AIRPLANE_H

#include "EntityMesh.h"
#include "Canon.h"

class Airplane
{
public:
	EntityMesh * plane;
	
	float speed;
	float maxSpeed;

	//Bools de control
	bool isacelerating;
	bool isFiring;

	Airplane(EntityMesh * _toC);
	void update(double elapsedTime);
	void setFiring();
	void setMaxSpeed(float maxSpeed);
	void instantiateBullet();
	void addCanon(Vector3 pos, float power, float vel);

};

#endif