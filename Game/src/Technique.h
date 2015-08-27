#ifndef TECHNIQUE_H
#define TECHNIQUE_H

#include "includes.h"
#include "init/loader.h"
#include "shader.h"
#include "framework.h"
#include "EntityMesh.h"
#include "rendertotexture.h"
#include "Observer.h"

class Renderer;
class RenderStack;
class World;

class Technique : public Observer{
public:
	int primitive;
	RenderStack * stack;
	//ObserverWindow * w_listener;
	Vector2 state;

	Technique();
	~Technique();

	virtual void init(Camera * camera, EntityMesh* caster);
	virtual void render(Renderer * r);
	virtual void update(Camera * camera);
	virtual void update(Game & game) = 0;
	virtual void setResolution(Camera * camera) const;
	virtual void downResolution(Camera * camera) const;
	bool loadShader(int & prim, const char * dir);
	Vector2 getState();
};

class ForwardRendering : public Technique{
public:
	RenderToTexture * screen;
	World * world;

	ForwardRendering(int prim, const char * dir);
	~ForwardRendering();

	void init(World * world);
	void render(Renderer * r);
	void update(Camera * camera);

	//Observing the size
	void update(Game & game);
};

class ShadowMapping : public Technique{
public:
	RenderToTexture * shadowmap;
	std::vector<EntityMesh*> caster;
	std::vector<EntityLight*> lights;
	std::vector<Camera*> cameras;

	ShadowMapping(int prim, const char * dir);
	~ShadowMapping();

	void init(std::vector<EntityMesh*> inner, std::vector<EntityLight*> lights);
	void render(Renderer * r);
	void update(Camera * camera);

	//Observing the size
	void update(Game & game);
};

class MinimapRendering : public Technique{
public:
	RenderToTexture * minimap;
	World * world;
	Shader * iconshader;

	MinimapRendering(int prim, const char * dir);
	~MinimapRendering();

	void init(World * world);
	void render(Renderer * r);
	void update(Camera * camera);

	//Observing the size
	void update(Game & game);
	void renderIcons() const;
	void generateIcon(Vector3 & pos, const char * icon) const;
	void renderPops(Mesh * icono, Texture * tex, Matrix44 model) const;
};

class Reflection: public Technique{
public:
	RenderToTexture * reflection;
	std::vector<EntityMesh*> caster;
	EntityMesh* emitter;

	Reflection(int prim, const char * dir);
	~Reflection();

	void init(Camera * camera, EntityMesh* caster);
	void render(Renderer * r);
	void update(Camera * camera);

	//Observing the size
	void update(Game & game);
};

#endif /* TECHNIQUE_H */