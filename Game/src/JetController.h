#ifndef JETCONTROLLER_H
#define JETCONTROLLER_H

#include "IAController.h"

class JetController : public IAController
{
public:
	JetController(){type = 1; enemy = NULL;}

	std::vector<Vector3>patrolPoints;
	unsigned int actualPatrolPoint;

	void nextPatrolPoint();

	virtual void init(Vehicle * _vehicle);
	virtual void update(double elapsed_time);

	virtual void setFiring();

	//State0 
	void die(float elapsed_time);
	//State1
	void followAndAttack(float elapsed_time);
	//State2
	void escape(float elapsed_time);
	//State3
	void doPatrol(float elapsed_time);

	bool isInObjective();
	
};

#endif