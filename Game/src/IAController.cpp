#include "IAController.h"
#include <time.h> 

IAController::IAController()
{
	toTargetAngle = 0;
	UpDownAngle = 0;
	RightLeftAngle = 0;
	type = 1;
	enemy = NULL;
}

void IAController::init(Vehicle * _toControl)
{
	maxTimeState = 0.0f;

	toControl = _toControl;

	toControl->setController(this);
}

void IAController::setObjectivePoint(Vector3 _op){this->objectivePoint = _op;}

void IAController::setEnemy(Vehicle * _vehicle){
	if(toControl != NULL){
		if(toControl->id != _vehicle->id){
			this->enemy = _vehicle;
		}
		else
			std::cout << "Error: Has definido como enemigo el mismo vehiculo al que controla." << std::endl;
	}
	else
	{
		std::cout << "Error: Controlador no inicializado." << std::endl;
	}
}



void IAController::setRotation(float speed)
{

	to_target = (objectivePoint - toControl->getGlobalMatrix().getTranslationOnly());
	Vector3 aleatori;
	aleatori.random(0.7);
	Vector3 to_target_N = (to_target+aleatori).getNormalized();
	front = (toControl->Model.rotateVector(Vector3(0,0,1))).normalize();

	float targetDotFront = to_target_N.dot(front);

	//toTargetAngle = acos(targetDotFront);

	UpDownAngle = to_target_N.dot(toControl->Model.rotateVector(Vector3(0,1,0)));
	RightLeftAngle = to_target_N.dot(toControl->Model.rotateVector(Vector3(1,0,0)));

	if(targetDotFront < 0.0  && UpDownAngle > -0.05 && UpDownAngle < 0.05 && RightLeftAngle > -0.05 && RightLeftAngle < 0.05)
		RightLeftAngle += 0.1;

	if(UpDownAngle < -0.01){toControl->doPitchLeft(speed);}
	if(UpDownAngle > 0.01){toControl->doPitchRight(speed);}

	if(RightLeftAngle < -0.01){toControl->doYawRight(speed);toControl->doRollRight(speed);}
	if(RightLeftAngle >  0.01){toControl->doYawLeft(speed);toControl->doRollLeft(speed);}

	float rolled = (toControl->Model.rotateVector(Vector3(0,1,0))).dot(Vector3(0,1,0));

	if(rolled < 0.9) toControl->doRollLeft(speed);
}

void IAController::recibeDeath(){} //Para no encontrar error de linkado
void IAController::update(double elapsed_time){} //Para no encontrar error de linkado
void IAController::setFiring(){} //Para no encontrar error de linkado



