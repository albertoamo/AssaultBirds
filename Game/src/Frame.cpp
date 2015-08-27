#include "Frame.h"
#include "world.h"

#include <algorithm>
#include <string>
#include <iostream>       // std::cout
#include <cstddef>  

Button::Button(Vector2 position, Vector2 size, const char * text){
	this->origin = position;
	this->size = size;
	Hud * button = new Hud(this->origin, this->size, 22);
	button->setTexture("menu_button");
	this->back = button;

	Label * label = new Label(this->origin + Vector2(size.x/6, 1), Vector3(0.0, 0.0, 0.0), size.x);
	label->setText(text);
	//label->addGlow(Vector3(0.0, 0.0, 0.0));
	this->title = label;

	this->origin = this->origin + Vector2(10, 0);
	this->mouseOver = false;

	Game::instance->attachObserver(this);

}

std::string Button::checkSelect(Vector2 mousepos){

	float orgx = (this->back->state.x / 100) * this->back->origin.x;
	float orgy = (this->back->state.y / 100) * this->back->origin.y;

	float sizex = orgx + (this->back->state.x / 100) * this->back->size.x;
	float sizey = orgy + (this->back->state.y / 100) * this->back->size.y;

	if (mousepos.x > orgx && mousepos.x < sizex && mousepos.y > orgy && mousepos.y < sizey){
		return this->title->word;
	}
	else{
		return "";
	}
}

std::string Button::checkSelect()
{
	if(this->hovered)
		return this->title->word;
	return "";
}

//Worst method ever, but seems to work.... running out of time
void Button::checkHover(Vector2 mousepos){

	//std::cout << "Button chcked: " << this->title->word << std::endl;
	float orgx = (this->back->state.x / 100) * this->back->origin.x;
	float orgy = (this->back->state.y / 100) * this->back->origin.y;

	float sizex = orgx + (this->back->state.x / 100) * this->back->size.x;
	float sizey = orgy + (this->back->state.y / 100) * this->back->size.y;

	if (mousepos.x > orgx && mousepos.x < sizex && mousepos.y > orgy && mousepos.y < sizey){
		this->title->color = Vector3(182.0/255, 187.0/255, 34.0/255);
		//Vector2 aux = this->back->origin;
		this->back->model.setTranslation(40,0,0);
		this->title->model.setTranslation(40, 0, 0);
	}
	else{
		this->title->color = Vector3(0.0, 0.0, 0.0);
		this->back->model.setTranslation(0, 0, 0);
		this->title->model.setTranslation(0, 0, 0);
	}
}

bool Button::checkMouseHovered(Vector2 mousepos)
{
	//std::cout << "Button chcked: " << this->title->word << std::endl;
	float orgx = (this->back->state.x / 100) * this->back->origin.x;
	float orgy = (this->back->state.y / 100) * this->back->origin.y;

	float sizex = orgx + (this->back->state.x / 100) * this->back->size.x;
	float sizey = orgy + (this->back->state.y / 100) * this->back->size.y;

	if (mousepos.x > orgx && mousepos.x < sizex && mousepos.y > orgy && mousepos.y < sizey)
		return true;
	return false;

}

void Button::setHover()
{
	hovered = true;
	this->title->color = Vector3(182.0/255, 187.0/255, 34.0/255);
	this->back->model.setTranslation(40,0,0);
	this->title->model.setTranslation(40, 0, 0);
}

void Button::unHover()
{
	hovered = false;
	this->title->color = Vector3(0.0, 0.0, 0.0);
	this->back->model.setTranslation(0, 0, 0);
	this->title->model.setTranslation(0, 0, 0);
}

void Button::render(int primitive, Camera * camera){

	back->render(primitive, camera);
	title->render(primitive, camera);

}

void Button::update(Game & game) {

	Vector2 oldState = state;
	float statex = (oldState.x / 100) * this->size.x;
	state = Game::instance->getState();
	//this->size = Vector2(this->size.x + (additer.x / 100), this->size.y + (additer.y / 100));

	float sizex = (state.x / 100) * this->size.x;
	float sizey = (state.y / 100) * this->size.y;

	std::cout << statex << "  " << sizex << std::endl;
	//this->title->origin = this->back->origin + Vector2(size.x / 3, 1);
	//this->title->size.x = sizex;
	this->title->setText(this->title->word.c_str());
}


ProgressBar::ProgressBar(Vector2 position, Vector2 size, Vector3 color){

	this->origin = position;
	this->size = size;
	Hud * backbar = new Hud(this->origin, this->size, 50);
	backbar->setTexture("hud_loadbar");
	this->back = backbar;

	Hud * bar = new Hud(this->origin, Vector2(0,this->size.y), 52);
	bar->setTexture("hud_loadbar_prog");
	this->bar = bar;

}
void ProgressBar::render(int primitive, Camera * camera){

	back->render(primitive, camera);
	bar->render(primitive, camera);

}
void ProgressBar::update(Game & game){
	Vector2 oldState = state;
	state = Game::instance->getState();
}

void ProgressBar::updateSize(float size){

	delete(bar);
	float amount = this->size.x / 100 * size;
	std::cout << amount << std::endl;
	Hud * bar = new Hud(this->origin, Vector2(amount, this->size.y), 52);
	bar->setTexture("hud_loadbar_prog");
	this->bar = bar;

	this->render(GL_TRIANGLES, Game::instance->camera);
}


Frame::Frame(const char* name){

	this->name = name;
}

void Frame::addBackground(Hud * hud){

	this->background = hud;
	hud->z_index = 0;
}

//Here the huds should be instanced
void Frame::addHud(Hud * hud){

	this->huds.push_back(hud);
}

void Frame::addUpdatable(Hud * hud){

	this->updatable.push_back(hud);
}

void Frame::addContainer(Container * container){

	this->containers.push_back(container);
}

void Frame::render(Camera * camera){

	glDisable(GL_DEPTH_TEST);

	background->render(GL_TRIANGLES, camera);

	for(std::vector<Hud*>::iterator it = huds.begin(); it != huds.end(); ++it)
			(*it)->render(GL_TRIANGLES, camera);
	
	for (std::vector<Container*>::iterator it = containers.begin(); it != containers.end(); ++it)
		(*it)->render(GL_TRIANGLES, camera);

	for (std::vector<Hud*>::iterator it = updatable.begin(); it != updatable.end(); ++it)
		(*it)->render(GL_TRIANGLES, camera);

	glEnable(GL_DEPTH_TEST);
}


State Frame::checkContainers(Vector2 mousepos){

	for (unsigned int x = 0; x < containers.size(); x++){
		std::string type = containers[x]->checkSelect(mousepos);

		if (type != ""){
			State stated = mapTypes(type.c_str());
			return stated;
		}
	}
}

State Frame::checkContainers(){
	for (unsigned int x = 0; x < containers.size(); x++){
		std::string type = containers[x]->checkSelect();

		if (type != ""){
			State stated = mapTypes(type.c_str());
			return stated;
		}
	}
}

State Frame::mapTypes(const char * type){
	
	State stated = STATE_DEFAULT;
	//std::cout << type << std::endl;
	if (strcmp(type, "PLAY") == 0){
		stated = STATE_PLAY;
	
	}
	else if (strcmp(type, "EXIT") == 0){ stated = STATE_EXIT; }

	return stated;
}

void Frame::fadeDie(float time){

	if (time > 50){
		background->alpha = background->alpha - 0.01;

		for (std::vector<Hud*>::iterator it = huds.begin(); it != huds.end(); ++it){
			for (auto ita = (*it)->children.begin(); ita != (*it)->children.end(); ita++){
				if ((*ita)->alpha > 0.0) (*ita)->alpha = (*ita)->alpha - 0.01;
			}
			if ((*it)->alpha > 0.0) (*it)->alpha = (*it)->alpha - 0.01;
		}
	}
}

void Frame::clearHuds(){
	for(unsigned int x = 0; x < huds.size(); x++){
		delete(huds[x]);
		
	}
	huds.clear();
}

void Frame::clearUpdatable(){
	for (unsigned int x = 0; x < updatable.size(); x++){
		delete(updatable[x]);

	}
	updatable.clear();
}