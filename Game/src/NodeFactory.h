#ifndef NODEFACTORY_H
#define NODEFACTORY_H
#include "framework.h"
#include "Node.h"

//It's not a pure factory method, but it was good to have it here.
class NodeFactory
{
public:
	static ScriptPoint * Spawn(const char* type)
	{

		if (type == "script_origin"){
			ScriptPoint * origin = new ScriptPoint("script_origin", Vector3(255, 0, 0));
			origin->setSize(1);
			return origin;
		}
		else if(type == "script_particle"){
			ScriptPoint * origin = new ScriptPoint("script_particle", Vector3(0, 255, 0));
			origin->setSize(1);
			return origin;
		}
		else if (type == "spawn_allies"){
			ScriptPoint * origin = new ScriptPoint("spawn_allies", Vector3(0, 0, 255));
			origin->setSize(4);
			return origin;

		}
		else if(type == "spawn_axis"){
			ScriptPoint * origin = new ScriptPoint("spawn_axis", Vector3(255, 0, 0));
			origin->setSize(4);
			return origin;

		}
		else if(type == "spawn_inter"){
			ScriptPoint * origin = new ScriptPoint("spawn_inter", Vector3(220, 20, 60));
			origin->setSize(2);
			return origin;

		}
	}


};

#endif

