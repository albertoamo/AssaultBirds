#ifndef HUD_H
#define HUD_H
#include "includes.h"
#include "shader.h"
#include "framework.h"
#include "mesh.h"
#include "Renderer.h"
#include "Observer.h"

typedef struct{

	float time;
	float previousTime;
	bool fade;
	Vector3 color;

}Fader;


class Hud: public Observer{
public:

	Hud * parent;
	std::vector<Hud*> children;

	Mesh * mesh;
	Shader* shader;
	Texture * tex;
	Matrix44 model;

	Fader fading;

	Vector3 color;
	Vector2 origin;
	Vector2 size;

	float alpha;
	int z_index;


	Vector2 state;
	//Controls * input;

	Hud();
	~Hud();
	Hud(int x, int y, Vector2 size, int index);
	Hud(Vector2 origin, Vector2 size, int index);
	Hud(int x, int y, Vector2 size, std::string type);
	Hud(Vector2 origin, Vector2 size, std::string type);

	bool setTexture(const char* texture, const char * shaName = "graphic");
	bool setShader(const char * shaderName);
	void setLabel(std::string & label, const char* font, float size);
	void createHud();
	void addBorder(float size);
	void addChildren(Hud * hud);
	void deleteChildren();

	void update(Game & game);
	virtual void render(int primitive, Camera * camera);
	Matrix44 getGlobalMatrix();

	void setFade(float time);
	void setFade(Vector3 color, float time);
	void fadeOverTime();
	void setColorFade();
	void setDepth(int depth);
	void setColor(Vector3 color);
	void translate(Vector3 Traslator);

};
#endif