#ifndef LABEL_H
#define LABEL_H
#include "framework.h"
#include "shader.h"
#include "Hud.h"

#define SPRITE_W 16
#define SPRITE_H 16

class Sprite2D {
public:

	Vector2 start, end;
	Vector2 range;
	float ratio;
	
	Sprite2D();

};

typedef struct{

	int index;
	float time;
	float previousTime;
	std::string buildWord;
	std::string totalWord;
	bool active;

}Roller;


//I want to keep the mesh in Quad object, so that later we can easily change letters or make some cool effects with them.
class Label: public Hud{
public:
	std::string font;
	std::string word;
	float fontSize;
	float addit;
	int blendtype;

	Roller roll;

	Label();
	~Label();
	Label(Vector2 position, Vector3 color, int size);
	bool loadFont(const char* fontfile, const char* shader);
	void setText(const char* text);
	void setFamilyFont(const char* font, float fontSize);
	void insertOverTime(float time);
	void render(int primitive, Camera * camera);
	void addGlow(Vector3 color);
	void update(Game & game);
	void readyFade();

	Sprite2D mapper(const char letter, const int index);
	Sprite2D getSprite(int nx, int ny, float pct);
};

class TextBox : public Hud{
public:

	std::vector<Label*> labels;

	TextBox();
	TextBox(Vector2 position, Vector3 color, int size);
	bool loadFont(const char* fontfile, const char* shader);
	void setText(const char* text);
	void setFamilyFont(const char* font, float fontSize);
	void insertOverTime(float time);
	void render(int primitive, Camera * camera);
	void addGlow(Vector3 color);
	void update(Game & game);

	Sprite2D mapper(const char letter, const int index);
	Sprite2D getSprite(int nx, int ny, float pct);
};

#endif
