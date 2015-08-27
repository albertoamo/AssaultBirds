#include "ShipController.h"

void ShipController::init(Vehicle * _vehicle)
{
	this->toControl = _vehicle;
	toControl->setController(this);

	patrolPoints.push_back(Vector3(3, -20, -4990));
	patrolPoints.push_back(Vector3(3770, -20, -2375));
	patrolPoints.push_back(Vector3(4549, -20, -6085));
	patrolPoints.push_back(Vector3(1461, -20, -6181));
	actualPatrolPoint = 0;
	setObjectivePoint(patrolPoints[actualPatrolPoint]);

}

void ShipController::update(double elapsed_time)
{
	if(toControl->life < 0.0)
		die(elapsed_time);
	else
		doPatrol(elapsed_time);
}

void ShipController::doPatrol(float elapsed_time)
{
	setRotation(elapsed_time);
	if((objectivePoint-toControl->origin).length() < 10.0)
		nextPatrolPoint();
}

void ShipController::nextPatrolPoint()
{
	actualPatrolPoint++;
	if(actualPatrolPoint == patrolPoints.size())
		actualPatrolPoint = 0;
	objectivePoint = patrolPoints[actualPatrolPoint];
}

void ShipController::die(float elapsed_time)
{
	toControl->maxSpeed = 0.0;
	toControl->minSpeed = 0.0;
	toControl->rotateLocal(elapsed_time/3, Vector3(0,0,1));
	toControl->Model.traslate(0, -elapsed_time*5, 0);
	if(toControl->origin.y < -50)
		toControl->toDelete = true;
}

bool ShipController::isInObjective()
{
	if(SphereSphereCollision(toControl->origin.v, toControl->totalRadius, objectivePoint.v, 25, Vector3(0,0,0).v))
		return true;

	return false;
}

void ShipController::setPatrolPoints(std::vector<Vector3> _patrolPoints)
{
	this->patrolPoints = _patrolPoints;
	actualPatrolPoint = 0;
	objectivePoint = patrolPoints[actualPatrolPoint];
}

void ShipController::pushPatrolPoint(Vector3 _newPoint)
{
	this->patrolPoints.push_back(_newPoint);	
}