#ifndef MATERIAL_H
#define MATERIAL_H
#include "includes.h"

#include <vector>
#include "framework.h"
#include "shader.h"
#include "texture.h"

class Material
{
public:

	//Name
	const char* name;

	//Shader
	Shader * shader;

	//Textures from the material
	Texture * colormap;
	Texture * normalmap;
	Texture * specularmap;

	std::vector< std::pair<std::string, Matrix44*> > matrices;
	std::vector< std::pair<std::string, Texture*> > maps;
	std::vector< std::pair<std::string, float*> > floats;

	//Material properties
	std::string surfaceType;
	std::string materialType;
	//std::string usage;
	//std::string sort;
	
	enum BlendTest { default = 0x01, add = 0x02, blend = 0x03 };
	enum AlphaTest { always = 0x02, never = 0x03 };
	enum CullTest { back = 0x01, front = 0x02, none = 0x03 };
	enum DepthTest { enable = 0x01, disable = 0x02 };

	unsigned char blendTest;
	unsigned char alphaTest;
	unsigned char cullTest;
	unsigned char depthTest;

	//Material constants
	float alpha;

	Material();
	Material(const char* name, std::string surfaceType, float alpha);
	~Material();
	
	void loadShader(const char* filename1, const char* filename2);
	void loadShader(const char* filename1, const char* filename2, const char* macros);
	bool loadTexture(const char* filename1, const char* filename2);//Load textures using alpha channel as specular
	bool loadTexture(const char* filename1, const char* filename2, const char* filename3);//Load textures using specular as a different colormap

	//Setters and getters
	void setAtt(std::string surfaceType, std::string materialType);
	void setSurface(std::string surfaceType);

	std::string getSurface();

};

#endif