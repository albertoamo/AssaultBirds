#ifndef RENDERER_H
#define RENDERER_H

#include "includes.h"
#include "init/loader.h"
#include "shader.h"
#include "framework.h"
#include "Entity.h"
#include "EntityMesh.h"
#include "rendertotexture.h"

class Billboard;
class DebugParticle;
class BulletManager;
class World;
class ShadowMapping;
class Reflection;
class ScriptPoint;
class ForwardRendering;
class MinimapRendering;

struct RenderStack{

	Camera * camera;
	Shader * shader;
	std::vector< EntityLight* > lights;
	std::vector<std::pair<std::string, Texture*>> maps;
	std::vector<std::pair<std::string, float>> floats;
	std::vector<std::pair<std::string, int>> integers;
	std::vector<std::pair<std::string, Matrix44>> matrices;

	~RenderStack(){
		delete(shader);
	}

};


//This class based in the visitor pattern will implement each different rendering technique
class Renderer {
public:

	Renderer();
	~Renderer();
	void render(const World & obj);
	void render(const Billboard & obj);
	void render(const BulletManager & obj);
	void render(const DebugParticle & obj);
	void render(const ShadowMapping & obj);
	void render(const Reflection & obj);
	void render(const ScriptPoint & obj);
	void render(const ForwardRendering & obj);
	void render(const MinimapRendering & obj);
};

class DebugParticle{
public:
	int primitive;

	Mesh * mesh;
	Shader * shader;
	Matrix44 model;

	DebugParticle(){
		this->mesh = new Mesh();
		model.setIdentity();
		shader = NULL;
	}

	~DebugParticle(){
		delete(mesh);
		if (shader != NULL)
			delete(shader);
	}

	void loadShader(const char* filename1, const char* filename2){

		shader = new Shader();
		std::string dr1 = "assets/materials/shaders/" + std::string(filename1);
		std::string dr2 = "assets/materials/shaders/" + std::string(filename2);

		shader = shader->Load(dr1.c_str(), dr2.c_str());

		if (shader == NULL)
			std::cout << "shader not found or error" << std::endl;
	}


	void addParticle(Vector3 & position){
		mesh->vertices.push_back(position);
	}

	void render(Renderer * r){ r->render(*this); }
};

#endif