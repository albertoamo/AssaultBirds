#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Vehicle.h"
#include "game.h"
#include "includes.h"
#include "utils.h"

class Controller{
public:
	Vehicle * toControl;
	Vehicle * enemy;
	Game * game;

	/*
	Type Controllers:

	0: PlayerController
	1: JetController
	2: DestructorController
	3: TurretController
	4: shipController
	*/
	int type;
	unsigned int weaponSelected;
	
	Controller(): game(NULL), toControl(NULL), enemy(NULL), weaponSelected(1){game = Game::instance;}

	~Controller(){}

	virtual void init(Vehicle * _toControl) = 0;

	void instantiateBullet();

	virtual void update(double seconds_elapsed) = 0;
	
	virtual void recibeDeath() = 0;

	virtual void setEnemy(Vehicle * _vehicle){}

	void setType(int _type){type = _type;}

	virtual void setPatrolPoints(std::vector<Vector3>){}
};


#endif