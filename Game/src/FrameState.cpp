#include "FrameState.h"
#include "game.h"
#include "world.h"
#include "includes.h"

//MainMenuState * FrameState::mainmenu = new MainMenuState();

void MainMenuState::handleInput(Game* game, Input input) {

	if (input == INPUT_MOUSEDOWN){
		State stated = frame->checkContainers(game->mouse_position);
		handleState(game, stated);
	}
	if(input == INPUT_BUTTON_A)
	{
		std::cout << "A button Pressed" << std::endl;
		State stated = frame->checkContainers();
		handleState(game, stated);
	}
	if (input == INPUT_PLAY){
		// Change to standing state...
		//game.setCurrentFrame(IMAGE_STAND);
	}
}

void MainMenuState::handleState(Game* game, State state){

	switch (state){
		case STATE_PLAY: 
			game->frameState = new LoadingMenuState();
		break;
		case STATE_EXIT:
			std::cout << "BYE BYE CYA SOON" << std::endl;
			exit(0);
		break;
		case STATE_DEFAULT:
			game->frameState = new ControlsMenuState();
			break;
	}
}

void MainMenuState::update(Game* game, double  seconds_elapsed) {


	buttonTime += seconds_elapsed;
	if(game->joystick != NULL)
	{
		if(game->JState.button[UP] && buttonTime >= 0.3)
		{
			buttonSelected--;
			buttonTime = 0.0;
		}
		if(game->JState.button[DOWN] && buttonTime >= 0.3)
		{
			buttonSelected++;
			buttonTime = 0.0;
		}

		if(game->JState.button[A_BUTTON])
		{
			handleInput(game, INPUT_BUTTON_A);
		}

		if(buttonSelected > 3)
			buttonSelected = 0;
		if(buttonSelected < 0)
			buttonSelected = 3;

		for (unsigned int x = 0; x < frame->containers.size(); x++)
			frame->containers[x]->unHover();
	}

	Vector2 moused = game->mouse_position;
	for (unsigned int x = 0; x < frame->containers.size(); x++){
		if(frame->containers[x]->checkMouseHovered(moused))
			buttonSelected = x;
	}
	frame->containers[buttonSelected]->setHover();
}

void MainMenuState::renderFrame(Game* game) {
	this->frame->render(game->hudcamera);
}


void LoadingMenuState::handleInput(Game* game, Input input) {

	if (input == INPUT_SPACEBAR){
		std::cout << "MOUSE PRESSED" << std::endl;
		game->frameState = new SpawnIntermission();
		//game->frameState = new PlayingState();
	}
	if(input == INPUT_BUTTON_START)
	{
		game->frameState = new SpawnIntermission();
		//game->frameState = new PlayingState();
	}
}

void LoadingMenuState::update(Game* game, double  seconds_elapsed) {

	if (loaded == false){

		((ProgressBar*)frame->containers[0])->updateSize(0);
		World::Instance()->Init("sp_test");
		((ProgressBar*)frame->containers[0])->updateSize(10);
		World::Instance()->loadWorld(game->window_width, game->window_height);
		this->frame->render(game->hudcamera);
		((ProgressBar*)frame->containers[0])->updateSize(60);
		this->frame->render(game->hudcamera);
		World::Instance()->doSpawn();
		((ProgressBar*)frame->containers[0])->updateSize(90);
		this->frame->render(game->hudcamera);
	
		((ProgressBar*)frame->containers[0])->updateSize(90);
		frame->huds.pop_back();
		Label * label = new Label(Vector2(27, 90), Vector3(85, 134, 93), 30);
		label->addGlow(Vector3(0, 84, 15));
		label->setText("Press [START] to continue");
		label->setFade(1000);
		frame->addHud(label);

		loaded = true;
		((ProgressBar*)frame->containers[0])->updateSize(100);
	}
}
void LoadingMenuState::renderFrame(Game* game) {

	this->frame->render(game->hudcamera);
}

void SpawnIntermission::handleInput(Game* game, Input input) {

	if (input == INPUT_MOUSEDOWN){
		std::cout << "MOUSE PRESSED" << std::endl;
		State stated = frame->checkContainers(game->mouse_position);
		handleState(game, stated);
	}
}

void SpawnIntermission::handleState(Game* game, State state){

	switch (state){
	case STATE_PLAYING:
		std::cout << "YOU PLAY" << std::endl;
		game->frameState = new PlayingState();
		break;
	}
}

void SpawnIntermission::update(Game* game, double  seconds_elapsed){

	float currentTime = SDL_GetTicks();
	int timeInterval = currentTime - this->previousTime;

	if (timeInterval > 1600){
		previousTime = currentTime;
		if (index < 4){
			aux->addHud(frame->huds[index]);
			index++;
		}
		adder = adder + 1600;
	}
	if (adder > 8000){ aux->fadeDie(timeInterval); }
	if (adder > 10500){ handleState(game, STATE_PLAYING); }

}
void SpawnIntermission::renderFrame(Game* game) {

	World::Instance()->renderWorld(game->camera, game->renderer);
	this->aux->render(game->hudcamera);
}


void PlayingState::handleInput(Game* game, Input input) {
	switch(input)
	{
	case INPUT_BUTTON_SELECT:
		handleState(game, STATE_MAIN);
		break;
	case INPUT_TO_LOSE:
		handleState(game, STATE_END);
		break;
	case INPUT_TO_WIN:
		handleState(game, STATE_WIN);
		break;
	}
}

void PlayingState::handleState(Game* game, State state){

	switch (state){
		case STATE_MAIN:
			game->frameState = new MainMenuState();
			break;
		case STATE_END:
			game->frameState = new GameOverState();
			break;
		case STATE_WIN:
			game->frameState = new GameWinState();
			break;
	}
}

void PlayingState::update(Game* game, double  seconds_elapsed) {

	frame->background->tex = ((ForwardRendering*)World::Instance()->techniques[1])->screen;
	frame->huds[0]->tex = ((MinimapRendering*)World::Instance()->techniques[2])->minimap;
	World::Instance()->update(seconds_elapsed);
}
void PlayingState::renderFrame(Game* game) {

	getProjectionPos();
	World::Instance()->renderWorld(game->camera, game->renderer);
	this->frame->render(game->hudcamera);
}

//Vector Coords son las coordenadas NO homogeneas del punto _pos en espacio de camara
bool getPointToCamCoords(Vector3 _pos, Vector2 & coords)
{
		glViewport(0, 0, RESX, RESY);
		Game::instance->camera->aspect = RESX / (float)RESY;
		Game::instance->camera->setPerspective(Game::instance->camera->fov, RESX / (float)RESY, Game::instance->camera->near_plane, Game::instance->camera->far_plane);

		Game::instance->camera->updateViewMatrix();
		Matrix44 viewProjection = Game::instance->camera->viewprojection_matrix;
		Vector4 posToCam = Vector4(_pos.x, _pos.y, _pos.z, 1.0);
		posToCam = viewProjection*posToCam;
		posToCam.x = posToCam.x/posToCam.w;
		posToCam.y = posToCam.y/posToCam.w;
		posToCam.z = posToCam.z/posToCam.w;

		posToCam.x += 1;
		posToCam.x *= RESX;
		posToCam.x /= 2;

		posToCam.y += 1;
		posToCam.y *= RESY;
		posToCam.y /= 2;

		std::string posToPrint = "";

		if(posToCam.x < 0.0){
			posToCam.x = 0.0;
			posToPrint += " izquierda ";
		}

		if(posToCam.w < 0.0){
			posToPrint = "Detras";
			return false;
		}

		coords.x = ((posToCam.x * 100)/RESX)-2;
		coords.y= (100-(posToCam.y * 100)/RESY)-3;

		glViewport(0, 0, Game::instance->window_width, Game::instance->window_height);
		Game::instance->camera->aspect = Game::instance->window_width / (float)Game::instance->window_height;
		Game::instance->camera->setPerspective(Game::instance->camera->fov, Game::instance->window_width / (float)Game::instance->window_height, Game::instance->camera->near_plane, Game::instance->camera->far_plane);

		return true;
}

void PlayingState::updateUpdatable(){

	Vehicle * veh = World::Instance()->controllersManager.getPlayer();

	Label * mission1 = new Label(Vector2(0.5, 4), Vector3(85, 134, 93), 5);
	mission1->size = Vector2(9, 12);
	mission1->setText("Mission 1: Protect the ships convoy on the strait.");
	frame->addUpdatable(mission1);

	Label * mission2 = new Label(Vector2(0.5, 6), Vector3(85, 134, 93), 5);
	mission2->size = Vector2(9, 12);
	std::string commRemaining = std::to_string(World::Instance()->enemyCommunications);
	std::string text2 = "Mission 2: Destroy the enemy communications." + std::string(" [" + commRemaining +" remaining]");
	mission2->setText(text2.c_str());
	frame->addUpdatable(mission2);

	Label * mission3 = new Label(Vector2(0.5, 8), Vector3(85, 134, 93), 5);
	mission3->size = Vector2(9, 12);
	std::string headRemm = std::to_string(World::Instance()->enemyHeadcuarters);
	std::string text3 = "Mission 3: Destroy the enemy headquarters." + std::string(" [" + headRemm + " remaining]");
	mission3->setText(text3.c_str());
	frame->addUpdatable(mission3);

	Label * mission4 = new Label(Vector2(0.5, 10), Vector3(85, 134, 93), 5);
	mission4->size = Vector2(9,12);
	std::string bombRemm = std::to_string(World::Instance()->enemyBombardiers);
	std::string text4 = "Mission 4: Destroy the enemy bombardiers." + std::string (" [" + bombRemm + " remaining]");
	mission4->setText(text4.c_str());
	frame->addUpdatable(mission4);

	std::string rocketnumStr = "[" + std::to_string(7-veh->RocketsCounter) + "]";

	Label * rocketnum = new Label(Vector2(85, 83.5), Vector3(85, 134, 93), 5);
	rocketnum->size = Vector2(12, 12);
	rocketnum->setText(rocketnumStr.c_str());
	frame->addUpdatable(rocketnum);

	std::string bombnumStr = "[" + std::to_string(4-veh->BombsCounter) + "]";

	Label * bombnum = new Label(Vector2(93, 83.5), Vector3(85, 134, 93), 5);
	bombnum->size = Vector2(12, 12);
	bombnum->setText(bombnumStr.c_str());
	frame->addUpdatable(bombnum);

	//(actual/Max)*16x
	float ammo = 16-16*veh->getActualCullDown();
	if(ammo > 16)
		ammo = 16;
	Hud * aValue = new Hud(Vector2(82, 85), Vector2(ammo, 6), 0);
	aValue->setTexture("hud_ammovalue");
	frame->addUpdatable(aValue);

	Hud * ammoheat = new Hud(Vector2(82, 85), Vector2(16, 6), 0);
	ammoheat->setTexture("hud_ammobar");
	frame->addUpdatable(ammoheat);

	float alliedscore = (World::Instance()->TotalAllies/7.0)*16;
	Hud * aScore = new Hud(Vector2(42, 2), Vector2(alliedscore, 3), 0);
	aScore->setColor(Vector3(255, 0, 0));
	aScore->setTexture("hud_healthvalue");
	frame->addUpdatable(aScore);

	float enemyscore = (World::Instance()->TotalEnemies/12.0)*16;
	Hud * eScore = new Hud(Vector2(42, 6), Vector2(enemyscore, 3), 0);
	eScore->setColor(Vector3(90, 234, 255));
	eScore->setTexture("hud_healthvalue");
	frame->addUpdatable(eScore);

	
	float hp = veh->life = veh->life;
	float healthsize = (hp / 100.0f) * 16;
	if(healthsize < 0.0)
		healthsize = 0.0;
	Vector2 sizes = Vector2((float)healthsize, 3);
	Hud * health = new Hud(Vector2(82, 92), sizes, 0);
	health->setColor(Vector3(((1 - hp) / 100)*255, ((hp) / 100)*255, 0));
	health->setTexture("hud_healthvalue");
	frame->addUpdatable(health);

	//En caso de que uno haya palmado, añade una cruz!!!
	int numAlive = World::Instance()->deaths;
	if(numAlive > 0){

		Hud * crossed1 = new Hud(Vector2(75, 2), Vector2(7, 7), 0);
		crossed1->setTexture("hud_crossed");
		frame->addHud(crossed1);
	
		if(numAlive > 1){
		Hud * crossed2 = new Hud(Vector2(81, 2), Vector2(7, 7), 0);
		crossed2->setTexture("hud_crossed");
		frame->addHud(crossed2);

		if(numAlive > 2){
		Hud * crossed3 = new Hud(Vector2(87, 2), Vector2(7, 7), 0);
		crossed3->setTexture("hud_crossed");
		frame->addHud(crossed3);

		if(numAlive > 3){
		Hud * crossed4 = new Hud(Vector2(93, 2), Vector2(7, 7), 0);
		crossed4->setTexture("hud_crossed");
		frame->addHud(crossed4);
	}
	}
	}
	}

	if(World::Instance()->enemyBombardiers == 0 && World::Instance()->enemyCommunications == 0 && World::Instance()->enemyHeadcuarters == 0)
	{
		World::Instance()->toDelete = true;
		Game::instance->handleInput(INPUT_TO_WIN);
	}
	if(World::Instance()->alliedShips == 0)
	{
		World::Instance()->toDelete = true;
		Game::instance->handleInput(INPUT_TO_LOSE);
	}

}


void PlayingState::getProjectionPos()
{
	frame->clearUpdatable();
	this->updateUpdatable();

	for(unsigned int i = 0; i < World::Instance()->models.size(); i++)
	{
		Vector2 coords;
		const char * team = "hud_target_allied";
		Entity * actualEntity = World::Instance()->models[i];
		Vector3 entityPos = actualEntity->getGlobalMatrix().getTranslationOnly();

		if(actualEntity->Type != "Vehicle")
			continue;
		
		if(actualEntity->Name == "Objective"|| actualEntity->Name ==  "Enemy")
			team = "hud_target_enemy";

		//Si hemos llegado hasta aqui es vehiculo
		Vehicle * veh = (Vehicle*)actualEntity;
		if(veh->control != NULL)
		if(veh->control->type == 0)
		{
			entityPos = entityPos + actualEntity->getGlobalMatrix().rotateVector(Vector3(0,0,1))*100;
			getPointToCamCoords(entityPos, coords);
			Hud * crosshair = new Hud(Vector2(coords.x, coords.y), Vector2(4, 5), 0);
			crosshair->setTexture("hud_crosshair");
			frame->addUpdatable(crosshair);

			continue;
		}

		if(!getPointToCamCoords(entityPos, coords))
			continue;

		Hud * enemy = new Hud(Vector2(coords.x, coords.y), Vector2(4, 5), 0);
		enemy->setTexture(team);
		frame->addUpdatable(enemy);
	
	}

}

void GameOverState::handleInput(Game* game, Input input) {

	switch(input){
		case INPUT_SPACEBAR:
			handleState(game, STATE_MAIN);
			break;
		case INPUT_BUTTON_START:
			handleState(game, STATE_MAIN);
			break;
	}
}

void GameOverState::handleState(Game* game, State state){

	switch (state){
		case STATE_MAIN:
			game->frameState = new MainMenuState();
			break;
	}

}

void GameOverState::update(Game* game, double  seconds_elapsed) {

	//frame->background->tex = ((ForwardRendering*)World::Instance()->techniques[1])->screen;
}
void GameOverState::renderFrame(Game* game) {

	this->frame->render(game->hudcamera);
}

void GameWinState::handleInput(Game* game, Input input) {

	switch(input){
		case INPUT_SPACEBAR:
			handleState(game, STATE_MAIN);
			break;
		case INPUT_BUTTON_START:
			handleState(game, STATE_MAIN);
			break;
	}
}

void GameWinState::handleState(Game* game, State state){

	switch (state){
		case STATE_MAIN:
			game->frameState = new MainMenuState();
			break;
	}

}

void GameWinState::update(Game* game, double  seconds_elapsed) {

	
}
void GameWinState::renderFrame(Game* game) {

	this->frame->render(game->hudcamera);
}


void ControlsMenuState::handleInput(Game* game, Input input)
{
	switch(input){
		case INPUT_SPACEBAR:
			handleState(game, STATE_MAIN);
			break;
		case INPUT_BUTTON_START:
			handleState(game, STATE_MAIN);
			break;
	}
}

void ControlsMenuState::handleState(Game* game, State state)
{
	switch (state){
		case STATE_MAIN:
			game->frameState = new MainMenuState();
			break;
	}
}

void ControlsMenuState::update(Game* game, double  seconds_elapsed)
{

}

void ControlsMenuState::renderFrame(Game* game)
{
	this->frame->render(game->hudcamera);
}