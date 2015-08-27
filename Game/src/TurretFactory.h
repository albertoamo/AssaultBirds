#ifndef TURRETFACTORY_H
#define TURRETFACTORY_H

#include "init/loader.h"
#include "Vehicle.h"
#include "Weapon.h"

class TurretFactory
{
public:
	static Vehicle * newBasicTurretSmall(Vector3 _position, Vector3 _rotation, float _quantRotation, const char* _name)
	{
		Vehicle * turret = loadModelEP(_position, "ax_turret_small.axmodel");
		turret->loadLowMesh("ax_turret_small.axmodel");
		turret->rotateLocal(_quantRotation, Vector3(0, 1, 0));
		turret->Name =_name;
		turret->calculateBB();

		turret->setCabinPosition(Vector3(0, 0.82, 3.15));
		turret->setOutPosition(Vector3(0, 7, -23));

		turret->subType = TURRET;

		//MovementVelocities
		turret->setMaxSpeed(0.0f);
		turret->setMinSpeed(0.0f);
		turret->setPitchVel(2.0f);
		turret->setRollVel(0.0f);
		turret->setYawVel(2.0f);
		turret->setThrustSpeed(200.0f);
		turret->setBrakeSpeed(50.0f);
		turret->setLife(20.0f);


		//Canon
		//Estos news se borrarán al llamar al destructor de Entity (donde se borran todos los hijos de los hijos de una entidad). 
		Weapon * newWeapon = new Weapon();
		newWeapon->addCanon(Vector3(0.5,0,0), 30.0f, 900.0f, 0.2f);
		newWeapon->addCanon(Vector3(-0.5,0,0), 30.0f, 900.0f, 0.2f);
		newWeapon->setWeaponID(1);
		newWeapon->selected = true;
		newWeapon->Name = "canon";
		turret->addWeapon(newWeapon);

		return turret;
	}

	static Vehicle * newBasicTurret(Vector3 _position, Vector3 _rotation, float _quantRotation, const char* _name)
	{
		Vehicle * turret = loadModelEP(_position, "ax_turret.axmodel");
		turret->loadLowMesh("ax_turret.axmodel");
		turret->rotateLocal(_quantRotation, Vector3(0, 1, 0));
		turret->Name =_name;
		turret->calculateBB();
		turret->totalRadius += 7.0;
		turret->setCabinPosition(Vector3(0, 0.82, 3.15));
		turret->setOutPosition(Vector3(0, 7, -23));

		//MovementVelocities
		turret->setMaxSpeed(0.0f);
		turret->setMinSpeed(0.0f);
		turret->setPitchVel(2.0f);
		turret->setRollVel(0.0f);
		turret->setYawVel(2.0f);
		turret->setThrustSpeed(200.0f);
		turret->setBrakeSpeed(50.0f);
		turret->setLife(20.0f);

		turret->subType = TURRET;

		//Canon
		//Estos news se borrarán al llamar al destructor de Entity (donde se borran todos los hijos de los hijos de una entidad). 
		Weapon * newWeapon = new Weapon();
		newWeapon->addCanon(Vector3(0.5,0,0.3), 30.0f, 900.0f, 0.2f);
		newWeapon->addCanon(Vector3(-0.5,0,0.3), 30.0f, 900.0f, 0.2f);
		newWeapon->setWeaponID(1);
		newWeapon->selected = true;
		newWeapon->Name = "canon";
		turret->addWeapon(newWeapon);

		return turret;
	}
};

#endif