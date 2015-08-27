#ifndef AIRPLANEFACTORY_H
#define AIRPLANEFACTORY_H

#include "init/loader.h"
#include "Vehicle.h"
#include "Weapon.h"

class AirplaneFactory
{
public:
	static Vehicle * newF18Plane(Vector3 _position, Vector3 _rotation, float _quantRotation, const char* _name)
	{
		Vehicle * plane = loadModelEP(_position, "ax_f18.axmodel");
		plane->loadLowMesh("spitfire_low_rotated2.hym");
		plane->rotateLocal(_quantRotation, Vector3(0, 1, 0));
		plane->Name =_name;
		plane->calculateBB();

		plane->subType = PLANE;

		plane->totalRadius -= 2;//Ajustamos la bounding

		plane->setCabinPosition(Vector3(0, 0.82, 3.15));
		plane->setOutPosition(Vector3(0, 3, -14));

		//MovementVelocities
		plane->setMaxSpeed(400.0f);
		plane->setMinSpeed(125.0f);
		plane->setPitchVel(0.8f);
		plane->setRollVel(0.8f);
		plane->setYawVel(0.4f);
		plane->setThrustSpeed(200.0f);
		plane->setBrakeSpeed(50.0f);
		plane->setLife(100.0f);


		//Canon
		//Estos news se borrarán al llamar al destructor de Entity (donde se borran todos los hijos de los hijos de una entidad). 
		Weapon * newWeapon = new Weapon();
		newWeapon->setCulldown(10, 10, 0.6, 5.0);
		newWeapon->addCanon(Vector3(3,0,0), 30.0f, 900.0f, 0.5f);
		newWeapon->addCanon(Vector3(-3,0,0), 30.0f, 900.0f, 0.5f);
		newWeapon->setWeaponID(1);
		newWeapon->selected = true;

		Weapon * newWeapon2 = new Weapon();
		newWeapon2->setCulldown(10, 10, 0.15, 5.0);
		newWeapon2->addCanon(Vector3(0.75f,0,2), 5.0f, 700.0f, 0.2f);
		newWeapon2->addCanon(Vector3(-0.75,0,2), 5.0f, 700.0f, 0.2f);
		newWeapon2->setWeaponID(2);
		newWeapon2->selected = false;

		Weapon * newWeapon3 = new Weapon();
		newWeapon3->setCulldown(10, 10, 0.15, 5.0);
		newWeapon3->addCanon(Vector3(0,0,2), 50.0f, 300.0f, 1.0f);
		newWeapon3->setWeaponID(3);
		newWeapon3->selected = false;
	

		Entity * paco = plane->getChildren("ax_f18_puerta13");
		std::cout << paco->Name << std::endl;
		//((EntityMesh*)paco)->material->cullTest = Material::front;
		((EntityMesh*)paco)->material->cullTest = Material::back;
		plane->addWeapon(newWeapon);
		plane->addWeapon(newWeapon2);
		plane->addWeapon(newWeapon3);
		return plane;
	}

	static Vehicle * newN12Plane(Vector3 _position, Vector3 _rotation, float _quantRotation, const char* _name)
	{
		Vehicle * plane = loadModelEP(_position, "ax_an12.axmodel");
		plane->loadLowMesh("ax_an12.axmodel");
		plane->rotateLocal(_quantRotation, Vector3(0, 1, 0));
		plane->Name =_name;
		plane->calculateBB();
		plane->buildCollisionTree();

		plane->setCabinPosition(Vector3(0, 0.82, 3.15));
		plane->setOutPosition(Vector3(0, 7, -23));

		plane->subType = PLANE;

		//MovementVelocities
		plane->setMaxSpeed(100.0f);
		plane->setMinSpeed(25.0f);
		plane->setPitchVel(0.8f);
		plane->setRollVel(0.8f);
		plane->setYawVel(0.4f);
		plane->setThrustSpeed(200.0f);
		plane->setBrakeSpeed(50.0f);
		plane->setLife(250.0f);

		EntityMesh::setRotation((EntityMesh*)plane->getChildren("ax_an12_mot1"), Vector3(0, 0, 1), 100);
		EntityMesh::setRotation((EntityMesh*)plane->getChildren("ax_an12_mot2"), Vector3(0, 0, 1), 100);
		EntityMesh::setRotation((EntityMesh*)plane->getChildren("ax_an12_mot3"), Vector3(0, 0, 1), 100);
		EntityMesh::setRotation((EntityMesh*)plane->getChildren("ax_an12_mot4"), Vector3(0, 0, 1), 100);

		return plane;
	}

};

#endif