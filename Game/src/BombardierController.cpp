#include "BombardierController.h"


void BombardierController::init(Vehicle * _toControl)
{

	toControl = _toControl;

	toControl->setController(this);

	//Seteamos puntos random para patrullar
	patrolPoints.push_back(toControl->origin + (toControl->Model.rotateVector(Vector3(0,0,1))*10000));
	patrolPoints.push_back(toControl->origin + (toControl->Model.rotateVector(Vector3(1,0,1))*10000));

	actualPatrolPoint = 0;
	setObjectivePoint(patrolPoints[actualPatrolPoint ]);
}

void BombardierController::update(double elapsed_time)
{
	if(toControl->life < 0.0)
		die(elapsed_time);
	else
		doPatrol(elapsed_time);
}

bool BombardierController::isInObjective()
{
	if(SphereSphereCollision(toControl->origin.v, toControl->totalRadius, objectivePoint.v, 25, Vector3(0,0,0).v))
		return true;

	return false;
}

void BombardierController::die(float elapsed_time)
{
	//Hacemos que se dirija hacia abajo aumentando su velocidad 
	objectivePoint.y -= 300*elapsed_time;
	toControl->accelerate(50*elapsed_time);
	toControl->minSpeed += elapsed_time*15;
	toControl->maxSpeed += elapsed_time*15;
	setRotation(elapsed_time);
}

void BombardierController::doPatrol(float elapsed_time)
{
	setRotation(elapsed_time);
	if((objectivePoint-toControl->origin).length() < 10.0)
		nextPatrolPoint();
	
	maxTimeState += elapsed_time;
}

void BombardierController::nextPatrolPoint()
{
	actualPatrolPoint++;
	if(actualPatrolPoint == patrolPoints.size())
		actualPatrolPoint = 0;
	objectivePoint = patrolPoints[actualPatrolPoint];
}