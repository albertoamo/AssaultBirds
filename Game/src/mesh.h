/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	The Mesh contains the info about how to render a mesh and also how to parse it from a file.
*/

#ifndef MESH_H
#define MESH_H
#include <cassert>

#include "includes.h"

#include <map>
#include <vector>
#include "framework.h"

#include <string>
#include <sys/stat.h>

#include "coldet/coldet.h"

#define LOD_DISTANCE 100

class Shader;


class Mesh
{
public:
	std::vector< Vector3 > vertices; //here we store the vertices
	std::vector< Vector3 > normals;	 //here we store the normals
	std::vector< Vector2 > uvs;	 //here we store the texture coordinates
	std::vector< Vector4 > colors;	 //here we store colors by vertex
	std::vector< Vector3 > tangents;
	bool VBOLoaded;
	Vector3 center;

	CollisionModel3D* collision_model;

	float radius;

	Mesh();
	~Mesh();
	static void deleteMeshes();
	void clear();
	void render(int primitive = GL_TRIANGLES);
	void render(int primitive, Shader* sh, bool VBO = false);
	void renderDFL(int primitive, Shader* sh);
	void renderVBO(int primitive, Shader* sh);

	void createQuad(float center_x, float center_y, float w, float h, bool flip_uvs);
	void createQuad(Vector2 pos, Vector2 size, Vector2 uvmapf, Vector2 uvmape, bool flip_uvs);
	void createPlane(float size);

	bool loadASE(const char* filename);
	bool loadOBJ(const char* filename);

	void fillTangents();

	bool writeBIN(const char* filename);
	bool loadBIN(const char* filename);
	
	typedef std::map<std::string, Mesh*> MMap;
	static MMap MeshManager;

	//PARA VBO! 
	GLuint vertices_vbo_id;
	GLuint normals_vbo_id;
	GLuint uvs_vbo_id;
	GLuint colors_vbo_id;
	GLuint tangents_vbo_id;

	void upload();

	void calculateRadius();

	//void buildCollisionTree();

};

#endif