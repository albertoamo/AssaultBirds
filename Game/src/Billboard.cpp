#include "Billboard.h"
#include "world.h"
#include <algorithm>

#define dispersion 45

bool cmp(const Plane * a, const Plane * b) {
	return a->distance > b->distance;
}

Billboard::Billboard(){
	Model.setIdentity();
	
}

Billboard::~Billboard(){

	for (Plane * plane : planes)
		delete(plane);

}
void Billboard::update(){
	
	int size = this->mesh->vertices.size();
	delete(mesh);

	this->mesh = new Mesh();
	Vector3 top = Vector3(0,1,0);//Game::instance->camera->getLocalVector(Vector3(0, 1, 0));
	Vector3 right = Game::instance->camera->getLocalVector(Vector3(1, 0, 0));

	for (unsigned int i = 0; i < planes.size(); i++){
		planes[i]->distance = planes[i]->pos.distance(Game::instance->camera->eye);
	}
	std::sort(planes.begin(), planes.end(), cmp);

	for (unsigned int i = 0; i < planes.size(); i++){

		Plane * plane = planes[i];

		Vector3 lookVector = Game::instance->camera->eye - plane->pos;
		lookVector.normalize();

		right = top.cross(lookVector);

		this->mesh->vertices.push_back(plane->pos - (right + top) * plane->size);
		this->mesh->vertices.push_back(plane->pos + (right - top) * plane->size);
		this->mesh->vertices.push_back(plane->pos + (right + top) * plane->size);
		this->mesh->vertices.push_back(plane->pos - (right - top) * plane->size);

		this->mesh->uvs.push_back(Vector2(plane->end.x, plane->start.y));
		this->mesh->uvs.push_back(Vector2(plane->start.x, plane->start.y));
		this->mesh->uvs.push_back(Vector2(plane->start.x, plane->end.y));
		this->mesh->uvs.push_back(Vector2(plane->end.x, plane->end.y));

	}
	

}

void Billboard::render(Renderer * r){

	r->render(*this); 
}

void Billboard::addPlane(Vector3 & position, float size){

	Plane * plane = new Plane();
	plane->pos = position;
	plane->size = size;
	planes.push_back(plane);

}

void Billboard::addClouds(Vector3 & position, float size, float num){

	for (unsigned int x = 0; x < num; x++){
		float newSize = random(size-150, size + 150);
		float dist = dispersion + (size * num) / (dispersion * 2);
		float outputx = -dist * 3 + (rand() % (int)(dist * 3 - (-dist * 3) + 1));
		float outputy = -dist + (rand() % (int)(dist - (-dist) + 1));
		float outputz = -dist * 3 + (rand() % (int)(dist * 3 - (-dist * 3) + 1));
		addPlane(Vector3(position.x + outputx, position.y + outputy, position.z + outputz), newSize);
	}

}

void Billboard::setMaterial(const char* filename){


	material = new Material("Billboard", "Default", 20);
	material->loadShader("billboard.vs", "billboard.ps");
	material->loadTexture(filename, filename);

}

Sprite::Sprite(){

	this->mesh = new Mesh();

}

Sprite::~Sprite(){

}
void Sprite::setMaterial(const char* filename, int rangex, int rangey){


	material = new Material("Billboard", "Default", 20);
	material->loadShader("billboard.vs", "billboard.ps");
	material->loadTexture(filename, filename);

	this->xsize = this->material->colormap->width / (rangex * this->material->colormap->width);
	this->ysize = this->material->colormap->height / (rangey * this->material->colormap->height);

	this->range.x = rangex;
	this->range.y = rangey;

}

void Sprite::addPlane(Vector3 & position, float size){

	Plane * plane = new Plane();
	plane->pos = position;
	plane->size = size;
	planes.push_back(plane);

	int randx = random(0, range.x - 1);
	int randy = random(0, range.y - 1);

	plane->generateSprite(randx, randy, xsize, ysize);

}

void Sprite::addPlane(Vector3 & position, float size, Vector2 coord){

	Plane * plane = new Plane();
	plane->pos = position;
	plane->size = size;
	planes.push_back(plane);
	plane->generateSprite(coord.x, coord.y, xsize, ysize);


}

void Sprite::addStorm(Vector3 start, Vector3 end, int num){

	for (unsigned int j = 0; j < num; j++){
		float x = random(start.x, end.x);
		float y = random(start.y, end.y);
		float z = random(start.z, end.z);
		float number = random(5, 8);
		float size = random(1800, 3800);
		addClouds(Vector3(x, y, z), size, number);
	}

}

void Sprite::addClouds(Vector3 & position, float size, float num){

	for (unsigned int x = 0; x < num; x++){
		float newSize = random(size - 150, size + 150);
		float dist = dispersion + (size * num) / (dispersion * 2);
		float outputx = -dist * 3 + (rand() % (int)(dist * 3 - (-dist * 3) + 1));
		float outputy = -dist + (rand() % (int)(dist - (-dist) + 1));
		float outputz = -dist * 3 + (rand() % (int)(dist * 3 - (-dist * 3) + 1));
		addPlane(Vector3(position.x + outputx, position.y + outputy, position.z + outputz), newSize);
	}

}

