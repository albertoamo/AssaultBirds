#include "Label.h"
#include <algorithm>
#include "game.h"
#include "SoundManager.h"

Label::Label(){

}

Label::~Label(){
	Hud * aux = NULL;
	for (auto it = Game::instance->observers.begin(); it != Game::instance->observers.end(); it++){
		if (((Hud*)(*it)) == this)
			aux = ((Hud*)this);
	}

	if (aux != NULL){
		Game::instance->observers.erase(aux);
	}

	deleteChildren();

	delete(mesh);
}

Label::Label(Vector2 position, Vector3 color, int size): addit(0.0)  {

	this->origin = position;
	this->size = Vector2(size,size);
	this->model = this->model.setIdentity();
	//this->model.setTranslation(origin.x, origin.y, 0);
	this->shader = NULL;
	this->tex = NULL;
	this->color = Vector3(color.x / 255, color.y / 255, color.z / 255) * 2.0;
	this->loadFont("font_arial", "graphic");
	this->mesh = new Mesh();
	this->alpha = 1;

	this->fading.fade = false;
	this->word = "";
	this->z_index = 100;
	this->parent = NULL;


	Game::instance->attachObserver(this);

	roll.active = false;
	roll.buildWord = "";
	roll.index = 0;

	//blendtype = GL_ONE;
}

void Label::setFamilyFont(const char* font, float fontSize){

	this->font = std::string(font);
	this->fontSize = fontSize;
}

void Label::setText(const char* text){

	delete(mesh);
	this->mesh = new Mesh();

	float size = strlen(text);
	std::string word = std::string(text);
	this->word = word;

	float ratio = (this->size.x / 100) * 30;

	float posx = ((state.x / 100) * this->origin.x) + addit;
	float posy = (state.y / 100) * this->origin.y;

	for (unsigned int x = 0; x < word.size(); x++){
		Sprite2D sprite = mapper(word[x], x);
		Vector2 newpos = Vector2(posx + (this->size.x * (x + 1)) - (sprite.ratio * x), posy);
		this->mesh->createQuad(newpos , this->size, sprite.start, sprite.end, true);

	}
}

void Label::insertOverTime(float time){

	for (auto it = children.begin(); it != children.end(); ++it){
		//((Label*)(*it))->roll.totalWord = ((Label*)parent)->roll.totalWord;
		((Label*)(*it))->roll.active = true;
		((Label*)(*it))->insertOverTime(time);
	}

	float currentTime = SDL_GetTicks();
	int timeInterval = currentTime - roll.previousTime;
	float value = ((float)timeInterval / time) * 2.0 - 1;

	if (timeInterval > time){

		roll.previousTime = currentTime;

		if (roll.index == this->roll.totalWord.size()){

			for (auto it = children.begin(); it != children.end(); ++it){
				((Label*)(*it))->roll.active = false;
				((Label*)(*it))->roll.buildWord = "";
				((Label*)(*it))->roll.index = 0;
			}

			roll.active = false;
			roll.buildWord = "";
			roll.index = 0;

			return;
		}

		roll.buildWord = roll.buildWord + this->roll.totalWord[roll.index];
		SoundManager::Instance()->playSound(LETTERMOVE);
		setText(roll.buildWord.c_str());

		roll.index++;

	}
	//this->alpha = sin(fabs(value));
}

bool Label::loadFont(const char* fontfile, const char* shader){

	std::string d_shader = "assets/materials/shaders/" + std::string(shader);
	std::string d_tex = "assets/images/fonts/" + std::string(fontfile) + ".TGA";

	this->shader = this->shader->Load(std::string(d_shader + ".vs").c_str(), std::string(d_shader + ".ps").c_str());

	this->tex = tex->Load(d_tex.c_str());
	if (tex == NULL){
		std::cout << "An error has ocurred loading " << d_tex << ". " << std::endl;
		return false;
	}

	return true;
}

void Label::addGlow(Vector3 color){
	Label * glow = new Label(this->origin, color, this->size.x);
	glow->setText(this->word.c_str());
	glow->loadFont("font_arial_sharp", "graphic");
	addChildren(glow);
}

//When I die, I will be sure that all of this will be written in my tombstone...
Sprite2D Label::mapper(const char letter, const int index){

	Sprite2D sprite;

	switch (letter){

		case '!': sprite = getSprite(1, 13, 30); break;
		case '¿': sprite = getSprite(2, 13, 30); break;
		case '#': sprite = getSprite(3, 13, 30); break;
		case '$': sprite = getSprite(4, 13, 30); break;
		case '-': sprite = getSprite(13, 13, 30); break;
		case ':': sprite = getSprite(10, 12, 30); break;

		case ',': sprite = getSprite(12, 13, 30); break;
		case '.': sprite = getSprite(14, 13, 30); break;
		case '/': sprite = getSprite(15, 13, 30); break;
		case '0': sprite = getSprite(0, 12, 30); break;
		case '1': sprite = getSprite(1, 12, 30); break;
		case '2': sprite = getSprite(2, 12, 30); break;
		case '3': sprite = getSprite(3, 12, 30); break;
		case '4': sprite = getSprite(4, 12, 30); break;
		case '5': sprite = getSprite(5, 12, 30); break;
		case '6': sprite = getSprite(6, 12, 30); break;
		case '7': sprite = getSprite(7, 12, 30); break;
		case '8': sprite = getSprite(8, 12, 30); break;
		case '9': sprite = getSprite(9, 12, 30); break;

		case ' ': sprite = getSprite(0, 15, 30); break;
		case 'A': sprite = getSprite(1, 11, 30); break;
		case 'B': sprite = getSprite(2, 11, 30); break;
		case 'C': sprite = getSprite(3, 11, 30); break;
		case 'D': sprite = getSprite(4, 11, 30); break;
		case 'E': sprite = getSprite(5, 11, 30); break;
		case 'F': sprite = getSprite(6, 11, 30); break;
		case 'G': sprite = getSprite(7, 11, 30); break;
		case 'H': sprite = getSprite(8, 11, 30); break;
		case 'I': sprite = getSprite(9, 11, 30); break;
		case 'J': sprite = getSprite(10, 11, 30); break;
		case 'K': sprite = getSprite(11, 11, 30); break;
		case 'L': sprite = getSprite(12, 11, 30); break;
		case 'M': sprite = getSprite(13, 11, 30); break;
		case 'N': sprite = getSprite(14, 11, 30); break;
		case 'O': sprite = getSprite(15, 11, 30); break;
		case 'P': sprite = getSprite(0, 10, 30); break;
		case 'Q': sprite = getSprite(1, 10, 30); break;
		case 'R': sprite = getSprite(2, 10, 30); break;
		case 'S': sprite = getSprite(3, 10, 30); break;
		case 'T': sprite = getSprite(4, 10, 30); break;
		case 'U': sprite = getSprite(5, 10, 30); break;
		case 'V': sprite = getSprite(6, 10, 30); break;
		case 'W': sprite = getSprite(7, 10, 30); break;
		case 'X': sprite = getSprite(8, 10, 30); break;
		case 'Y': sprite = getSprite(9, 10, 30); break;
		case 'Z': sprite = getSprite(10, 10, 30); break;
		case '[': sprite = getSprite(11, 10, 30); break;
		case ' \ ': sprite = getSprite(12, 10, 30); break;
		case ']': sprite = getSprite(13, 10, 30); break;
		case '^': sprite = getSprite(14, 10, 30); break;
		case '_': sprite = getSprite(15, 10, 30); break;

		case '`': sprite = getSprite(0, 9, 30); break;
		case 'a': sprite = getSprite(1, 9, 30); break;
		case 'b': sprite = getSprite(2, 9, 30); break;
		case 'c': sprite = getSprite(3, 9, 30); break;
		case 'd': sprite = getSprite(4, 9, 30); break;
		case 'e': sprite = getSprite(5, 9, 30); break;
		case 'f': sprite = getSprite(6, 9, 30); break;
		case 'g': sprite = getSprite(7, 9, 30); break;
		case 'h': sprite = getSprite(8, 9, 30); break;
		case 'i': sprite = getSprite(9, 9, 30); break;
		case 'j': sprite = getSprite(10, 9, 30); break;
		case 'k': sprite = getSprite(11, 9, 30); break;
		case 'l': sprite = getSprite(12, 9, 30); break;
		case 'm': sprite = getSprite(13, 9, 30); break;
		case 'n': sprite = getSprite(14, 9, 30); break;
		case 'o': sprite = getSprite(15, 9, 30); break;

		case 'p': sprite = getSprite(0, 8, 30); break;
		case 'q': sprite = getSprite(1, 8, 30); break;
		case 'r': sprite = getSprite(2, 8, 30); break;
		case 's': sprite = getSprite(3, 8, 30); break;
		case 't': sprite = getSprite(4, 8, 30); break;
		case 'u': sprite = getSprite(5, 8, 30); break;
		case 'v': sprite = getSprite(6, 8, 30); break;
		case 'w': sprite = getSprite(7, 8, 30); break;
		case 'x': sprite = getSprite(8, 8, 30); break;
		case 'y': sprite = getSprite(9, 8, 30); break;
		case 'z': sprite = getSprite(10, 8, 30); break;
		case '{': sprite = getSprite(11, 8, 30); break;
		case '|': sprite = getSprite(12, 8, 30); break;
		case '}': sprite = getSprite(13, 8, 30); break;
		case '~': sprite = getSprite(14, 8, 30); break;
		case '[]': sprite = getSprite(15, 8, 30); break;
		default: sprite = getSprite(0, 0, 30); break;
	}

	float aux = sprite.end.y;
	sprite.end.y = sprite.start.y;
	sprite.start.y = aux;
	return sprite;
}

Sprite2D::Sprite2D(){

}

Sprite2D Label::getSprite(int nx, int ny, float pct){

	Sprite2D sprite;
	sprite.start.x = ((1 /(float)SPRITE_W)*nx);
	sprite.start.y = ((1 / (float)SPRITE_H)*ny);
	sprite.end.x = (sprite.start.x + (1 / (float)SPRITE_W)) - 0.014;
	sprite.end.y = (sprite.start.y + (1 / (float)SPRITE_H)) - 0.014;

	sprite.ratio = (this->size.x / 100) * pct;

	return sprite;

}


void Label::render(int primitive, Camera * camera){

	if (fading.fade) fadeOverTime();
	if (roll.active) insertOverTime(100);
	
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
	shader->setVector3("text_color", this->color);
	shader->setFloat("time", SDL_GetTicks() * 20);

	this->mesh->render(primitive, shader, true);

	shader->disable();
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);

}

void Label::update(Game & game){
	Vector2 oldstate = state;
	state = Game::instance->getState();
	
	if (oldstate.x > 0){
		Vector2 additer = state - oldstate;
		addit = this->size.x + (additer.x / 100);
		this->size = Vector2(this->size.x + (additer.x / 100), this->size.y + (additer.y / 100));
	}
	setText(this->word.c_str());

	for (auto it = children.begin(); it != children.end(); ++it){
		((Label*)(*it))->setText(this->word.c_str());
	}
}

void Label::readyFade(){

	this->roll.totalWord = this->word;
	((Label*)this->children[0])->roll.totalWord = this->word;
	this->roll.active = true;
}