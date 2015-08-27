#include "JetController.h"

void JetController::init(Vehicle * _toControl)
{
	//Para generar luego un estado aleatorio
	//srand(time(NULL));

	maxTimeState = 0.0f;

	toControl = _toControl;

	toControl->setController(this);

	state = 3;

	//Seteamos puntos random para patrullar
	Vector3 PatrolPointDefault;
	PatrolPointDefault.random(1000);
	patrolPoints.push_back(PatrolPointDefault+Vector3(0,1000,0));
	PatrolPointDefault.random(1000);
	patrolPoints.push_back(PatrolPointDefault+Vector3(0,1000,0));
	PatrolPointDefault.random(1000);
	patrolPoints.push_back(PatrolPointDefault+Vector3(0,1000,0));
	PatrolPointDefault.random(1000);
	patrolPoints.push_back(PatrolPointDefault+Vector3(0,1000,0));
	
	/*
	patrolPoints.push_back(Vector3(-600, 1000, -600));
	patrolPoints.push_back(Vector3(600, 1000, -600));
	patrolPoints.push_back(Vector3(600, 1000, 600));
	patrolPoints.push_back(Vector3(-600, 1000, 600));
	*/
	actualPatrolPoint = 0;
	setObjectivePoint(patrolPoints[actualPatrolPoint ]);


}

void JetController::update(double elapsed_time)
{
		const Uint8* keystate = game->keystate;
	
		if (keystate[SDL_SCANCODE_F]) objectivePoint.x += 300*elapsed_time;
		if (keystate[SDL_SCANCODE_H]) objectivePoint.x -= 300*elapsed_time;
		if (keystate[SDL_SCANCODE_T]) objectivePoint.y += 300*elapsed_time;
		if (keystate[SDL_SCANCODE_G]) objectivePoint.y -= 300*elapsed_time;
		if (keystate[SDL_SCANCODE_R]) objectivePoint.z += 300*elapsed_time;
		if (keystate[SDL_SCANCODE_Y]) objectivePoint.z -= 300*elapsed_time;

		setStates();

		switch(state)
		{
		case 0:
			die(elapsed_time);
			break;
		case 1: 
			escape(elapsed_time);
			break;
		case 2:
			followAndAttack(elapsed_time);
			break;
		case 3:
			doPatrol(elapsed_time);
			break;
		default:
			doPatrol(elapsed_time);
			break;
		}
}

bool JetController::isInObjective()
{
	if(SphereSphereCollision(toControl->origin.v, toControl->totalRadius, objectivePoint.v, 25, Vector3(0,0,0).v))
		return true;

	return false;
}


void JetController::die(float elapsed_time)
{
	objectivePoint.y -= 300*elapsed_time;
	toControl->maxSpeed *= 2;
	setRotation(elapsed_time);
}

void JetController::followAndAttack(float elapsed_time)
{
	if( enemy != NULL){
		if(enemy->life > 0.0){
			setObjectivePoint(enemy->Model.getTranslationOnly());
		}
	}

	setRotation(elapsed_time);
	setFiring();

	maxTimeState += elapsed_time;
}

void JetController::escape(float elapsed_time)
{
	if(maxTimeState < 0.5)
	{
		Vector3 aleatori;
		aleatori.random(600);
		aleatori = aleatori + Vector3(0,1000,0);
		setObjectivePoint(aleatori);
	}

	setRotation(elapsed_time);

	maxTimeState += elapsed_time;
}

void JetController::doPatrol(float elapsed_time)
{
	setRotation(elapsed_time);
	if((objectivePoint-toControl->origin).length() < 10.0)
		nextPatrolPoint();
	
	maxTimeState += elapsed_time;
}

void JetController::nextPatrolPoint()
{
	actualPatrolPoint++;
	if(actualPatrolPoint == patrolPoints.size())
		actualPatrolPoint = 0;
	objectivePoint = patrolPoints[actualPatrolPoint];
}

void JetController::setFiring()
{
	if(enemy != NULL)
		if(to_target.length() < 800 && UpDownAngle > -0.1 && UpDownAngle < 0.1 && RightLeftAngle > -0.1 && RightLeftAngle < 0.1)
			toControl->isFiring = true;
}

void IAController::setStates()
{
	if(toControl->life < 0.0)
	{
		//nunca saldrá de state=0 (die) porque es el único estado que no suma elapsedTime a maxTimeState
		 state = 0;
	}

	if(maxTimeState > 15.0f)
	{
		state++;
		enemy = NULL;
		if(state == 4)
			if(toControl->life < 20.0)
				state = 1;
			else
				state = 2;
		maxTimeState = 0.0f;
	}
}
