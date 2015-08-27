#ifndef CONTROLLERMANAGER_H
#define CONTROLLERMANAGER_H

#include "Controller.h"
#include "PlayerController.h"
#include "JetController.h"
#include "TurretController.h"
#include "ShipController.h"
#include "BombardierController.h"

class ControllerManager{
public:

	//Mapea ID de la nave con ID del equipo
	static std::multimap<unsigned int, unsigned int> teamsMap;
	static std::vector<Controller*>controllers;

	ControllerManager(){}
	~ControllerManager();

	void setJetControl(Vehicle * _vehicle, unsigned int _team);
	void setPlayerControl(Vehicle * _vehicle, unsigned int _team);
	void setTurretControl(Vehicle * _vehicle, unsigned int _team);
	void setShipController(Vehicle * _vehicle, unsigned int _team);
	void setBombardierControl(Vehicle * _vehicle, unsigned int _team);

	void swapVehicleController(Vehicle * _PlayerController, Vehicle * _IAController);
	bool getNextPlane(Vehicle * _vehicle);
	bool checkPlaneLife(unsigned int _index);
	bool checkIsPlane(unsigned int _index);
	bool checkIsPlayer(unsigned int);
	void debug();

	Vehicle * getPlayer();

	void match();

	void update(double seconds_elapsed);

	void deleteController(unsigned int i);
	int checkDeaths();

	

};

#endif