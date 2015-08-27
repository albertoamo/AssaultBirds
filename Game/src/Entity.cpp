#include "Entity.h"
#include "EntityMesh.h"

unsigned int Entity::idCounter = 0;

Entity::Entity():parent(NULL), toDelete(false){
	setId();
}

Entity::Entity(const char* name):parent(NULL), toDelete(false){
	this->Name = name;
	setId();
}


Entity::~Entity(){
	deleteChildren();
}

void Entity::deleteChildren(){
	for(auto it = children.begin(); it != children.end(); it++){
		if((*it)->children.size()){
			(*it)->deleteChildren();
		}
		else{
			delete(*it);
		}
	}

	children.clear();
}

void Entity::translateLocal(Vector3 Traslator){
	this->Model.traslateLocal(Traslator.x, Traslator.y, Traslator.z);
	this->origin = Model.getTranslationOnly();
}

void Entity::rotateLocal(float angle_in_rads, Vector3 axis){
	this->Model.rotateLocal(angle_in_rads, axis);
}

void Entity::scaleLocal(float val){
	Model.setScale(val, val, val);
}

void Entity::setName(const char* name){
	this->Name = name;
}

void Entity::setId(){
	id = idCounter;
	Entity::idCounter++;
}

void Entity::addChildren(Entity * entity){
	entity->parent = this;
	this->children.push_back(entity);
}

Matrix44 Entity::getGlobalMatrix(){

	if(parent)
		return Model * parent->getGlobalMatrix();

	return Model;
}

Vector3 Entity::getOrigin()
{
	return getGlobalMatrix().getTranslationOnly();
}

Entity * Entity::getChildren(const char* name){

	for (auto it = children.begin(); it != children.end(); ++it){
		if( strcmp((*it)->Name, name) == 0){
			return (*it);
		}
		if( !(*it)->children.empty()){
			return (*it)->getChildren(name);
		}
	}

	return NULL;
}

void Entity::rotateChildren(const char * name, float angle_in_rads, Vector3 axis){
	this->getChildren(name)->rotateLocal(angle_in_rads, axis);
}

void Entity::translateChildren(const char * name, Vector3 axis){
	this->getChildren(name)->translateLocal(axis);
}

void Entity::scaleChildren(const char * name, float size){
	this->getChildren(name)->scaleLocal(size);
}


void Entity::processEvent(std::string event)
{
	for(auto child = children.begin(); child != children.end(); child++)
			(*child)->processEvent(event);
}

void Entity::update(float elapsedTime)
{
	for(auto it = children.begin(); it != children.end(); it++)
		(*it)->update(elapsedTime);
}

unsigned int Entity::getRootId(Entity * ent)
{
    if(!ent->parent)
		return ent->id;
	return getRootId(ent->parent);

}

Entity * Entity::getGrandParent(Entity * ent)
{
	if(!ent->parent)
		return ent;
	return getGrandParent(ent->parent);
}

void Entity::printChildren(){

	for (auto it = children.begin(); it != children.end(); it++)
		(*it)->printChildren();

	std::cout << this->Name << std::endl;
}