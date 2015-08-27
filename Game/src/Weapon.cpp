#include "Weapon.h"
#include "Vehicle.h"
#include "Controller.h"

Canon::Canon(Vector3 _pos, float _pow, float _bulletVel,float _bulletTime)
{
	this->Name = "canonX";
	this->Type = "canon";
	this->timer = _bulletTime;
	this->Model.setTranslation(_pos.x, _pos.y, _pos.z);
	this->power = _pow;
	this->bulletVel = _bulletVel;
	this->bulletTime = _bulletTime;
	this->options = 0x05;
}

void Canon::fire(Vector3 direction, float velocity)
{
	unsigned int author = getRootId(parent);
	//SoundManager::Instance()->playSound(FIRE);
	BulletManager::instance->createBullet(this->getGlobalMatrix().getTranslationOnly(), direction*(bulletVel+velocity), power, author);
}

void Canon::processEvent(std::string event)
{
	std::size_t found = event.find("fire");
	if(found != std::string::npos && timer > bulletTime)
	{
		Weapon * weap = (Weapon*)parent;
		weap->cullDown -= weap->cullMinsFactor;
		std::string planeVel = event.substr(4, event.length());
		Vector3 direction = this->getGlobalMatrix().rotateVector(Vector3(0,0,1));
		fire(direction, stof(planeVel));
		timer = 0.0f;
		if(getGrandParent(parent)->Type == "Vehicle")
		{
			Entity * father = getGrandParent(parent);
			Vehicle * fath = (Vehicle*)father;
			if( fath->control != NULL)
				if(fath->control->type == 0)
				{
					if(weap->weaponID == 1)
						SoundManager::Instance()->playSound(FIRE1);
					if(weap->weaponID == 2)
						SoundManager::Instance()->playSound(FIRE2);
					if(weap->weaponID == 3)
						SoundManager::Instance()->playSound(FIRE3);
				}
		}

	}
}

void Canon::update(float elapsedTime)
{
	timer += elapsedTime;
}
