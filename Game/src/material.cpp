#include "material.h"

Material::Material(const char* name, std::string surfaceType, float alpha)
{
	blendTest = 0x00;
	alphaTest = 0x00;
	cullTest = 0x00;
	depthTest = 0x00;

	this->surfaceType = surfaceType;
	this->name = name;
	this->alpha = alpha;
	this->cullTest = this->front;
	this->depthTest = this->enable;
	//this->blendTest = this->default;
	this->shader = NULL;
	this->colormap = NULL;
	this->normalmap = NULL;
	this->specularmap = NULL;

}

Material::~Material(){

	//DEL LOS DELETES DE LAS TEXTURAS Y DE LOS SHADERS SE ENCARGAN LOS MANAGERS

	//delete shader;
	//delete colormap;
	//delete normalmap;

	//if(specularmap)
		//delete specularmap;
}

void Material::loadShader(const char* filename1, const char* filename2){
	
	shader = new Shader();
	std::string dr1 = "assets/materials/shaders/" + std::string(filename1);
	std::string dr2 = "assets/materials/shaders/" + std::string(filename2);
	/*
	if (!shader->load(dr1.c_str(), dr2.c_str())){
		std::cout << "shader not found or error" << std::endl;
		//exit(0); DE MOMENTO COMENTAMOS EL EXIT PARA PODER DEBUGGAR
	}*/

	shader = shader->Load(dr1.c_str(), dr2.c_str());
	if(shader == NULL)
		std::cout << "shader not found or error" << std::endl;

	//this->shader = shader; //(( <- (8) yo y mi yo y mi ballantines (8) [Sutil metfora]
}

void Material::loadShader(const char* filename1, const char* filename2, const char* macros){

	shader = new Shader();
	std::string dr1 = "assets/materials/shaders/" + std::string(filename1);
	std::string dr2 = "assets/materials/shaders/" + std::string(filename2);

	shader = shader->Load(dr1.c_str(), dr2.c_str(),macros);
	if (shader == NULL)
		std::cout << "shader not found or error" << std::endl;

	//this->shader = shader; //(( <- (8) yo y mi yo y mi ballantines (8) [Sutil metfora]
}

bool Material::loadTexture(const char* color, const char* normal){

	std::string d_color = "assets/images/" + std::string(color);
	std::string d_normal = "assets/images/" + std::string(normal);
	
	colormap = new Texture();
	colormap = colormap->Load(d_color.c_str());
	if (colormap == NULL)
		std::cout << "An error has ocurred loading " << color << ". " << std::endl;

	normalmap = new Texture();
	normalmap = normalmap->Load(d_normal.c_str());
	if (normalmap == NULL)
		std::cout << "An error has ocurred loading " << normal << ". " << std::endl;

	return true;
}


bool Material::loadTexture(const char* color, const char* normal, const char* specular){

	std::string d_color = "assets/images/" + std::string(color);
	std::string d_normal = "assets/images/" + std::string(normal);
	std::string d_spec = "assets/images/" + std::string(specular);

	if (!colormap->load(d_color.c_str()))
		std::cout << "An error has ocurred loading " << color << ". " << std::endl;
	if (!normalmap->load(d_normal.c_str()))
		std::cout << "An error has ocurred loading " << normal << ". " << std::endl;
	if (!specularmap->load(d_spec.c_str()))
		std::cout << "An error has ocurred loading " << specular << ". " << std::endl;

	return true;
}

void Material::setSurface(std::string surfaceType){
	this->surfaceType = surfaceType;
}

std::string Material::getSurface(){
	return this->surfaceType;
}

void Material::setAtt(std::string surfaceType, std::string materialType){
	this->surfaceType = surfaceType;
	this->materialType = materialType;
}