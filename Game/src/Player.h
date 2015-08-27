#ifndef PLAYER_H
#define PLAYER_H
#include "includes.h"

#include <vector>
#include "framework.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"
#include "Entity.h"

class Player{

public:

	//Name
	char* name;
	float health;
	float speed;
	Matrix44 Model;
	Vector3 origin;

	Camera * camera;

	//EntityMesh body;
	//EntityMesh viewhands
	//EntityMesh ...

	//Weapons settings and stuff
	//Weapon gunfire
	//Weapon torpedo

	//Attribute settings
	bool godMode;
	bool status;

	Player();
	Player(char * name, Vector3 position);

	~Player();

	void initCamera(int window_width, int window_height);
	void update();
	void respawn(Vector3 position);

	void move(Vector3 delta);
	void rotate(float angle, const Vector3& axis);

	void translateLocal(Vector3 Traslator){
		this->Model.traslateLocal(Traslator.x, Traslator.y, Traslator.z);
		this->origin = Model.getTranslationOnly();
	}

	void rotateLocal(float angle_in_rads, Vector3 axis){
		this->Model.rotateLocal(angle_in_rads, axis);
	}

	//Camera settings from player accesed from here
	void setFov(float fov);
	float getFov();

};

#endif