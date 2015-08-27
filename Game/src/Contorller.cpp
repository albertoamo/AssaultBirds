#include "Controller.h"
#include "world.h"
#include "coldet/coldet.h"

ControllerManager::ControllerManager()
{
	
}

Controller::Controller(): game(NULL), toControl(NULL)
{
	game = Game::instance;
}
