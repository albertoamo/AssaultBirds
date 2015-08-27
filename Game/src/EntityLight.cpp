#include "EntityLight.h"

EntityLight::EntityLight(float d, float s, Vector3 pos){
	this->diffuse_factor = d;
	this->specular_factor = s;
	this->position = pos;
}

void EntityLight::setColor(Vector3 color){
	this->color = color;
}

void EntityLight::move(Vector3 Translator){
	this->position = this->position + Translator;
}