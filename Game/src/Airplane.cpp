#include "Airplane.h"

float clamp(float var, float max, float min)
{
	return var < max ? max : (var > min ? min : var);
}

Airplane::Airplane(EntityMesh * _toC):
plane(_toC), maxSpeed(300.0f), isFiring(false), isacelerating(false){}

void Airplane::setFiring(){ isFiring = true;}

void Airplane::instantiateBullet()
{
	Vector3 direction = plane->Model.rotateVector(Vector3(0,0,1));
	Vector3 canon = plane->Model.rotateVector(Vector3(1,0,0))*4;
}

void Airplane::update(double elapsedTime)
{
	speed = clamp(speed, 0, maxSpeed);

	if(isFiring)
		plane->processEvent("fire");

	isFiring = false;
}

void Airplane::addCanon(Vector3 pos, float power, float vel)
{
	Canon * newCanon = new Canon(pos, power, vel);
	plane->addChildren((Entity*)newCanon);
}