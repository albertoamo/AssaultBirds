#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "world.h"
#include "Hud.h"
#include "Technique.h"
#include "Renderer.h"

class GameState
{
public: 
	Hud * hud;
	Camera * camera;
	GameState();
	void setCamera(Camera * _camera);
	virtual void init();
	virtual void update(float time_elapsed);
	virtual void render(float time_elapsed);
};

class InGame : public GameState{
	InGame();
	virtual void init();
	virtual void update(float time_elapsed);
	virtual void render(float time_elapsed);
};

class InMenu : public GameState{
	InMenu();
	virtual void init();
	virtual void update(float time_elapsed);
	virtual void render(float time_elapsed);
};

class IsPaused : public GameState{
	IsPaused();
	virtual void init();
	virtual void update(float time_elapsed);
	virtual void render(float time_elapsed);
};

#endif