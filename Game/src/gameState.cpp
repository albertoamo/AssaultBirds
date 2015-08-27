#include "gameStateh.h"

GameState::GameState():hud(NULL), camera(NULL){}

void GameState::setCamera(Camera * _camera)
{
	camera = _camera;
}

InGame::InGame():GameState(){}

InMenu::InMenu():GameState(){}

IsPaused::IsPaused():GameState(){}