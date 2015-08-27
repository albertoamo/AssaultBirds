#ifndef ENTITYLIGHT_H
#define ENTITYLIGHT_H

#include "Entity.h"
#include "framework.h"

#define MAX_POINT_LIGHTS 6
class EntityLight : public Entity{
public: 
	//metodos
	Vector3 position; //this should be replaced...
	Vector3 color;

	float diffuse_factor;
	float specular_factor;

	EntityLight(){}
	EntityLight(float d, float s, Vector3 pos);
	void move(Vector3 Translator);
	void setColor(Vector3 color);

};

//Different type of lights
class PointLight: public EntityLight{
public:
	//Vector3 position;

	struct
	{
		float Constant;
		float Linear;
		float Exp;
	} Atten;

	PointLight(){}
	PointLight(Vector3 pos, Vector3 atten);
};

//Different type of lights
class SpotLight : public EntityLight{
public:
	//Vector3 position;
	Vector3 direction;
	float cutoff;

	SpotLight(){}
	SpotLight(Vector3 pos, Vector3 direction, float cutoff);
};

//Opengl pointlight struct to be passed to shaders

struct pointLights{

	GLuint color;
	GLuint diffuse_factor;
	GLuint specular_factor;
	GLuint position;

	struct
	{
		GLuint Constant;
		GLuint Linear;
		GLuint Exp;
	} Atten;

}; //this should be an array of maximum 6 lights..

struct SpotLights{

	GLuint color;
	GLuint diffuse_factor;
	GLuint specular_factor;
	GLuint position;
	GLuint direction;
	GLuint cutoff;

};

#endif