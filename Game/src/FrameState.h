#ifndef FRAMESTATE_H
#define FRAMESTATE_H
#include "utils.h"
#include "includes.h"
#include "Frame.h"
#include "MenuFactory.h"
#include "SoundManager.h"

class MainMenuState;

//FALTARIA CARGARSE EL MENU ANTERIOR

class FrameState{
public:
	Frame * frame;
	static MainMenuState * mainmenu;

	//virtual ~FrameState();
	virtual void handleInput(Game* game, Input input) = 0;
	virtual void update(Game* game, double  seconds_elapsed) = 0;
	virtual void renderFrame(Game* game) = 0;
};

class MainMenuState : public FrameState
{
public:
	
	float buttonTime;
	int buttonSelected;
	bool loopSound;

	MainMenuState(){
		buttonSelected = 0;
		buttonTime = 0.5;
		loopSound = false;
		frame = MenuFactory::MainMenu();
		SoundManager::Instance()->stopSound(WINSONG);
		SoundManager::Instance()->stopSound(LOSESONG);
		SoundManager::Instance()->playSound(BACKGROUND1);
		SoundManager::Instance()->stopSound(MOTORS);
		SoundManager::Instance()->stopSound(BACKDEF);
		SoundManager::Instance()->stopSound(ALARM);
	}

	void handleInput(Game* game, Input input);
	void handleState(Game* game, State state);
	void update(Game* game, double seconds_elapsed);
	void renderFrame(Game* game);

};

class LoadingMenuState : public FrameState
{
public:
	bool loaded;

	LoadingMenuState(){
		frame = MenuFactory::LoadingMenu();
		loaded = false;
		SoundManager::Instance()->stopSound(BACKGROUND1);
	}

	void handleInput(Game* game, Input input);
	void update(Game* game, double seconds_elapsed);
	void renderFrame(Game* game);

};

class SpawnIntermission : public FrameState
{
public:
	float time;
	int index;
	float previousTime;
	float adder;
	Frame * aux;

	SpawnIntermission(){
		
		index = 0;
		time = 1200;
		previousTime, adder = 0.0;
		aux = new Frame("auxiliar_intermission");
		frame = MenuFactory::IntermissionMenu();
		aux->addBackground(frame->background);
	}

	void handleInput(Game* game, Input input);
	void handleState(Game* game, State state);
	void update(Game* game, double seconds_elapsed);
	void renderFrame(Game* game);

};


class PlayingState : public FrameState
{
public:
	Frame * enemyQuads;

	PlayingState(){
		frame = MenuFactory::PlayingMenu();
		frame->background->setShader("postFX");
		//SoundManager::Instance()->playSound(INTROSONG);
	}

	void handleInput(Game* game, Input input);
	void handleState(Game* game, State state);
	void updateUpdatable();
	void update(Game* game, double  seconds_elapsed);
	void renderFrame(Game* game);
	void getProjectionPos();

};


class GameWinState : public FrameState
{
public:
	GameWinState(){
		frame = MenuFactory::GameWinMenu();
		SoundManager::Instance()->stopSound(INTROSONG);
		SoundManager::Instance()->playSound(WINSONG);
		SoundManager::Instance()->stopSound(MOTORS);
		SoundManager::Instance()->stopSound(BACKDEF);
	}

	void handleInput(Game* game, Input input);
	void handleState(Game* game, State state);
	void update(Game* game, double  seconds_elapsed);
	void renderFrame(Game* game);
	Frame * getProjectionPos();
	Frame * enemyQuads;

};

class GameOverState : public FrameState
{
public:
	GameOverState(){
		frame = MenuFactory::GameOverMenu();
		SoundManager::Instance()->stopSound(INTROSONG);
		SoundManager::Instance()->playSound(LOSESONG);
		SoundManager::Instance()->stopSound(MOTORS);
		SoundManager::Instance()->stopSound(BACKDEF);
		//frame->background->setShader("postFX");
	}

	void handleInput(Game* game, Input input);
	void handleState(Game* game, State state);
	void update(Game* game, double  seconds_elapsed);
	void renderFrame(Game* game);
	Frame * getProjectionPos();
	Frame * enemyQuads;

};

class ControlsMenuState : public FrameState
{
public:
	ControlsMenuState()
	{
		frame = MenuFactory::ControlsMenu();
		SoundManager::Instance()->stopSound(BACKGROUND1);
	}

	void handleInput(Game* game, Input input);
	void handleState(Game* game, State state);
	void update(Game* game, double  seconds_elapsed);
	void renderFrame(Game* game);

};

#endif