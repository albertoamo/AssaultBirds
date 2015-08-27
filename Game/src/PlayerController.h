#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include "Controller.h"
class PlayerController : public Controller
{
public:
	//Variables para la 'escena' inicial
	static bool started;
	float aux_min;
	float aux_radius;
	float aux_time;
	bool aux_camChanged;

	Camera * camera;
	
	int cameraSelector; //1=Cabin, 2=Out
	Vector3 cameraTraslator;

	Vector3 centerTraslator[5];
	int centerSelector;
	float centerDegrees;
	float center;
	bool takeoff;
	float startTime;

	Vector3 diePoint;

	PlayerController(){type = 0; cameraSelector = 2; canPressX = true; canPressA = false; canPressB = true;}

	void setCameraToEntity();
	void changeCameraPoint();
	void returnCameraToPosition(double elapsed_time);
	void returnCenterDegrees(double elapsed_time);

	void init(Vehicle * _toControl);
	void recibeDeath();
	void die(double elapsedTime);

	void update(double elapsed_time);

	//bools de control
	bool canPressX; //X se usa para seleccionar cambiar de arma
	bool canPressA;
	bool canPressB;

	//Extra Rocket
	void shootRocket();
	float rocketTime;

	//Extra Bombs
	void shootBomb();
	float bombTime;

};

#endif