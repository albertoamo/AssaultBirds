#ifndef BILLBOARD_H
#define BILLBOARD_H

#include "includes.h"
#include "framework.h"
#include "Entity.h"
#include "EntityMesh.h"
#include "Renderer.h"
#include "rendertotexture.h"

class Plane{
public:
	Vector3 pos;
	float size;
	float distance;
	Vector2 start, end;
	void generateSprite(int nx, int ny, float xsize, float ysize){

		start.x = xsize*nx;
		start.y = ysize*ny;
		end.x = start.x + xsize;
		end.y = start.y + ysize;


	}
};

class Billboard: public Entity{
public:

	std::vector<Plane*> planes;

	Mesh * mesh;
	Material * material;
	Vector2 size;

	Billboard();
	~Billboard();

	void setMaterial(const char* filename);
	void addClouds(Vector3 & position, float size, float num);
	void addPlane(Vector3 & position, float size);

	void render(Renderer * r);
	void update();
};



class Sprite: public Billboard{
public:

	float xsize, ysize;
	Vector2 range;

	Sprite();
	~Sprite();

	void setMaterial(const char* filename, int rangex, int rangey);
	void addPlane(Vector3 & position, float size);
	void addPlane(Vector3 & position, float size, Vector2 coord);
	void addClouds(Vector3 & position, float size, float num);
	void addStorm(Vector3 start, Vector3 end , int num);
};

#endif