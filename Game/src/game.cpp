#include "game.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "EntityMesh.h"
#include <iostream>
#include "EntityLight.h"
#include <cmath>
#include "world.h"
#include "init/utility.h"
#include <fstream>
#include "NodeFactory.h"

#include "SoundManager.h"

//some globals
Game* Game::instance = NULL;
World* World::instance = NULL;

SoundManager * SoundManager::instance = NULL;

float debug_time;

Game::Game(SDL_Window* window)
{
	debug_time = 0.0f;

	this->window = window;
	instance = this;

	// initialize attributes
	// Warning: DO NOT CREATE STUFF HERE, USE THE INIT 
	// things create here cannot access opengl
	SDL_GetWindowSize( window, &window_width, &window_height );
	std::cout << " * Window size: " << window_width << " x " << window_height << std::endl;

	keystate = NULL;
	joystick = openJoystick(0);

	mouse_locked = false;
}

void Game::deleteGame()
{
	Shader::deleteShaders();
	Mesh::deleteMeshes();
	Texture::deleteTextures();

	delete(camera);
	delete(World::Instance());
	delete(renderer);
}

//Here we have already GL working, so we can create meshes and textures
void Game::init(void)
{

	std::cout << " * Path: " << getPath() << std::endl;
	this->changeState(Vector2(window_width, window_height));

	//OpenGL flags
	glEnable(GL_CULL_FACE); //render both sides of every triangle
	glEnable(GL_DEPTH_TEST); //check the occlusions using the Z buffer

	hudcamera = new Camera();
	hudcamera->setOrthographic(0, window_width, window_height, 0, -1, 1);
	this->renderer = new Renderer();
	this->camera = new Camera();

	SoundManager::Instance()->pushSound("assets/sound/weapons/ShootTest.wav", FIRE1, SHOOTS);
	SoundManager::Instance()->pushSound("assets/sound/weapons/ShootTest2.wav", FIRE2, SHOOTS);
	SoundManager::Instance()->pushSound("assets/sound/weapons/Shoot3.mp3", FIRE3, SHOOTS);
	SoundManager::Instance()->pushSound("assets/sound/music/BackGround-MH.wav", BACKGROUND1, BACKGROUND, true);
	SoundManager::Instance()->pushSound("assets/sound/gui/letter_move.wav", LETTERMOVE, MENU);
	SoundManager::Instance()->pushSound("assets/sound/gui/mouse_ylover.wav", MOUSEOVER, MENU);
	SoundManager::Instance()->pushSound("assets/sound/music/intro_song3.wav", INTROSONG, BACKGROUND);
	SoundManager::Instance()->pushSound("assets/sound/music/lose_song.wav", LOSESONG, MENU);
	SoundManager::Instance()->pushSound("assets/sound/music/cccp_anthem.wav", WINSONG, MENU);
	SoundManager::Instance()->pushSound("assets/sound/effects/takeoff.wav", TAKEOFF, EFFECTS);
	SoundManager::Instance()->pushSound("assets/sound/effects/swap_camera2.wav", SWAPCAM, EFFECTS);
	SoundManager::Instance()->pushSound("assets/sound/effects/motors.wav", MOTORS, EFFECTS, true);
	SoundManager::Instance()->pushSound("assets/sound/effects/mouse_ylselect.wav", MSELECT, EFFECTS);
	SoundManager::Instance()->pushSound("assets/sound/effects/explo_distant01.wav", EXPL1, EFFECTS);
	SoundManager::Instance()->pushSound("assets/sound/effects/explo_distant02.wav", EXPL2, EFFECTS);
	SoundManager::Instance()->pushSound("assets/sound/effects/explo_distant03.wav", EXPL3, EFFECTS);
	SoundManager::Instance()->pushSound("assets/sound/ambient/background_default.wav", BACKDEF, BACKGROUND);
	SoundManager::Instance()->pushSound("assets/sound/effects/mp_hit_indication_3c.wav", HIT, EFFECTS);
	SoundManager::Instance()->pushSound("assets/sound/effects/alarm_altitude_01.wav", ALARM, EFFECTS);
	SoundManager::Instance()->pushSound("assets/sound/effects/missil_launch.wav", MISSIL, SHOOTS);

	this->frameState = new MainMenuState();
}

//what to do when the image has to be draw
void Game::render(void)
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	renderFrame();
	//calculateFPS();
	//drawFPS();

	SDL_GL_SwapWindow(this->window);
}

void Game::update(double seconds_elapsed)
{

	if(World::Instance()->toDelete == true)
		toMenu();

	//JOYSTICK
	if(joystick != NULL){
		JState = getJoystickState(joystick);
		if(JState.button[START_BUTTON]) handleInput(INPUT_BUTTON_START);
		if(JState.button[BACK_BUTTON]) toMenu();
	}

	double speed = seconds_elapsed * 100;
	debug_time += seconds_elapsed;

	this->updateFrames(seconds_elapsed);
	if (keystate[SDL_SCANCODE_SPACE]) handleInput(INPUT_SPACEBAR);

	//mouse input to rotate the cam
	if ((mouse_state & SDL_BUTTON_LEFT) || mouse_locked ){

		handleInput(INPUT_MOUSEDOWN);

		if(keystate[SDL_SCANCODE_LSHIFT]) speed *= 10;

		camera->rotate(mouse_delta.x * 0.005, Vector3(0,-1,0));
		camera->rotate(mouse_delta.y * 0.005, camera->getLocalVector( Vector3(-1,0,0)));

		if (keystate[SDL_SCANCODE_W]){
			camera->move(Vector3(0, 0, 1) * speed);
		}
		if (keystate[SDL_SCANCODE_S]){
			camera->move(Vector3(0, 0, -1) * speed);
		}
		if (keystate[SDL_SCANCODE_A]){
			camera->move(Vector3(1, 0, 0) * speed);
		}
		if (keystate[SDL_SCANCODE_D]){
			camera->move(Vector3(-1, 0, 0) * speed);
		}
		//camera->set();
	}



	//to navigate with the mouse fixed in the middle
	if (mouse_locked){
		int center_x = floor(window_width*0.5);
		int center_y = floor(window_height*0.5);
        //center_x = center_y = 50;
		SDL_WarpMouseInWindow(this->window, center_x, center_y); //put the mouse back in the middle of the screen
		//SDL_WarpMouseGlobal(center_x, center_y); //put the mouse back in the middle of the screen
        
        this->mouse_position.x = center_x;
        this->mouse_position.y = center_y;
	}
    
}

//Keyboard event handler (sync input)
void Game::onKeyPressed( SDL_KeyboardEvent event )
{
	Vector3 dir = camera->center - camera->eye;
	dir.normalize();

	switch(event.keysym.sym)
	{
	case SDLK_ESCAPE: 	
		std::cout << "Salimos" << std::endl,
		deleteGame();
		exit(0); //ESC key, kill the app
	break;
	case SDLK_m:
		SoundManager::Instance()->playSound(BACKGROUND1);
	break;

	case SDLK_l: 
		std::cout << "Punto Capturado." << std::endl;
		std::cout << camera->eye.x << " " << camera->eye.y << " " << camera->eye.z << std::endl;
	break;

	case SDLK_p:
		for(unsigned int i = 1; i < 3; i++){ 
			Vector3 Collision = Vector3(0,0,0);
			EntityMesh * ent = World::Instance()->models[i];
			if(ent->collision_model != NULL)
			if(ent->collision_model->rayCollision(camera->eye.v, (camera->center - camera->eye).v, true))
			{
				ent->collision_model->getCollisionPoint(Collision.v, false);
				std::cout << "Colision en: " << Collision.x << "  " << Collision.y << "  " << Collision.z << std::endl;
				//World::Instance()->debug->vertices.push_back(Collision);
				World::Instance()->debugParticle->addParticle(Collision);
				break;
			}
		}
	break;

	}
}


void Game::onMouseButton( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_MIDDLE) {
		mouse_locked = !mouse_locked;
		//SDL_ShowCursor(!mouse_locked);
	}
}

void Game::setWindowSize(int width, int height)
{
    std::cout << "window resized: " << width << "," << height << std::endl;
    Uint32 flags = SDL_GetWindowFlags(window);

	glViewport( 0,0, width, height );
	camera->aspect =  width / (float)height;
	window_width = width;
	window_height = height;

	camera->setPerspective(45, window_width / (float)window_height, 1, 100000);
	this->changeState(Vector2(window_width, window_height));
	hudcamera->setOrthographic(0, width, height, 0, -1, 1);
}

SDL_Joystick* Game::openJoystick(int num_joystick)
{
	// Initialize the joystick subsystem
	SDL_InitSubSystem(SDL_INIT_JOYSTICK);

	// Check for number of joysticks available
	if(SDL_NumJoysticks() <= num_joystick)
		return NULL;

	// Open joystick and return it
	return SDL_JoystickOpen(num_joystick);
}

JoystickState Game::getJoystickState(SDL_Joystick* joystick)
{
	JoystickState state;
	memset(&state,0,sizeof(JoystickState)); //clear

	if ( joystick == NULL )
	{
		//std::cout << "Error: Joystick not opened" << std::endl;
		return state;
	}

	state.num_axis = SDL_JoystickNumAxes((::SDL_Joystick*) joystick);
	state.num_buttons = SDL_JoystickNumButtons( (::SDL_Joystick*)joystick);

	if (state.num_axis > 8) state.num_axis = 8;
	if (state.num_buttons > 16) state.num_buttons = 16;

	for (int i = 0; i < state.num_axis; ++i) //axis
		state.axis[i] = SDL_JoystickGetAxis((::SDL_Joystick*) joystick,i) / 32768.0f; //range -32768 to 32768
	for (int i = 0; i < state.num_buttons; ++i) //buttons
		state.button[i] = SDL_JoystickGetButton((::SDL_Joystick*) joystick,i);
	state.hat = (HATState)(SDL_JoystickGetHat((::SDL_Joystick*) joystick,0) - SDL_HAT_CENTERED); //one hat is enough

	return state;
}

void Game::toMenu()
{
	handleInput(INPUT_BUTTON_SELECT);
	delete(World::Instance());
}