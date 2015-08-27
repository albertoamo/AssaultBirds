#include "BulletManager.h"

BulletManager * BulletManager::instance = new BulletManager();

BulletManager::BulletManager()
{

	instance = this;

	bullets_vector = std::vector<Bullet>();
	bullets_vector.resize(600);
	for (unsigned int i = 0; i < bullets_vector.size(); i++)
	{
		bullets_vector[i].ttl = -1;
	}
}

void BulletManager::update(float elapsed_time)
{
	for(unsigned int x = 0; x < bullets_vector.size(); x ++){

		Bullet &v = bullets_vector[x];
		if(v.ttl <= 0)
			continue;

		v.lastPosition = v.position;
		v.position = v.position + v.velocity * elapsed_time + Vector3(0,-5,0) * elapsed_time;
		v.velocity = v.velocity - v.velocity * -0.01 * elapsed_time;
		v.ttl -= elapsed_time;
	}
}

bool BulletManager::createBullet(Vector3 ps, Vector3 vel, float pow, int author)
{
	for(unsigned int x = 0; x < bullets_vector.size(); x ++)
	{

		Bullet &v = bullets_vector[x];
		if(v.ttl > 0)
			continue;
		
		//std::cout << "Instantiating Bullets: " << ps.x << "  " << ps.y << "  " << ps.z << "  " << " Con vel: " << vel.x << "  " << vel.y << "  " << vel.z << std::endl; 

		//El pequeño añadido es para no renderizar dos vertices en el mismo punto, da error ¿?
		v = Bullet(ps, vel, 10.0f, pow, author);
		return true;
	}

return false;
}
