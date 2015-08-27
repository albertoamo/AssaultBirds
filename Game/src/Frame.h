#ifndef FRAME_H
#define FRAME_H
#include "Label.h"

enum State
{
	STATE_MAIN,
	STATE_OPTIONS,
	STATE_LOAD,
	STATE_INTERMISSION,
	STATE_PLAYING,
	STATE_DEFAULT,
	STATE_PLAY,
	STATE_EXIT,
	STATE_END,
	STATE_WIN,
	STATE_HELP
};

enum Input
{
	INPUT_PLAY,
	INPUT_MOUSEDOWN,
	INPUT_SPACEBAR,
	INPUT_OPTIONS,
	INPUT_BUTTON_A,
	INPUT_BUTTON_START,
	INPUT_NEXTSTATE,
	INPUT_BUTTON_SELECT,
	INPUT_TO_WIN,
	INPUT_TO_LOSE

};

class Container: public Observer{
public:
	Vector2 origin;
	Vector2 size;
	Vector2 state;
	bool hovered;

	Container(){}
	~Container(){}

	virtual void render(int primitive, Camera * camera) = 0;
	virtual void update(Game & game) = 0;
	virtual void checkHover(Vector2 mousepos) = 0;
	virtual void setHover(){};
	virtual void unHover(){};
	virtual bool checkMouseHovered(Vector2 mousepos){return false;}
	virtual std::string checkSelect(Vector2 mousepos) = 0;
	virtual std::string checkSelect(){return "";};

};

class Button : public Container{
public:
	Hud* back;
	Label* title;
	bool hover;
	bool mouseOver;

	Button(Vector2 position, Vector2 size, const char * text);
	void render(int primitive, Camera * camera);
	void update(Game & game);
	void checkHover(Vector2 mousepos);
	bool checkMouseHovered(Vector2 mousepos);
	void setHover();
	void unHover();
	std::string checkSelect(Vector2 mousepos);
	std::string checkSelect();
	~Button();

};

class ProgressBar : public Container{
public:
	Hud* back;
	Hud* bar;

	ProgressBar(Vector2 position, Vector2 size, Vector3 color);
	void render(int primitive, Camera * camera);
	void update(Game & game);
	void checkHover(Vector2 mousepos){}
	std::string checkSelect(Vector2 mousepos){ return ""; }
	void updateSize(float size);
	~ProgressBar();

};

class Frame{
public:

	const char* name;

	Hud * background;
	std::vector<Hud*> huds;
	std::vector<Hud*> updatable;
	std::vector<Container*> containers;

	Frame(const char* name);
	~Frame();

	void update();
	void render(Camera * camera);

	void addHud(Hud * hud);
	void addUpdatable(Hud * hud);
	void addBackground(Hud * hud);
	void addContainer(Container * container);
	void addTimed(Hud * hud, float time);
	void fadeDie(float time);
	void clearHuds();
	void clearUpdatable();
	State checkContainers(Vector2 mousepos);
	State checkContainers();
	State mapTypes(const char * type);

};


#endif