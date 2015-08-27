#include "ControllerManager.h"

std::multimap<unsigned int, unsigned int> ControllerManager::teamsMap = std::multimap<unsigned int, unsigned int>();
std::vector<Controller*> ControllerManager::controllers = std::vector<Controller*>();

ControllerManager::~ControllerManager()
{
	for(unsigned int i = 0; i < controllers.size(); i++)
		delete(controllers[i]);
	controllers.clear();
	teamsMap.clear();
	PlayerController::started = false;
}

void ControllerManager::setJetControl(Vehicle * _vehicle, unsigned int _team)
{
	if(_team == 1)
		_vehicle->BBcolor = Vector3(0,1,0);
	if(_team == 2)
		_vehicle->BBcolor = Vector3(1,0,0);

	JetController * jetController = new JetController();
	jetController->init(_vehicle);
	controllers.push_back(jetController);
	teamsMap.insert(std::pair<unsigned int, unsigned int>(_vehicle->id, _team));
}

void ControllerManager::setPlayerControl(Vehicle * _vehicle, unsigned int _team)
{
	if(_team == 1)
		_vehicle->BBcolor = Vector3(0,1,0);
	if(_team == 2)
		_vehicle->BBcolor = Vector3(1,0,0);

	PlayerController * playerController = new PlayerController();
	playerController->init(_vehicle);
	controllers.push_back(playerController);
	teamsMap.insert(std::pair<unsigned int, unsigned int>(_vehicle->id, _team));
}

void ControllerManager::setTurretControl(Vehicle * _vehicle, unsigned int _team)
{
	if(_team == 1)
		_vehicle->BBcolor = Vector3(0,1,0);
	if(_team == 2)
		_vehicle->BBcolor = Vector3(1,0,0);

	TurretController * turretController = new TurretController();
	turretController->init(_vehicle);
	controllers.push_back(turretController);
	teamsMap.insert(std::pair<unsigned int, unsigned int>(_vehicle->id, _team));
}

void ControllerManager::setShipController(Vehicle * _vehicle, unsigned int _team)
{
	if(_team == 1)
		_vehicle->BBcolor = Vector3(0,1,0);
	if(_team == 2)
		_vehicle->BBcolor = Vector3(1,0,0);

	ShipController * shipController = new ShipController();
	shipController->init(_vehicle);
	controllers.push_back(shipController);
	teamsMap.insert(std::pair<unsigned int, unsigned int>(_vehicle->id, _team));
}

void ControllerManager::setBombardierControl(Vehicle * _vehicle, unsigned int _team)
{
	if(_team == 1)
		_vehicle->BBcolor = Vector3(0,1,0);
	if(_team == 2)
		_vehicle->BBcolor = Vector3(1,0,0);

	BombardierController * bombardierController = new BombardierController();
	bombardierController->init(_vehicle);
	controllers.push_back(bombardierController);
	teamsMap.insert(std::pair<unsigned int, unsigned int>(_vehicle->id, _team));
}

void ControllerManager::update(double seconds_elapsed)
{	
	match();
	for(unsigned int i = 0; i < controllers.size(); i++){
			controllers[i]->update(seconds_elapsed);
	}
}

void ControllerManager::deleteController(unsigned int _vehicleID)
{

	for(unsigned int i = 0; i < controllers.size(); i++)
	{
		if(controllers[i]->enemy != NULL)
			if(controllers[i]->enemy->id == _vehicleID)
				controllers[i]->enemy = NULL;
	}

	std::map<unsigned int, unsigned int>::iterator toD;
	toD = teamsMap.find(_vehicleID);
	if(toD != teamsMap.end())
	{
		teamsMap.erase(toD);
	}

	for(unsigned int i = 0; i < controllers.size(); i++)
	{
		if(controllers[i]->toControl->id == _vehicleID)
		{
			delete(controllers[i]);
			controllers.erase(controllers.begin()+i);
			break;
		}
	}
}

void ControllerManager::match()
{
	for(int i = 0; i < controllers.size(); i ++)
	{
		if(controllers[i]->enemy == NULL)
		{
			unsigned int team = teamsMap.find(controllers[i]->toControl->id)->second;
			std::vector<unsigned int> possibleEnemies;
			for(auto it = teamsMap.begin(); it != teamsMap.end(); it++)
			{
				if(it->second != team)
				{
					if(controllers[i]->type == 3)
					{
						possibleEnemies.push_back(it->first);
					}
					else
					{
						if(checkIsPlane(it->first))
						{
							possibleEnemies.push_back(it->first);
						}

					}
				}
			}
			
			if(possibleEnemies.size() > 0)
			{
				int num = 0 + (rand() % (int)((possibleEnemies.size()-1) - 0 + 1));
				for(int y = 0; y < controllers.size(); y++)
				{
					if(controllers[y]->toControl->id == possibleEnemies[num])
					{
						controllers[i]->enemy = controllers[y]->toControl;
					}
				}
			}
		}
	}
}

void ControllerManager::swapVehicleController(Vehicle * _PlayerController, Vehicle * _JetController)
{

	
	std::cout << "Doing swap." << std::endl;
	unsigned int playerTeam, iaTeam;
	std::map<unsigned int, unsigned int>::iterator team;
	team = teamsMap.find(_PlayerController->id);
	playerTeam = team->second;
	team = teamsMap.find(_JetController->id);
	iaTeam= team->second;

	deleteController(_PlayerController->id);
	deleteController(_JetController->id);

	if(playerTeam != iaTeam)
		std::cout << "Error, swap entre diferentes equipos." << std::endl;

	setPlayerControl(_JetController, iaTeam);
	setJetControl(_PlayerController, playerTeam);
}

bool ControllerManager::getNextPlane(Vehicle * _vehicle)
{
	unsigned int team = teamsMap.find(_vehicle->id)->second;
	unsigned int newPlayer = 0;

	//Escoje solo de tu euipo jugadores con id mas grande
	for(auto it = teamsMap.begin(); it != teamsMap.end(); it++)
	{
		if(it->second != team || it->first <= _vehicle->id || !checkPlaneLife(it->first) || !checkIsPlane(it->first) )
			continue;
		newPlayer = it->first;
		break;
	}

	//Si no hemos encontrado, damos la vuelta al mapeado
	if(newPlayer == 0)
	{
		for(auto it = teamsMap.begin(); it != teamsMap.end(); it++)
		{
			//segunda comprobacion para no hacer swap con la ia si solo tenemos un avion restante en equipo
			if(it->second != team || it->first == _vehicle->id || !checkPlaneLife(it->first) || !checkIsPlane(it->first))
				continue;
			newPlayer = it->first;
			break;
		}
	}

	//hacemos el swap con el controlador adecuado
	for(unsigned int i = 0; i < controllers.size(); i++)
	{
		if(controllers[i]->toControl->id == newPlayer && controllers[i]->toControl->toDelete != true)
		{
			if(controllers[i]->type != 1)
				continue;

			swapVehicleController(_vehicle, controllers[i]->toControl);
			SoundManager::Instance()->playSound(SWAPCAM);

			return true;
		}
	}

	return false;
}

void ControllerManager::debug()
{
	//Por si queremos debugar algo por frame
}

bool ControllerManager::checkPlaneLife(unsigned int _id)
{
	for(unsigned int i = 0; i < controllers.size(); i++)
	{
		if(controllers[i]->toControl->id == _id)
			return controllers[i]->toControl->living;
	}
	return false;
}

bool ControllerManager::checkIsPlane(unsigned int _id)
{
	for(unsigned int i = 0; i < controllers.size(); i++)
	{
		if(controllers[i]->toControl->id == _id)
			return controllers[i]->type == 1 || controllers[i]->type == 0;
	}
	return false;
}

Vehicle * ControllerManager::getPlayer()
{
	for(unsigned int i = 0; i < controllers.size(); i++)
	{
		if(controllers[i]->type == 0)
			return controllers[i]->toControl;
	}
	return NULL;
}

bool ControllerManager::checkIsPlayer(unsigned int _id)
{
	for(unsigned int i = 0; i < controllers.size(); i++)
	{
		if(controllers[i]->toControl->id == _id && controllers[i]->type == 0)
		{
			return true;
		}
	}
	return false;
}