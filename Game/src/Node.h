#ifndef GAMENODE_H
#define GAMENODE_H
#include "Entity.h"
#include "mesh.h"
#include "Renderer.h"
#include "framework.h"

//debug class... 
class Node: public Entity{
public:

	Vector3 dir;
	Vector3 origin;


	Node(){}
	~Node(){}
	Node(Vector3 & origin, Vector3 dir){
		this->origin = origin;
		this->dir = dir;
	}


};

class ScriptPoint : public Entity{
public:

	Mesh * mesh;
	Mesh * connector;
	Shader * shader;
	Vector3 color;
	const char* type;

	int size;
	bool visible;

	ScriptPoint(const char* type, Vector3 & color){
		this->type = type;
		this->color = Vector3(color.x / 255, color.y / 255, color.z / 255);
		this->size = 5;
		this->visible = true;
		this->mesh = new Mesh();
		this->connector = new Mesh();
		Model.setIdentity();


		loadShader("simple_color");
	}

	~ScriptPoint(){
		
		delete(mesh);
		delete(connector);
		delete(shader);

		if (type == "script_origin")
			delete(connector);
	}

	bool loadShader(const char * dir){

		this->shader = new Shader();
		std::string dr1 = "assets/materials/shaders/" + std::string(dir) + ".vs";
		std::string dr2 = "assets/materials/shaders/" + std::string(dir) + ".ps";

		if (!this->shader->load(dr1, dr2)){
			
			std::cout << "Shader ERROR: [ " << dir << " ]" << std::endl;
			return false;
		}

		return true;
	}
	void render(Renderer * r){
		r->render(*this);
	}

	void attachNode(Vector3 origen, Vector3 dir){
		Node * node = new Node(origen, dir);
		drawNode(origen, dir);
		addChildren(node);

		//connector->vertices.push_back(origin);
		//connector->vertices.push_back(Vector3(origin.x + dir.x * 25, origin.y + dir.y * 25, origin.z + dir.z * 25));
	}

	void setColor(Vector3 & color){
		this->color = Vector3(color.x / 255, color.y / 255, color.z / 255);
	}

	void setSize(float size){
		this->size = size;
	}

	void drawNode(Vector3 origen, Vector3 dir){

		std::vector< Vector3 > oldvert1 = this->mesh->vertices;
		delete(this->mesh);
		this->mesh = new Mesh();
		this->mesh->vertices = oldvert1;

		if (type == "script_origin"){
			std::vector< Vector3 > oldvert2 = this->connector->vertices;
			delete(this->connector);
			this->connector = new Mesh();
			this->connector->vertices = oldvert2;

			if (this->connector->vertices.size() <= 0){
				this->connector->vertices.push_back(origen);
				this->connector->vertices.push_back(origen);
			}
			else{
				this->connector->vertices.push_back(this->connector->vertices.back());
				this->connector->vertices.push_back(origen);
			}

		}

		this->connector->vertices.push_back(Vector3(origen.x + dir.x * 3 * size, origen.y + dir.y * 3 * size, origen.z + dir.z * 3 * size));
		this->connector->vertices.push_back(origen);


		std::cout << connector->vertices.size() << std::endl;
		float vertex_buffer[] = {
			-1.0f, -1.0f, -1.0f, // triangle 1 : begin
			-1.0f, -1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f, // triangle 1 : end
			1.0f, 1.0f, -1.0f, // triangle 2 : begin
			-1.0f, -1.0f, -1.0f,
			-1.0f, 1.0f, -1.0f, // triangle 2 : end
			1.0f, -1.0f, 1.0f,
			-1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, 1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, -1.0f,
			1.0f, -1.0f, 1.0f,
			-1.0f, -1.0f, 1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f, 1.0f, 1.0f,
			-1.0f, -1.0f, 1.0f,
			1.0f, -1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, 1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, -1.0f,
			-1.0f, 1.0f, -1.0f,
			1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, -1.0f,
			-1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 1.0f
		};

		for (unsigned int x = 0; x < (sizeof(vertex_buffer) / sizeof(*vertex_buffer)); x = x + 3){
			vertex_buffer[x] = vertex_buffer[x] * this->size;
			vertex_buffer[x+1] = vertex_buffer[x+1] * this->size;
			vertex_buffer[x+2] = vertex_buffer[x+2] * this->size;
			Vector3 vertex = Vector3(origen.x + vertex_buffer[x], origen.y + vertex_buffer[x + 1], origen.z + vertex_buffer[x + 2]);
			this->mesh->vertices.push_back(vertex);
 		}
	}
};

#endif