/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	This class encapsulates the game, is in charge of creating the game, getting the user input, process the update and render.
*/

#ifndef GAME_H
#define GAME_H

#include "includes.h"
#include "camera.h"
#include "Renderer.h"
#include "Observer.h"
#include "FrameState.h"

//#include "Joystick_SDL.h"

class Game
{
public:
	static Game* instance;

	//window
	SDL_Window* window;
	int window_width;
	int window_height;
    
	Vector2 window_size;

    float time;

	//keyboard state
	const Uint8* keystate;
	FrameState* frameState;

	//Joystick state
	SDL_Joystick * joystick;
	JoystickState JState;
	SDL_Joystick* openJoystick(int num_joystick);
	JoystickState getJoystickState(SDL_Joystick* joystick);

	//mouse state
	int mouse_state; //tells which buttons are pressed
	Vector2 mouse_position; //last mouse position
	Vector2 mouse_delta; //mouse movement in the last frame
	bool mouse_locked; //tells if the mouse is locked (not seen)
	
	Camera* camera; //our global camera
	Camera * hudcamera;
	Renderer * renderer;

	Game(SDL_Window* window);
	//~Game();
	void deleteGame();
	void init( void );
	void render( void );
	void update( double dt );

	void onKeyPressed( SDL_KeyboardEvent event );
	void onMouseButton( SDL_MouseButtonEvent event );
    void onResize( SDL_Event e );
	void setWindowSize(int width, int height);

	void toMenu();

	std::set<Observer*> observers;

	//Observer pattern
	void attachObserver(Observer *o) { observers.insert(o); o->update(*this); }
	void detachObserver(Observer *o) { observers.erase(o); }
	void notifyObservers()
	{
		for (auto &o : observers)
		{
			o->update(*this);
		}
	}
	Vector2 getState() { return window_size; }
	void changeState(const Vector2 & s)
	{
		window_size = s;
		notifyObservers();
	}
	//State Pattern
	virtual void handleInput(Input input){
		frameState->handleInput(this, input);
	}

	virtual void updateFrames(double seconds_elapsed){
		frameState->update(this, seconds_elapsed);
	}
	virtual void renderFrame(){
		frameState->renderFrame(this);
	}
};


#endif 