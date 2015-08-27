#include "Technique.h"
#include "renderer.h"
#include "world.h"
#include "game.h"
#include <tuple>      

Technique::Technique(){}
Technique::~Technique(){

	delete(this->stack);
}
bool Technique::loadShader(int & prim, const char * dir){
	
	this->stack->shader = new Shader();
	this->primitive = prim;
	std::string dr1 = "assets/materials/shaders/" + std::string(dir) + ".vs";
	std::string dr2 = "assets/materials/shaders/" + std::string(dir) + ".ps";

	if (!this->stack->shader->load(dr1, dr2)){
		std::cout << "Shader ERROR: [ " << dir << " ]" << std::endl;
		return false;
	}
	
	return true;
}

void Technique::render(Renderer * r){
	//r->render(*this);
}
void Technique::update(Camera * camera){
	this->stack->camera = camera;
}
void Technique::init(Camera * camera, EntityMesh* caster){
	this->stack->camera = camera;
}

void Technique::setResolution(Camera * camera) const{
	glViewport(0, 0, RESX, RESY);
	camera->aspect = RESX / (float)RESY;
	camera->setPerspective(camera->fov, RESX / (float)RESY, camera->near_plane, 100000);
}

void Technique::downResolution(Camera  * camera) const{
	glViewport(0, 0, Game::instance->window_width, Game::instance->window_height);
	camera->aspect = Game::instance->window_width / (float)Game::instance->window_height;
	camera->setPerspective(camera->fov, Game::instance->window_width / (float)Game::instance->window_height, camera->near_plane, 100000);
}

void Technique::update(Game & game){

	state = Game::instance->getState();
}

Vector2 Technique::getState() {

	return state;
}


ShadowMapping::ShadowMapping(int prim, const char * dir){

	this->stack = new RenderStack();
	this->loadShader(prim, dir);
	shadowmap = new RenderToTexture();
	Vector2 res = getDesktopSize(0);
	shadowmap->createShadowMap(RESX, RESY);
	shadowmap->disable();

	//w_listener = new ObserverWindow();
	Game::instance->attachObserver(this);
}

void addStack(EntityMesh * ent, std::pair<std::string, Texture*> repeat){

	for (auto it = ent->children.begin(); it != ent->children.end(); ++it){
		addStack(((EntityMesh*)(*it)), repeat);
	}

	ent->material->maps.push_back(repeat);

}

void addStack(EntityMesh * ent, std::pair<std::string, Matrix44*> repeat){

	for (auto it = ent->children.begin(); it != ent->children.end(); ++it){
		addStack(((EntityMesh*)(*it)), repeat);
	}

	ent->material->matrices.push_back(repeat);

}


void ShadowMapping::init(std::vector<EntityMesh*> inner, std::vector<EntityLight*> lights){

	std::vector<EntityMesh*> caster;
	for (unsigned int x = 0; x < inner.size(); x++){
		if (inner[x]->material->materialType != "Sky")
			caster.push_back(inner[x]);
	}

	this->stack->camera = new Camera();

	this->stack->camera->lookAt(Vector3(1729, 0, -3747), Vector3(1766, -4, -3746), Vector3(0, 1, 0));
	this->stack->camera->setOrthographic(10, -10, -10, 10, -100, 100);

	Matrix44 * texGen = new Matrix44();
	texGen = &(this->stack->camera->viewprojection_matrix);

	for (unsigned int x = 0; x < caster.size(); x++){
		addStack(caster[x], std::make_pair("shadowMat", texGen));
		addStack(caster[x], std::make_pair("shadow_map", shadowmap));
	}

	this->stack->lights = lights;
	this->caster = caster;

}

void ShadowMapping::render(Renderer * r){
	r->render(*this);
}
void ShadowMapping::update(Camera * camera){
	World::Instance()->debugParticle->addParticle(this->stack->camera->eye);
	this->stack->camera->lookAt(this->stack->camera->eye, Vector3(1766, -4, -3746), Vector3(0, 1, 0));
}

void ShadowMapping::update(Game & game){

	
	if (this->state.x == 0 && this->state.y == 0)//to avoid debug purposes...
		state = Game::instance->getState();
	else if (this->shadowmap != NULL){
		/*state = Game::instance->getState();
		glDeleteFramebuffers(1, &shadowmap->fbo);
		glDeleteTextures(1, &shadowmap->texture_id);
		this->shadowmap->createShadowMap(state.x, state.y);
		this->shadowmap->disable();*/
	}
}


Reflection::Reflection(int prim, const char * dir){

	this->stack = new RenderStack();
	this->loadShader(prim, dir);

	this->reflection = NULL;
	Game::instance->attachObserver(this);

	reflection = new RenderToTexture();
	//Vector2 size = ((Technique*)this)->getState();
	reflection->createMirror(RESX, RESY);
	reflection->disable();

	this->stack->camera = new Camera();
	this->stack->camera->setPerspective(Game::instance->camera->fov + 1, RESX / (float)RESY, 1, 100000);
	this->stack->camera->aspect = RESX / (float)RESY;
}

Reflection::~Reflection(){

	delete(reflection);
	delete(stack);

}

void Reflection::init(Camera * camera, EntityMesh* caster){

	this->emitter = caster;

	//We change the shader, in this case, we recompile it again and add a preprocessor conditional
	emitter->material->loadShader("phongTan.vs", "phongTan.ps", "#define REFLECTION\n#define MAPMOVE\n");
	this->update(camera);

	Matrix44 * texGen;
	texGen = &(this->stack->camera->viewprojection_matrix);
	this->stack->lights = World::Instance()->lights;
	this->stack->floats.push_back(std::make_pair("ref_height", this->emitter->origin.y - 0.1));
	emitter->material->maps.push_back(std::make_pair("color_reflection", reflection));
	emitter->material->matrices.push_back(std::make_pair("texGen", texGen));

}

void Reflection::render(Renderer * r){
	r->render(*this);
}
void Reflection::update(Camera * camera){

	this->stack->camera->eye = camera->eye;
	this->stack->camera->center = camera->center;
	this->stack->camera->up = camera->up;
	float distEye = camera->eye.y - this->emitter->origin.y;
	float distCent = camera->center.y - this->emitter->origin.y;

	this->stack->camera->up.y = -this->stack->camera->up.y;

	this->stack->camera->eye.y = this->stack->camera->eye.y - (2 * distEye);
	this->stack->camera->center.y = this->stack->camera->center.y - (2 * distCent);
	this->stack->camera->lookAt(this->stack->camera->eye, this->stack->camera->center, this->stack->camera->up);


}

void Reflection::update(Game & game){

	/*
	if (this->state.x == 0 && this->state.y == 0)//to avoid debug purposes...
		state = Game::instance->getState();
	else if (this->reflection != NULL){
		state = Game::instance->getState();
		glDeleteFramebuffers(1, &reflection->fbo);
		glDeleteTextures(1, &reflection->texture_id);
		this->reflection->createMirror(state.x, state.y);
		this->reflection->disable();
		this->stack->camera->
(Game::instance->camera->fov + 5, state.x / (float)state.y, 1, 100000);
	}*/
	
}


ForwardRendering::ForwardRendering(int prim, const char * dir){

	this->stack = new RenderStack();
	this->loadShader(prim, dir);

	this->screen = NULL;
	screen = new RenderToTexture();
	Vector2 res = getDesktopSize(0);
	screen->createMirrorHDR(RESX, RESY);
	screen->disable();

}

ForwardRendering::~ForwardRendering(){

	delete(screen);

}

void ForwardRendering::init(World * world){
		
	this->world = world;

}

void ForwardRendering::render(Renderer * r){
	r->render(*this);
}

void ForwardRendering::update(Camera * camera){

}

void ForwardRendering::update(Game & game){

	state = Game::instance->getState();

}

MinimapRendering::MinimapRendering(int prim, const char * dir){

	this->stack = new RenderStack();
	Vehicle * veh = World::Instance()->controllersManager.getPlayer();
	this->stack->camera = new Camera();
	//this->stack->camera->setPerspective(70, RESX / (float)RESY, 1, 100000);
	this->stack->camera->lookAt(Vector3(100, 1000, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	this->stack->camera->setOrthographic(-1000, 1000, -1000, 1000, -10000, 10000);
	this->stack->lights = World::Instance()->lights;
	this->loadShader(prim, dir);
	iconshader = NULL;
	std::string d_shader = "assets/materials/shaders/" + std::string("simple");

	iconshader = iconshader->Load(std::string(d_shader + ".vs").c_str(), std::string(d_shader + ".ps").c_str());

	this->minimap = NULL;
	minimap = new RenderToTexture();
	minimap->createMirror(RESX, RESY);
	minimap->disable();

}

MinimapRendering::~MinimapRendering(){

	delete(minimap);

}

void MinimapRendering::init(World * world){

	this->world = world;

}

void MinimapRendering::render(Renderer * r){
	r->render(*this);
}

void MinimapRendering::update(Camera * camera){
	Vehicle * veh = World::Instance()->controllersManager.getPlayer();
	Vector3 camPos = veh->Model.getTranslationOnly() + veh->Model.rotateVector(Vector3(0, 1, 0)) * 500;
	Vector3 camPoz = veh->Model.getTranslationOnly();
	//this->stack->camera->lookAt(camPos + Vector3(0,0,0.1), veh->Model.getTranslationOnly() + Vector3(0,0,2), this->stack->camera->up);
	this->stack->camera->lookAt(Vector3(camPoz.x, camPoz.y + 15, camPoz.z), veh->Model.getTranslationOnly() + Vector3(0, 0, 10), this->stack->camera->up);
}

void MinimapRendering::update(Game & game){

	state = Game::instance->getState();

}

void MinimapRendering::renderPops(Mesh * icono, Texture * tex, Matrix44 model) const{

	Matrix44 viewProjection = stack->camera->viewprojection_matrix;
	Matrix44 mvp = model * viewProjection;
	Matrix44 modelT = model.getRotationOnly();

	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	iconshader->enable();

	//Matrices
	iconshader->setMatrix44("model_matrix", model);
	iconshader->setMatrix44("normal_matrix", modelT);
	iconshader->setMatrix44("mvp", mvp);

	//Texturas
	iconshader->setTexture("color_texture", tex->texture_id);

	//Propiedades del material
	iconshader->setFloat("alpha", 1.0);
	iconshader->setFloat("time", SDL_GetTicks() * 20);

	icono->render(GL_TRIANGLES, iconshader, true);

	iconshader->disable();
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
}

void MinimapRendering::generateIcon(Vector3 & pos, const char * icon) const{

	std::string d_tex = "assets/images/hud/" + std::string(icon) + ".TGA";

	Texture *tex = NULL;
	tex = tex->Load(d_tex.c_str());

	Mesh icono;
	icono.createPlane(150);

	Matrix44 model = Matrix44();
	model.setTranslation(pos.x, pos.y + 100, pos.z);

	renderPops(&icono, tex, model);

}

void MinimapRendering::renderIcons() const{


	for (unsigned int i = 0; i < World::Instance()->models.size(); i++)
	{
		Vector2 coords;
		Entity * actualEntity = World::Instance()->models[i];
		Vector3 entityPos = actualEntity->getGlobalMatrix().getTranslationOnly();

		std::string name = std::string(actualEntity->Name);
		if (name == std::string("ax_base")){ generateIcon(entityPos, "hud_obj"); continue; }
		if (name == std::string("ax_antenna")){ generateIcon(entityPos, "hud_obj_antenna"); continue; }
		if (name == std::string("ax_radar")){ generateIcon(entityPos, "hud_obj_radar"); continue; }
		if (name == std::string("Objective")){ generateIcon(entityPos, "hud_com_bombardier"); continue; }

		Vehicle * veh = (Vehicle*)actualEntity;
		if (veh != NULL){

			if (veh->control != NULL){
				//if (veh->control->type == 0){ generateIcon(entityPos, "hud_com_me"); continue; }
			}


			switch (veh->subType){
			case PLANE: {
				if (name == std::string("Allied")){ generateIcon(entityPos, "hud_allied_fire"); continue; }
				else { generateIcon(entityPos, "hud_enemy_fire"); continue; }
			}break;
			case TURRET:generateIcon(entityPos, "hud_com_turret"); continue; break;
			case SHIP: generateIcon(entityPos, "hud_com_def"); continue; break;
			case ROCKET: generateIcon(entityPos, "hud_com_me"); continue; break;
			case BOMB: generateIcon(entityPos, "hud_com_me"); continue; break;
			}
		}


	}



}