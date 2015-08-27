#ifndef CANON_H
#define CANON_H
#include "Entity.h"
#include "BulletManager.h"
#include "SoundManager.h"

class Canon : public Entity
{

	float power;
	float bulletVel;
	float timer;
	float bulletTime;

public:
	Canon(Vector3 _pos, float _pow, float _bulletVel, float _bulletTime);

	void fire(Vector3 direction, float velocity);

	void processEvent(std::string event);

	void update(float elapsedTime);
};

class Weapon : public Entity
{
public:
	bool selected;
	float power;
	unsigned int weaponID;
	
	//cullDowns
	float cullDown;
	float maxCullDown;
	float cullMinsFactor;
	float waitCullDown;
	float actualWait;
	bool canFire;

	Weapon()
	{
		this->Name = "weaponX";
		Type = "weapon"; 
		selected = false;
		this->options = 0x05;
		cullDown = 10.0;
		waitCullDown = 2.0;
		canFire = true;
		actualWait = 10;
	}

	~Weapon(){}

	void setWeaponID(unsigned int id){weaponID = id;}

	void addCanon(Vector3 _position, float _power, float _bulletVel, float _bulletTime){
		//Es una entity, se destruye con el destructor de entityes que recorrerá los hijos.
		Canon * newCanon = new Canon(_position, _power, _bulletVel, _bulletTime);
		addChildren((Entity*)newCanon);
	}

	void processEvent(std::string _event)
	{
		std::size_t found = _event.find("fire");
		if(selected && found != std::string::npos && canFire)
		{	
			for(auto child = children.begin(); child != children.end(); child++)
				(*child)->processEvent(_event);
			return;
		}
		if(_event.find("selectWeapon") != std::string::npos){
			if(atoi((_event.substr(_event.size()-1, 1)).c_str()) == weaponID)
				selected = true;
			else
				selected = false;
		}
	}

	void setCulldown(float _cullDown, float _maxCullDown, float _cullMinsFactor, float _waitCullDown)
	{
		cullDown = _cullDown;
		maxCullDown = _maxCullDown;
		cullMinsFactor = _cullMinsFactor;
		waitCullDown = _waitCullDown;
		actualWait = waitCullDown;
	}

	virtual void update(float elapsedTime)
	{
		for(unsigned int i = 0; i < children.size(); i++)
			children[i]->update(elapsedTime);

		if(canFire)
			cullDown += cullMinsFactor*elapsedTime;

		if(cullDown > 10)
			cullDown = 10.0;

		if(cullDown < 0.0)
			canFire = false;

		if(!canFire)
			actualWait -= elapsedTime;

		if(actualWait < 0.0)
		{
			actualWait = waitCullDown;
			cullDown = maxCullDown;
			canFire = true;
		}

	}

};



#endif