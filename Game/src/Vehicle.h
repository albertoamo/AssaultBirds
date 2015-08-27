#ifndef Vehicle_H
#define Vehicle_H

#include "utils.h"
#include "framework.h"
#include "EntityMesh.h"

class Weapon;
class Controller;

enum SUBTYPE{
	PLANE = 0,
	TURRET = 1,
	SHIP = 2,
	ROCKET = 3,
	BOMB = 4
};

class Vehicle : public EntityMesh
{
public:

	//Controller y comunicación
	Controller * control;
	void setController(Controller * _control);
	void comunicateDeath();

	int subType;

	//atributos unicos de cada avion
	bool living;
	float life;
	float pitchVel, rollVel, yawVel;
	float thrustSpeed, brakeSpeed;
	float maxSpeed;
	float minSpeed;
	float speed;
	int RocketsCounter;
	int BombsCounter;
	bool alarmActivated;

	//Bools de control
	bool isAccelerating;
	bool isFiring;

	//Posiciones donde el controller tendrá que colocar la camara
	Vector3 cabinPosition;
	Vector3 outPosition;
	Vector3 frontPosition;

	Vehicle();
	Vehicle(const char* name, Material * material, Vector3 origin);
	virtual ~Vehicle();

	//Setters de atributos de movimiento
	void setPitchVel(float _pv);
	void setRollVel(float _rv);
	void setYawVel(float _yv);
	void setMaxSpeed(float _maxSpeed);
	void setMinSpeed(float _minSpeed);
	void setThrustSpeed(float _ts);
	void setBrakeSpeed(float _bs);
	void setLife(float _l);

	//Movimiento
	void dying(float seconds_elapsed);
	void accelerate();
	void decelerate();
	void doPitch(float value);
	void doRoll(float value);
	void doYaw(float value);
	void doPitchRight(float elapsed);
	void doRollRight(float elapsed);
	void doYawRight(float elapsed);
	void doPitchLeft(float elapsed);
	void doRollLeft(float elapsed);
	void doYawLeft(float elapsed);

	//Setters de posiciones de la camara
	void setCabinPosition(Vector3 _position);
	void setOutPosition(Vector3 _position);

	//Por si esta conectado Joystick
	void accelerate(float _speed);
	void decelerate(float _speed);

	//Metodos actualizacion
	void update(float seconds_elapsed);
	void addWeapon(Weapon * _weapon);
	void selectWeapon(unsigned int i);
	virtual void bulletCollision(Bullet & bullet);
	virtual void onCollision();
	virtual void onCollision(EntityMesh * _entity);
	virtual void onCollision(Vehicle * _entity);

	float getActualCullDown();

};

#endif