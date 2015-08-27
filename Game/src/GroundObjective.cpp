#include "GroundObjective.h"
#include "BulletManager.h"

GroundObjective::GroundObjective()
{
	this->Type = "GroundObjective";
}

GroundObjective::GroundObjective(const char* name, Material* material, Vector3 origin)
	:EntityMesh(name, material, origin)
{
	this->Type = "GroundObjective";
	this->life = 100.0f;
	this->dieTime = 10.0;
	this->fallVel = 10.0;
}

void GroundObjective::update(float elapsed_time)
{
	actualizeCollisionModel();

	if(life < 100.0f)
	{
		translateLocal(Vector3(0,-1,0)*elapsed_time*fallVel);
		dieTime -= elapsed_time;
		if(dieTime < 0.0)
			toDelete = true;
	}
}

void GroundObjective::onCollision()
{
	this->life -= 1000;
}

void GroundObjective::bulletCollision(Bullet & _bullet)
{
	//std::cout << "Un avion " << id << " ha recibido un impacto de la bala lanzado por " << _bullet.author << std::endl;
	//std::cout << "Bala lanzada por " << _bullet.author << " choca con: " << id << std::endl;
	_bullet.ttl = -1.0f;
	this->life -= _bullet.power;
}