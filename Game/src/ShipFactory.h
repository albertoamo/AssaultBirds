#ifndef SHIPFACTORY_H
#define SHIPFACTORY_H

#include "init/loader.h"
#include "Vehicle.h"
#include "Weapon.h"

class ShipFactory
{
	public:
	static Vehicle * newFragataShip(Vector3 _position, Vector3 _rotation, float _quantRotation, const char* _name)
	{
		Vehicle * ship = loadModelEP(_position, "ax_fragata.axmodel");
		ship->loadLowMesh("ax_fragata.axmodel");
		ship->rotateLocal(_quantRotation, Vector3(0, 1, 0));
		ship->Name =_name;
		ship->calculateBB();
		ship->buildCollisionTree();

		ship->subType = SHIP;
		ship->options = 0x02;

		//MovementVelocities
		ship->setMaxSpeed(100.0f);
		ship->setMinSpeed(50.0f);
		ship->setPitchVel(0.0f);
		ship->setRollVel(0.0f);
		ship->setYawVel(0.4f);
		ship->setThrustSpeed(200.0f);
		ship->setBrakeSpeed(50.0f);
		ship->setLife(100.0f);

		return ship;
	}

	static Vehicle * newWaspShip(Vector3 _position, Vector3 _rotation, float _quantRotation, const char* _name)
	{
		Vehicle * ship = loadModelEP(_position, "ax_wasp.axmodel");
		ship->loadLowMesh("ax_wasp.axmodel");
		ship->rotateLocal(_quantRotation, Vector3(0, 1, 0));
		ship->Name =_name;
		//ship->calculateBB();
		ship->totalRadius = 100.0;
		ship->buildCollisionTree();

		ship->options = 0x02;

		ship->subType = SHIP;

		//MovementVelocities
		ship->setMaxSpeed(100.0f);
		ship->setMinSpeed(50.0f);
		ship->setPitchVel(0.0f);
		ship->setRollVel(0.0f);
		ship->setYawVel(0.4f);
		ship->setThrustSpeed(200.0f);
		ship->setBrakeSpeed(50.0f);
		ship->setLife(100.0f);

		return ship;
	}

	static Vehicle * newTankerShip(Vector3 _position, Vector3 _rotation, float _quantRotation, const char* _name)
	{
		Vehicle * ship = loadModelEP(_position, "ax_tanker.axmodel");
		ship->loadLowMesh("ax_tanker.axmodel");
		ship->rotateLocal(_quantRotation, Vector3(0, 1, 0));
		ship->Name =_name;
		//ship->calculateBB();
		ship->totalRadius = 100.0;
		ship->buildCollisionTree();

		ship->options = 0x02;

		ship->subType = SHIP;

		//MovementVelocities
		ship->setMaxSpeed(100.0f);
		ship->setMinSpeed(50.0f);
		ship->setPitchVel(0.0f);
		ship->setRollVel(0.0f);
		ship->setYawVel(0.4f);
		ship->setThrustSpeed(200.0f);
		ship->setBrakeSpeed(50.0f);
		ship->setLife(100.0f);

		return ship;
	}

	static Vehicle * newCarrierShip(Vector3 _position, Vector3 _rotation, float _quantRotation, const char* _name)
	{
		Vehicle * ship = loadModelEP(_position, "ax_carrier.axmodel");
		ship->loadLowMesh("ax_carrier.axmodel");
		ship->rotateLocal(_quantRotation, Vector3(0, 1, 0));
		ship->Name =_name;
		//ship->calculateBB();
		ship->totalRadius = 200.0;
		ship->buildCollisionTree();

		ship->options = 0x02;

		ship->subType = SHIP;

		//MovementVelocities
		ship->setMaxSpeed(0.0f);
		ship->setMinSpeed(0.0f);
		ship->setPitchVel(0.0f);
		ship->setRollVel(0.0f);
		ship->setYawVel(0.4f);
		ship->setThrustSpeed(200.0f);
		ship->setBrakeSpeed(50.0f);
		ship->setLife(100.0f);

		return ship;
	}

};

#endif