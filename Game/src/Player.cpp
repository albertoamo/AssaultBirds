#include "Player.h"

Player::Player(char * name, Vector3 position){
	this->name = name;
	this->origin = position;
}

Player::~Player()
{
	delete(name);
	delete(camera);
}

void Player::initCamera(int window_width, int window_height){
	this->camera = new Camera();
	this->camera->lookAt(origin + Vector3(0,25,25), Vector3(0, 0, 0), Vector3(0, 1, 0)); //position the camera and point to 0,0,0
	this->camera->setPerspective(70, window_width / (float)window_height, 1, 100000); //set the projection, we want to be perspective
}

void Player::update(){

	//Here player updates!
	this->camera->set();
}

void Player::respawn(Vector3 position){

	if(!status){//In case the player is dead, we respawn, otherwise not!
		this->move(position);
		std::cout << "Player [ " + std::string(this->name) + " ] has respawned!" << std::endl;
	}
	else{
		std::cout << "Player [ " + std::string(this->name) + " ] is alive, cannot respawn him!" << std::endl;
	}

}

void Player::move(Vector3 delta)
{
	Vector3 localDelta = this->camera->getLocalVector(delta);

	this->origin = this->origin - localDelta;
	this->translateLocal(delta);

	this->camera->eye = this->camera->eye - localDelta;
	this->camera->center = this->camera->center - localDelta;
	this->camera->updateViewMatrix();

}

void Player::rotate(float angle, const Vector3& axis)
{
	Matrix44 R;
	R.setRotation(angle, axis);
	Vector3 new_front = R * (this->camera->center - this->camera->eye);
	this->camera->center = this->camera->eye + new_front;
	this->camera->updateViewMatrix();

}