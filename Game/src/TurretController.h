#ifndef TURRET_H
#define TURRET_H

#include "IAController.h"

class TurretController : public IAController
{
public:
	TurretController(){type = 3; enemy = NULL;}

	float newEnemy;

	virtual void init(Vehicle * _vehicle);
	virtual void update(double elapsed_time);

	virtual void setFiring();

	//virtual void setRotation(double seconds_elapsed);

};

#endif TURRET_H