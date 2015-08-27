#ifndef BULLETMANAGER_H
#define BULLETMANAGER_H

#include "framework.h"
#include "Renderer.h"

class Bullet
{
public:
	Vector3 position;
	Vector3 lastPosition;
	Vector3 velocity; //Contiene la dircción
	float ttl;
	float power;
	unsigned int author;
	//Añadir un tipo, int? char*? string?

	Bullet(){this->ttl = -1;}

	Bullet(Vector3 _position, Vector3 _velocity, float _ttl, float _power, unsigned int _author)
	{
		this->position = _position;
		this->lastPosition = _position;
		this->velocity = _velocity; //Contiene la dircción
		this->ttl = _ttl;
		this->power = _power;
		this->author = _author;
	}

};


class BulletManager
{
public:
	std::vector<Bullet> bullets_vector;

	BulletManager();

	void update(float elapsed_time);
	void render(Renderer * r){ r->render(*this); }

	bool createBullet(Vector3 ps, Vector3 vel, float pow, int author);

	static BulletManager * instance;
};

#endif