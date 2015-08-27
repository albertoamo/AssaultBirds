#include "Vehicle.h"
#include "Weapon.h"

#include "Controller.h"

//Constructores
Vehicle::Vehicle():speed(0.0f), maxSpeed(300.0f), isFiring(false), isAccelerating(false)
{
	this->Type = "Vehicle";
}

Vehicle::Vehicle(const char* name, Material* material, Vector3 origin):
EntityMesh(name, material, origin)
{
	this->isAccelerating = false;
	this->isFiring = false;

	this->Type = "Vehicle";

	//Inicializadores base de atributos
	this->speed = 0.0f;
	this->life = 100.0f;
	this->maxSpeed = 0.0f;
	this->minSpeed = 0.0f;
	this->thrustSpeed = 0.0f;
	this->brakeSpeed = 0.0f;
	this->pitchVel = 0.0f;
	this->rollVel = 0.0f;
	this->yawVel = 0.0f;
	this->living = true;

	cabinPosition = Vector3(0,3,-10);
	outPosition = Vector3(0,4,-15);
	frontPosition = Vector3(0,3,10);

	RocketsCounter = 1;
	BombsCounter = 1;

	this->control = NULL;

	alarmActivated = false;
}

Vehicle::~Vehicle()
{
	delete(collision_model);
	deleteChildren();
}

//Setters
void Vehicle::setMaxSpeed(float _maxSpeed){	maxSpeed = _maxSpeed;}
void Vehicle::setMinSpeed(float _minSpeed){	minSpeed = _minSpeed;}
void Vehicle::setPitchVel(float _pv){this->pitchVel = _pv;}
void Vehicle::setRollVel(float _rv){this->rollVel = _rv;}
void Vehicle::setYawVel(float _yv){this->yawVel = _yv;}
void Vehicle::setThrustSpeed(float _ts){this->thrustSpeed = _ts;}
void Vehicle::setBrakeSpeed(float _bs){this->brakeSpeed = _bs;}
void Vehicle::setLife(float _l){this->life = _l;}
void Vehicle::setCabinPosition(Vector3 _position){this->cabinPosition  = _position;}
void Vehicle::setOutPosition(Vector3 _position){this->outPosition = _position;}

//Movimientos

//Acelera o desacelera
void Vehicle::accelerate(){speed += thrustSpeed;}
void Vehicle::decelerate(){speed -= brakeSpeed;}
void Vehicle::accelerate(float _speed){speed += thrustSpeed*_speed;}
void Vehicle::decelerate(float _speed){speed -= brakeSpeed*_speed;}

//Gira Teclat
void Vehicle::doPitchRight(float elapsed){rotateLocal(elapsed*pitchVel, Vector3(1,0,0));}
void Vehicle::doRollRight(float elapsed){rotateLocal(elapsed*rollVel, Vector3(0,0,-1));}
void Vehicle::doYawRight(float elapsed){rotateLocal(elapsed*yawVel, Vector3(0,1,0));}
void Vehicle::doPitchLeft(float elapsed){rotateLocal(elapsed*pitchVel, Vector3(-1,0,0));}
void Vehicle::doRollLeft(float elapsed){rotateLocal(elapsed*rollVel, Vector3(0,0,1));}
void Vehicle::doYawLeft(float elapsed){rotateLocal(elapsed*yawVel, Vector3(0,-1,0));}

//Gira Joystick
void Vehicle::doPitch(float elapsed){rotateLocal(elapsed*pitchVel, Vector3(1,0,0));}
void Vehicle::doRoll(float elapsed){rotateLocal(elapsed*rollVel, Vector3(0,0,-1));}
void Vehicle::doYaw(float value){rotateLocal(value*yawVel, Vector3(0,1,0));}

//Referente a armas
void Vehicle::addWeapon(Weapon * weapon){addChildren(weapon);}
void Vehicle::selectWeapon(unsigned int i){processEvent("selectWeapon"+std::to_string(i));}

//Actualizacion de estados
void Vehicle::update(float seconds_elapsed)
{
	actualizeCollisionModel();

	if(life < 0.0)
		living = false;

	decelerate(seconds_elapsed);

	if(subType == ROCKET){
		life -= seconds_elapsed*10;
		if(!living)
			toDelete = true;
	}

	if(subType == BOMB){
		translateLocal(Vector3(0,-seconds_elapsed*19.6, 0));
		if(!living)
			toDelete = true;
		return;
	}

	//------------------------------------Movimiento--------------------------------
	//Clampeamos la aceleracion entre min y max (los nombres estan al reves en la funcion de clmap no se xk)
	speed = clamp(speed, minSpeed, maxSpeed);
	//std::cout << "Speed1: " << speed << std::endl;

	//Actualizamos la posicion del avion respecto la velocidad que lleve
	translateLocal(Vector3(0,0,1)*speed*seconds_elapsed);

	//No queremos que el vehiculo se vaya demasiado lejos
	float length = (Model.getTranslationOnly() - Vector3(0,0,0)).length();
	if(control != NULL)
	{
		if(control->type == 0){
			if(length > 10000){
				if( !alarmActivated ){
				alarmActivated = true;
				SoundManager::Instance()->playSound(ALARM);
				}
			}
			else{
				alarmActivated = false;
				SoundManager::Instance()->stopSound(ALARM);
			}
		}
	}
	if(length > 9000)
	{
		//this->life -= 1000;
	}

	//------------------------------Actualizaciones hijos---------------------------------
	//Disparamos despues de mover!
	if(isFiring)
		processEvent("fire"+std::to_string(speed));
	isFiring = false;

	for(auto it = children.begin(); it != children.end(); it++)
		(*it)->update(seconds_elapsed);

}


void Vehicle::bulletCollision(Bullet & _bullet)
{
	//std::cout << "Un avion " << id << " ha recibido un impacto de la bala lanzado por " << _bullet.author << std::endl;
	_bullet.ttl = -1.0f;
	this->life -= _bullet.power;
}

void Vehicle::onCollision()
{
	//std::cout << "He chocado algo . Puesto en Vehicle." << std::endl;
	this->life -= 50;
}

void Vehicle::onCollision(EntityMesh * _entity)
{

	if(_entity->Type == this->Type)
	{
		Vehicle * plane = (Vehicle*)_entity;
		if(plane->Name != this->Name)
			onCollision(plane);
	}
	else
	{
		if(subType == ROCKET || subType == BOMB)
		{
			life -= 1000;
			SoundManager::Instance()->playSound(EXPL2);
		}
		if(control != NULL)
			if(control->type == 1 || control->type == 0 || control->type == 2){
				this->life -= 1000;
				toDelete = true;
				comunicateDeath();
				SoundManager::Instance()->playSound(EXPL3);
			}
	}
}

void Vehicle::onCollision(Vehicle * _entity)
{
	this->life -= 1000.0f;
	SoundManager::Instance()->playSound(EXPL1);
	if(subType == ROCKET || subType == BOMB)
		toDelete = true;
}


void Vehicle::setController(Controller * _control)
{
	this->control = _control;
}

void Vehicle::comunicateDeath()
{
	if(control != NULL)
		control->recibeDeath();
	control = NULL;
}

float Vehicle::getActualCullDown()
{
	for(unsigned int i = 0; i < children.size(); i++)
	{
		if(children[i]->Type == "weapon")
		{
			Weapon * weapon = (Weapon*)children[i];
			if(weapon->selected)
			{
				float cullDownFact = 0.0;
				cullDownFact = weapon->cullDown/weapon->maxCullDown;
				return cullDownFact;
			}
		}
	}
}