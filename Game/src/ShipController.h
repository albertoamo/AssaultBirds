#ifndef SHIPCONTROLLER_H
#define SHIPCONTROLLER_H

#include "Vehicle.h"
#include "IAController.h"

class ShipController : public IAController
{
public:
	ShipController(){type = 4;}

	virtual void init(Vehicle * _vehicle);
	virtual void update(double elapsed_time);

	std::vector<Vector3>patrolPoints;
	unsigned int actualPatrolPoint;

	void nextPatrolPoint();

	void die(float elapsed_time);
	void doPatrol(float elapsed_time);
	bool isInObjective();

	void setPatrolPoints(std::vector<Vector3> _patrolPoints);
	void pushPatrolPoint(Vector3 _newPoint);

};

#endif