#ifndef BOMBARDIERCONTROLLER_H
#define BOMBARDIERCONTROLLER_H

#include "IAController.h"

class BombardierController : public IAController
{
public:
	BombardierController(){type = 2; enemy = NULL;}
	
	std::vector<Vector3>patrolPoints;
	unsigned int actualPatrolPoint;

	void nextPatrolPoint();

	virtual void init(Vehicle * _vehicle);
	virtual void update(double elapsed_time);

	//State0 
	void die(float elapsed_time);
	//State3
	void doPatrol(float elapsed_time);

	bool isInObjective();

};
#endif