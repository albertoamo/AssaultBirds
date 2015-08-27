#include "TurretController.h"
#include "Weapon.h"

void TurretController::init(Vehicle * _toControl)
{
	toControl = _toControl;

	toControl->setController(this);

	setObjectivePoint(_toControl->origin + Vector3(0,0,3000));

	newEnemy = 10.0;
}

void TurretController::update(double seconds_elapsed)
{
	if( enemy != NULL){
		Vector3 firingPoint = enemy->origin + enemy->Model.rotateVector(Vector3(0,0,1))*30;
		setObjectivePoint(firingPoint);
	}

	newEnemy -= seconds_elapsed;
	if(newEnemy < 0.0)
	{
		newEnemy = 10.0;
		enemy = NULL;
	}

	setRotation(seconds_elapsed);
	setFiring();

	if(toControl->life < 0.0)
	{
		toControl->toDelete = true;
		toControl->comunicateDeath();
	}
}

void TurretController::setFiring()
{
	float distance;
	if(enemy != NULL){
		if(enemy->control != NULL){
			if(enemy->control->type == 0)
			{
				distance = 6000;
			}
			else{
				distance = 3200;
			}
			if(to_target.length() < distance)
			{
				toControl->isFiring = true;
			}
		}
	}
}

/*void TurretController::setRotation(double speed)
{

	Entity*rot = toControl->getChildren("ax_turret_cannon");
	EntityMesh*rot2 = (EntityMesh*)rot;

	to_target = (objectivePoint - toControl->getGlobalMatrix().getTranslationOnly());
	Vector3 aleatori;
	aleatori.random(0.7);
	Vector3 to_target_N = (to_target+aleatori).getNormalized();
	front = (toControl->Model.rotateVector(Vector3(0,0,1))).normalize();

	float targetDotFront = to_target_N.dot(front);

	UpDownAngle = to_target_N.dot(rot->Model.rotateVector(Vector3(0,1,0)));
	RightLeftAngle = to_target_N.dot(toControl->Model.rotateVector(Vector3(1,0,0)));

	Vector3 front = (toControl->Model.rotateVector(Vector3(0,0,1))).normalize();
	float FrontUpAngle = front.dot(Vector3(0,1,0));

	rot2->Model.setRotation(UpDownAngle, Vector3(1,0,0));
	toControl->getChildren("canon")->Model.setRotation(UpDownAngle, Vector3(1,0,0));

	if(RightLeftAngle < -0.01){toControl->doYawRight(speed);}
	if(RightLeftAngle >  0.01){toControl->doYawLeft(speed);}

	//float rolled = (toControl->Model.rotateVector(Vector3(0,1,0))).dot(Vector3(0,1,0));
	//std::cout << "Rolled: " << rolled << std::endl;

	//float rolled = (toControl->Model.rotateVector(Vector3(1,0,0))).dot(Vector3(0,1,0));

}*/