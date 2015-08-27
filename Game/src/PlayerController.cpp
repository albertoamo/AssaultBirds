#include "PlayerController.h"
#include "World.h"

bool PlayerController::started = false;

void PlayerController::init(Vehicle * _toControl)
{
	this->toControl = _toControl;

	game = Game::instance;
	camera = game->camera;

	camera->setPerspective(45, Game::instance->window_width / (float)Game::instance->window_height, 2, 100000);

	cameraTraslator = Vector3(0,500,-230);
	cameraSelector = 2;

	centerSelector = 2;
	centerDegrees = 0.0;
	centerTraslator[0] = Vector3(1,0,0);
	centerTraslator[1] = Vector3(0,1,0);
	centerTraslator[2] = Vector3(0,0,0);

	setCameraToEntity();

	Vector3 diePoint = Vector3(0,0,0);

	_toControl->setController(this);

	center = 30;

	rocketTime = 0.6;
	bombTime = 1.0;

	aux_min = 0.0;
	aux_radius = 0.0;
	aux_time = 9.5;
	startTime = 0.2;
	takeoff = false;

	aux_camChanged = false;

	if(!started)
	{
		setCameraToEntity();

		if(toControl->minSpeed != 0.0){
			aux_radius = toControl->totalRadius;
			aux_min = toControl->minSpeed;
		}

		toControl->minSpeed = 0.0;
		toControl->speed = 0.0;
		toControl->totalRadius = 0.;
	}
	else
	{
		aux_radius = toControl->totalRadius;
		aux_time = 0.0;
	}
}

void PlayerController::setCameraToEntity()
{
	Matrix44 model = toControl->Model;
	model.traslateLocal(cameraTraslator.x, cameraTraslator.y, cameraTraslator.z);

	if(centerSelector != 2){
		model.rotateLocal(centerDegrees, centerTraslator[centerSelector]);
	}

	camera->lookAt(model.getTranslationOnly(), toControl->origin+model.rotateVector(Vector3(0,0,1))*center, model.rotateVector(Vector3(0,1,0)));
	
	glViewport(0, 0, RESX, RESY);
	Game::instance->camera->aspect = RESX / (float)RESY;
	Game::instance->camera->setPerspective(Game::instance->camera->fov, RESX / (float)RESY, Game::instance->camera->near_plane, Game::instance->camera->far_plane);

	camera->set();

	glViewport(0, 0, Game::instance->window_width, Game::instance->window_height);
	Game::instance->camera->aspect = Game::instance->window_width / (float)Game::instance->window_height;
	Game::instance->camera->setPerspective(Game::instance->camera->fov, Game::instance->window_width / (float)Game::instance->window_height, Game::instance->camera->near_plane, Game::instance->camera->far_plane);
}

void PlayerController::update(double seconds_elapsed)
{

	const Uint8* keystate = game->keystate;
	JoystickState JState = game->JState;

	if(!started){//DETECTAMOS CUANDO QUEREMOS EMPEZAR
		startTime -= seconds_elapsed;
		if(startTime < 0.0)
			if(!started){
				started = true;
				SoundManager::Instance()->playSound(MOTORS);
				SoundManager::Instance()->playSound(BACKDEF);
			}
	} 
	else//CUANDO HAYAMOS EMPEZADO
	{
		//-------------------------HAZ EL DESPEGUE
		if(toControl->minSpeed < aux_min)
			toControl->minSpeed += seconds_elapsed*15;
		
		if(aux_time > 0)
		{
			aux_time -= seconds_elapsed;

			if(aux_time < 5.0)
				toControl->doYaw(seconds_elapsed/4.0);
			if(aux_time < 2.4){
				toControl->doPitch(seconds_elapsed/4);
				if(!takeoff){
					SoundManager::Instance()->playSound(TAKEOFF);
					takeoff = true;
				}
			}
			if(aux_time < 0.5 && !aux_camChanged)
			{
				toControl->totalRadius = aux_radius;
				changeCameraPoint();
				SoundManager::Instance()->playSound(INTROSONG);
				aux_camChanged = true;
			}

			returnCameraToPosition(seconds_elapsed);
			setCameraToEntity();
			return;
		} //SOLO LLEGAREMOS HASTA AQUI MIENTRAS DESPEGUE; BLOQUEAMOS USO AL USUARIO

	if(!toControl->living)
	{
		die(seconds_elapsed); //SI EL USUARIO HA MUERTO ACTUALIZAMOS SU MOVIMIENTO
		returnCameraToPosition(seconds_elapsed);
		setCameraToEntity();

		if(game->joystick != NULL)
		{
			if(JState.button[A_BUTTON])
				World::Instance()->controllersManager.getNextPlane(toControl); //PARA QUE PUEDA CAMBIAR RAPIDO
		}
		return; //CUANDO MORIMOS BLOQUEAMOS EL USO AL USUARIO
	}

	if (! game->mouse_locked ) //is left button pressed?
	{
		float speed = seconds_elapsed * 300;

		if(keystate[SDL_SCANCODE_1]) toControl->selectWeapon(1);
		if(keystate[SDL_SCANCODE_2]) toControl->selectWeapon(2);
		if(keystate[SDL_SCANCODE_3]) toControl->selectWeapon(3);

		if(keystate[SDL_SCANCODE_LSHIFT]) speed *= 10; //move faster with left shift

		if (keystate[SDL_SCANCODE_W]) toControl->accelerate(seconds_elapsed);
		if (keystate[SDL_SCANCODE_S]) toControl->speed =0.0f;

		if(keystate[SDL_SCANCODE_SPACE])
			toControl->isFiring = true;

			
		if (keystate[SDL_SCANCODE_KP_4]) toControl->doYawLeft(seconds_elapsed);
		if (keystate[SDL_SCANCODE_KP_6]) toControl->doYawRight(seconds_elapsed);
		if (keystate[SDL_SCANCODE_KP_8]) toControl->doPitchRight(seconds_elapsed);
		if (keystate[SDL_SCANCODE_KP_2]) toControl->doPitchLeft(seconds_elapsed);
		if (keystate[SDL_SCANCODE_KP_7]) toControl->doRollLeft(seconds_elapsed);
		if (keystate[SDL_SCANCODE_KP_9]) toControl->doRollRight(seconds_elapsed);

		if (keystate[SDL_SCANCODE_I]) toControl->outPosition.y += seconds_elapsed*0.5;
		if (keystate[SDL_SCANCODE_K]) toControl->outPosition.y -= seconds_elapsed*0.5;
		if (keystate[SDL_SCANCODE_J]) toControl->outPosition.x += seconds_elapsed*0.5;
		if (keystate[SDL_SCANCODE_L]) toControl->outPosition.x -= seconds_elapsed*0.5;
		if (keystate[SDL_SCANCODE_U]) toControl->outPosition.z += seconds_elapsed*0.5;
		if (keystate[SDL_SCANCODE_O]) toControl->outPosition.z -= seconds_elapsed*0.5;

		if(game->joystick != NULL)
		{
			center += seconds_elapsed*20;

			toControl->accelerate(seconds_elapsed*((JState.axis[5] + 1)/(float)2));

			if(abs(JState.axis[RIGHT_ANALOG_X])>0.1)toControl->doRoll(JState.axis[RIGHT_ANALOG_X]*seconds_elapsed);

			Entity*rot = toControl->getChildren("ax_f18_flap_RR");
			if(rot != NULL){
				float angleRot = 0.0;
				EntityMesh*rot2 = (EntityMesh*)rot;
				angleRot = -JState.axis[RIGHT_ANALOG_X]/2 + JState.axis[LEFT_ANALOG_Y]/2;
				rot2->rotateOverAngle(angleRot, Vector3(-1,0,0));

			
				rot = toControl->getChildren("ax_f18_flap_LL");
				rot2 = (EntityMesh*)rot;
				angleRot = JState.axis[RIGHT_ANALOG_X]/2 + JState.axis[LEFT_ANALOG_Y]/2;
				rot2->rotateOverAngle(angleRot, Vector3(-1,0,0));


				rot = toControl->getChildren("ax_f18_flap_BL");
				rot2 = (EntityMesh*)rot;
				rot2->rotateOverAngle(JState.axis[LEFT_ANALOG_X]/3, Vector3(0,1,0));

				rot = toControl->getChildren("ax_f18_flap_BR");
				rot2 = (EntityMesh*)rot;
				rot2->rotateOverAngle(JState.axis[LEFT_ANALOG_X]/3, Vector3(0, 1,0));
			}

			if(abs(JState.axis[LEFT_ANALOG_Y])>0.1) toControl->doPitch(JState.axis[LEFT_ANALOG_Y]*seconds_elapsed); //std::cout << JState.axis[1] << " 1 " << std::endl;
			if(abs(JState.axis[LEFT_ANALOG_X])>0.1) toControl->doYaw(JState.axis[LEFT_ANALOG_X]*seconds_elapsed); //std::cout << JState.axis[0] << " 0 " << std::endl; //toControl->rotateLocal(-seconds_elapsed*JState.axis[0], Vector3(0,0,1));

			if(cameraSelector == 2)
			{
				cameraTraslator.x += 10*-JState.axis[LEFT_ANALOG_X]*seconds_elapsed;
				cameraTraslator.y += 10*JState.axis[LEFT_ANALOG_Y]*seconds_elapsed;
				cameraTraslator.z -= 60*(toControl->speed / toControl->maxSpeed)*seconds_elapsed; 
			}

			if(cameraSelector == 1)
			{
				bool pressed = false;
				if(JState.button[RIGHT]) //if(JState.hat & HAT_RIGHT)
				{
					centerSelector = 1; 
					centerDegrees += seconds_elapsed/2;
					centerDegrees = clamp(centerDegrees, 0.0, 0.4);
					pressed = true;
				}
				if(JState.button[UP])//if(JState.hat & HAT_UP) 
				{
					centerSelector = 0; 
					centerDegrees += seconds_elapsed/2;
					centerDegrees = clamp(centerDegrees, 0.0, 0.4);
					pressed = true;
				}
				if(JState.button[LEFT])//if(JState.hat & HAT_LEFT)
				{
					centerSelector = 1; 
					centerDegrees -= seconds_elapsed/2;
					centerDegrees = clamp(centerDegrees, -0.4, 0.0);
					pressed = true;
				}

				if(!pressed)
				{
					returnCenterDegrees(seconds_elapsed);
				}
			}
			else
			{
				if(JState.button[LB_BUTTON]) 
				{
					float traslator = toControl->speed;
					traslator = clamp(traslator, 150, 300);
					cameraTraslator.z += seconds_elapsed*traslator;
					center -= seconds_elapsed*80;
				}
			}
			
			center = clamp(center, 0, 30);

			if(JState.button[A_BUTTON])
			{
				if(canPressA){
				if(World::Instance()->controllersManager.getNextPlane(toControl))
					return;
				}
			}
			else{
				canPressA = true;
			}

			rocketTime -= seconds_elapsed;
			if(JState.button[Y_BUTTON])
			{
				if(rocketTime < 0.0)
					shootRocket();
			}

			if(JState.button[B_BUTTON]) 
			{
				if(canPressB)
				{
					changeCameraPoint();
					canPressB = false;
				}
			}
			else
				canPressB = true;


			if(JState.button[X_BUTTON]) 
			{ 
				if(canPressX)
				{
					weaponSelected++;
					if(weaponSelected > 3)
						weaponSelected = 1;
					toControl->selectWeapon(weaponSelected);
					canPressX = false;
				}
			}
			else
					canPressX = true;

			//if(JState.button[LB_BUTTON]) toControl->decelerate(seconds_elapsed);//std::cout << "Pulsado LB." << std::endl;
			if(JState.button[RB_BUTTON]) toControl->isFiring = true;
			
			bombTime-= seconds_elapsed;
			if(JState.button[RIGHT_ANALOG_BUTTON]){
				if(bombTime < 0.0)
						shootBomb();
			}
			
			if(JState.button[BACK_BUTTON] || keystate[SDL_SCANCODE_P])
			{
				std::cout << "Para pausar juego" << std::endl;
			}

		}
		returnCameraToPosition(seconds_elapsed);
		setCameraToEntity();
	}
	else{
	glViewport(0, 0, RESX, RESY);
	Game::instance->camera->aspect = RESX / (float)RESY;
	Game::instance->camera->setPerspective(Game::instance->camera->fov, RESX / (float)RESY, Game::instance->camera->near_plane, Game::instance->camera->far_plane);

	camera->set();

	glViewport(0, 0, Game::instance->window_width, Game::instance->window_height);
	Game::instance->camera->aspect = Game::instance->window_width / (float)Game::instance->window_height;
	Game::instance->camera->setPerspective(Game::instance->camera->fov, Game::instance->window_width / (float)Game::instance->window_height, Game::instance->camera->near_plane, Game::instance->camera->far_plane);
	camera->up = Vector3(0,1,0);
	}
	}
}

void PlayerController::recibeDeath()
{
	if (!World::Instance()->controllersManager.getNextPlane(toControl)){
		World::Instance()->toDelete = true;
		Game::instance->handleInput(INPUT_TO_LOSE);
	}
}

void PlayerController::changeCameraPoint()
{
	if(cameraSelector == 1)
	{
		//cameraTraslator = toControl->outPosition;
		cameraSelector = 2;
		camera->setPerspective(45, Game::instance->window_width / (float)Game::instance->window_height, 2, 100000);
	}
	else
	{
		//cameraTraslator = toControl->cabinPosition;
		cameraSelector = 1;
		camera->setPerspective(45, Game::instance->window_width / (float)Game::instance->window_height, 0.1, 100000);
	}
}

void PlayerController::die(double seconds_elapsed)
{
	if(diePoint.x == 0 && diePoint.y == 0 && diePoint.z == 0)
	{
		Vector3 front = toControl->Model.rotateVector(Vector3(0,0,1));
		front.normalize();

		diePoint = toControl->origin + front*1000;
		diePoint.y -= 5000;	
	}
	toControl->maxSpeed += 50*seconds_elapsed;
	toControl->accelerate(10*seconds_elapsed);

	Vector3 to_target = (diePoint - toControl->origin);
	Vector3 aleatori;
	aleatori.random(0.7);
	Vector3 to_target_N = (to_target+aleatori).getNormalized();
	Vector3 front = (toControl->Model.rotateVector(Vector3(0,0,1))).normalize();

	float UpDownAngle = to_target_N.dot(toControl->Model.rotateVector(Vector3(0,1,0)));
	float RightLeftAngle = to_target_N.dot(toControl->Model.rotateVector(Vector3(1,0,0)));

	if(UpDownAngle < -0.01){toControl->doPitchLeft(seconds_elapsed);}
	if(UpDownAngle > 0.01){toControl->doPitchRight(seconds_elapsed);}

	if(RightLeftAngle < -0.01){toControl->doYawRight(seconds_elapsed);toControl->doRollRight(seconds_elapsed);}
	if(RightLeftAngle >  0.01){toControl->doYawLeft(seconds_elapsed);toControl->doRollLeft(seconds_elapsed);}

	toControl->doRollLeft(seconds_elapsed);
	
}

void PlayerController::returnCameraToPosition(double elapsed_time)
{
	if(cameraSelector == 1){
		if(elapsed_time> 1.0)
			cameraTraslator = toControl->cabinPosition;
		Vector3 director = toControl->cabinPosition - cameraTraslator;
		if(director.length() > 0.02)
			cameraTraslator = cameraTraslator + director*elapsed_time*4;
	}

	if(cameraSelector == 2){
		if(elapsed_time> 1.0)
			cameraTraslator = toControl->outPosition;
		Vector3 director = toControl->outPosition - cameraTraslator;
		if(director.length() > 0.02)
			cameraTraslator = cameraTraslator + director*elapsed_time*4;
	}
}

void PlayerController::returnCenterDegrees(double elapsed_time)
{
	if(centerSelector == 1)
	{
		if(centerDegrees < 0.0)
		{
			centerDegrees += elapsed_time/2;
			centerDegrees = clamp(centerDegrees, -0.4, 0.0);
		}
		if(centerDegrees > 0.0)
		{
			centerDegrees -= elapsed_time/2;
			centerDegrees = clamp(centerDegrees, 0.0, 0.4);
		}
	}
	if(centerSelector == 0)
	{
		centerDegrees -= elapsed_time/2;
		centerDegrees = clamp(centerDegrees, 0.0, 0.4);
	}
}

void PlayerController::shootRocket()
{
	//toControl->auxAmmoCounter;
	std::string nameRocket = std::string("ax_f18_rocket");
	nameRocket += std::to_string(toControl->RocketsCounter);
	Entity * rocket = toControl->getChildren(nameRocket.c_str());
	if(rocket != NULL)
	{
		Matrix44 newModel = rocket->getGlobalMatrix();
				
		EntityMesh * defaultRocket = (EntityMesh*)rocket;	

		Vehicle * newRocket = new Vehicle();
		newRocket->control = NULL;
		newRocket->options = toControl->options;
		newRocket->Model = newModel;
		newRocket->mesh = defaultRocket->mesh;
		newRocket->material = defaultRocket->material;
		newRocket->lowMesh = defaultRocket->lowMesh;
		newRocket->collision_model = NULL;
		newRocket->totalRadius = 10;

		newRocket->setMaxSpeed(toControl->maxSpeed+150.0);
		newRocket->setMinSpeed(toControl->speed + 150.0);
		newRocket->setPitchVel(0.8f);
		newRocket->setRollVel(0.8f);
		newRocket->setYawVel(0.4f);
		newRocket->setThrustSpeed(200.0f);
		newRocket->setBrakeSpeed(50.0f);
		newRocket->setLife(100.0f);

		newRocket->subType = ROCKET;
		newRocket->Name = toControl->Name;

		rocket->options = toControl->options;

		World::Instance()->models.push_back(newRocket);

		SoundManager::Instance()->playSound(MISSIL);

		toControl->RocketsCounter++;
	}
	rocketTime = 0.6;

}

void PlayerController::shootBomb()
{
	//toControl->auxAmmoCounter;
	std::string nameBomb = std::string("ax_f18_bomb");
	nameBomb += std::to_string(toControl->BombsCounter);
	Entity * bomb = toControl->getChildren(nameBomb.c_str());
	if(bomb != NULL)
	{
		Matrix44 newModel = bomb->getGlobalMatrix();
		EntityMesh * defaultBomb = (EntityMesh*)bomb;

		Vehicle * newBomb = new Vehicle();
		newBomb->control = NULL;
		newBomb->options = toControl->options;
		Vector3 rotation = toControl->Model.rotateVector(Vector3(0,0,1));
		rotation.normalize();
		float angle = rotation.dot(Vector3(1,0,0));

		newBomb->Model.setIdentity();
		Vector3 Position = toControl->Model.getTranslationOnly();
		newBomb->Model.traslate(Position.x, Position.y, Position.z);
		newBomb->rotateLocal(M_PI, Vector3(0,1,0));


		newBomb->mesh = defaultBomb->mesh;
		newBomb->material = defaultBomb->material;
		newBomb->lowMesh = defaultBomb->lowMesh;
		newBomb->collision_model = NULL;
		newBomb->totalRadius = 60;

		newBomb->setMaxSpeed(toControl->maxSpeed);
		newBomb->setMinSpeed(toControl->speed);
		newBomb->setPitchVel(0.0f);
		newBomb->setRollVel(0.0f);
		newBomb->setYawVel(0.0f);
		newBomb->setThrustSpeed(200.0f);
		newBomb->setBrakeSpeed(50.0f);
		newBomb->setLife(100.0f);

		newBomb->subType = BOMB;
		newBomb->Name = toControl->Name;

		bomb->options = toControl->options;

		World::Instance()->models.push_back(newBomb);

		SoundManager::Instance()->playSound(MISSIL);

		toControl->BombsCounter++;
	}
	bombTime = 0.6;
}