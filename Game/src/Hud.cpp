#include "Hud.h"
#include "game.h"
//#include "Observer.h"

Hud::Hud(){

}

Hud::~Hud(){

	Hud * aux = NULL;
	for( auto it = Game::instance->observers.begin(); it != Game::instance->observers.end(); it++ ){
		if((*it) == this)
			aux = this;
	}

	if(aux != NULL){
		Game::instance->observers.erase(aux);
	}

	deleteChildren();

	delete(mesh);
	//delete(shader);
	//delete(tex );

}

Hud::Hud(int x, int y, Vector2 size, int index){

	this->fading.fade = false;
	this->z_index = index;
	this->mesh = new Mesh();
	this->origin = Vector2(x,y);
	this->size = size;

	Game::instance->attachObserver(this);
}

Hud::Hud(Vector2 origin, Vector2 size, int index) : parent(NULL){

	this->model = this->model.setIdentity();
	//this->model.setTranslation(origin.x, origin.y, 0);
	this->color = Vector3(1.0, 1.0, 1.0);
	this->fading.fade = false;
	this->z_index = index;
	this->mesh = new Mesh();
	this->origin = origin;
	this->size = size;
	this->alpha = 1;
	this->shader = NULL;
	this->tex = NULL;

	Game::instance->attachObserver(this);
	createHud();
}
Hud::Hud(int x, int y, Vector2 size, std::string type){
	this->fading.fade = false;
	Game::instance->attachObserver(this);
}
Hud::Hud(Vector2 origin, Vector2 size, std::string type){
	this->fading.fade = false;
	Game::instance->attachObserver(this);
}

void Hud::createHud(){

	float posx = (state.x / 100) * this->origin.x;
	float posy = (state.y / 100) * this->origin.y;

	float sizex = (state.x / 100) * this->size.x;
	float sizey = (state.y / 100) * this->size.y;

	//this->size = Vector2(sizex, sizey);
	this->mesh->createQuad(posx, posy, sizex, sizey, true);
	//this->mesh->createQuad(Vector2(posx, posy), Vector2(sizex, sizey), Vector2(0,0), Vector2(1,1), true);
}

void setLabel(std::string & label, const char* font, float size){


}

void addBorder(float size){

}

void Hud::addChildren(Hud * hud){

	hud->parent = this;
	this->children.push_back(hud);
}

void Hud::deleteChildren(){

	for (auto it = children.begin(); it != children.end(); it++){
		if ((*it)->children.size()) (*it)->deleteChildren();
		else delete(*it);
	}
	children.clear();
}

Matrix44 Hud::getGlobalMatrix(){

	if (parent)
		return model * parent->getGlobalMatrix();

	return model;
}


void Hud::fadeOverTime(){

	float currentTime = SDL_GetTicks();
	int timeInterval = currentTime - fading.previousTime;
	float value = ((float)timeInterval / fading.time) * 2.0 - 1;

	if (timeInterval > fading.time){
		fading.previousTime = currentTime;
	}
	this->alpha = sin(fabs(value));
}

void Hud::setFade(float time){

	if (this->fading.fade == false) fading.fade = true;
	else fading.fade = false;

	fading.time = time;

}

bool Hud::setTexture(const char* texture, const char * shaName){

	std::string d_shader = "assets/materials/shaders/" + std::string(shaName);
	std::string d_tex = "assets/images/hud/" + std::string(texture) + ".TGA";

	this->shader = this->shader->Load(std::string(d_shader + ".vs").c_str(), std::string(d_shader + ".ps").c_str());

	this->tex = tex->Load(d_tex.c_str());
	if (tex == NULL){
		std::cout << "An error has ocurred loading " << d_tex << ". " << std::endl;
		return false;
	}

	return true;
}

bool Hud::setShader(const char * shaderName){

	std::string d_shader = "assets/materials/shaders/" + std::string(shaderName);

	this->shader = new Shader();
	this->shader->load(d_shader + ".vs", d_shader + ".ps");

	if (shader == NULL){
		std::cout << "An error has ocurred loading: " << shaderName << ". " << std::endl;
		return false;
	}

	return true;
}

void Hud::setFade(Vector3 color, float time){

	if (this->fading.fade == false) fading.fade = true;
	else fading.fade = false;

	fading.time = time;

}

void Hud::update(Game & game){

	state = Game::instance->getState();
	delete(mesh);
	this->mesh = new Mesh();
	createHud();
}

void Hud::setDepth(int depth){
	this->z_index = depth;
}

void Hud::setColor(Vector3 color){
	this->color = Vector3(color.x / 255, color.y / 255, color.z / 255);
}

void Hud::translate(Vector3 Traslator){
	this->model.traslateLocal(Traslator.x, Traslator.y, Traslator.z);
	//Vector3 translated = model.getTranslationOnly();
	//this->origin = Vector2(translated.x, translated.y);
}

void Hud::render(int primitive, Camera * camera){

	if (fading.fade) fadeOverTime();

	for (auto it = children.begin(); it != children.end(); ++it){
		(*it)->render(GL_TRIANGLES, camera);
	}

	Matrix44 modelmat = getGlobalMatrix();
	Matrix44 viewProjection = camera->viewprojection_matrix;
	Matrix44 mvp = modelmat * viewProjection;
	Matrix44 modelT = modelmat.getRotationOnly();

	shader->enable();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);

	//Matrices
	shader->setMatrix44("model_matrix", modelmat);
	shader->setMatrix44("normal_matrix", modelT);
	shader->setMatrix44("mvp", mvp);

	//Texturas
	shader->setTexture("color_texture", tex->texture_id);

	//Propiedades del material
	shader->setFloat("alpha", alpha);
	shader->setFloat("time", SDL_GetTicks() * 20);
	shader->setVector3("text_color", this->color);

	this->mesh->render(primitive, shader, true);

	shader->disable();
	glEnable(GL_CULL_FACE);
	//glDisable(GL_BLEND);


}