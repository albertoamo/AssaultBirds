                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       #ifndef ENTITY_H
#define ENTITY_H

#include "framework.h"
#include "includes.h"
#include "camera.h"

class Entity{
	public:

		enum Options { collision = 0x01, animated = 0x02, lod = 0x03, hitbox = 0x04, visible = 0x05, clip = 0x06 };
		unsigned char options;

		unsigned int id;
		static unsigned int idCounter;

		//Attributes
		Vector3 origin;
		Matrix44 Model;
		const char * Name;
		const char * Type;

		bool toDelete;

		Entity* parent;
		std::vector<Entity*> children;

		//Constructor methods
		Entity();
		Entity(const char* name);

		~Entity();

		//Basic getters and setters
		Vector3 getOrigin();
		Matrix44 getGlobalMatrix();

		void setId();
		void setName(const char* name);

		//Basic drawing methods
		virtual void render(int primitive = GL_TRIANGLES){};
		virtual void update(float elapsedTime);

		//Self movement methods
		void scaleLocal(float val);
		void translateLocal(Vector3 Traslator);
		void rotateLocal(float angle_in_rads, Vector3 axis);

		//Children based methods
		void deleteChildren();
		void addChildren(Entity * entity);
		Entity * getChildren(const char* name);
		Entity * getGrandParent(Entity * father);
		unsigned int getRootId(Entity * father);

		//Children movement methods
		void scaleChildren(const char * name, float size);
		void translateChildren(const char * name, Vector3 axis);
		void rotateChildren(const char * name, float angle_in_rads, Vector3 axis);
		void printChildren();

		//Event processing
		virtual void processEvent(std::string event);


};

#endif                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   