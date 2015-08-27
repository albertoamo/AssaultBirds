#ifndef IACONTROLLER_H
#define IACONTROLLER_H

#include "Controller.h"

class IAController : public Controller
{
public:
	Vector3 objectivePoint;
	Vector3 to_target;
	Vector3 front;

	/*
	Estados:
	0: Muere<-Si entra en este ya no sale
	1: Persigue Objetivo i Ataca (default)
	2: Ve a un punto aleatorio (huir)
	3: Patrullar
	*/

	unsigned int state;

	float maxTimeState;

	float toTargetAngle;
	float UpDownAngle;
	float RightLeftAngle;

	IAController();
	virtual void init(Vehicle * _toControl);
	virtual void update(double elapsed_time);

	void setEnemy(Vehicle * Enemy);
	void setObjectivePoint(Vector3 _op);
	virtual void setRotation(float elapsed_time);
	virtual void recibeDeath();

	//Funciones de estados
	virtual void setFiring();
	virtual void setStates();



};

#endif