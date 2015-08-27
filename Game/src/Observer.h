#ifndef OBSERVER_H
#define OBSERVER_H
#include <set>
#include "framework.h"

class Game;

class Observer {
public:
	virtual void update(Game & game) = 0;
	//virtual void update(World & world) = 0;
	//virtual void update(World & world) = 0;
};

//This is keep here as an example, deprecated.
/*
class ObserverWindow : public Observer
{
	Vector2 state;
public:
	
	void update(Game & game);
	Vector2 getState();
};
*/
#endif