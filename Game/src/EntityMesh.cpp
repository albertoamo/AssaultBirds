#include "EntityMesh.h"
#include "BulletManager.h"
#include <iostream>
#include "Renderer.h"

//float Rotaters::previoustime = 0.0;
std::vector<Rotaters> EntityMesh::rotaters;

EntityMesh::EntityMesh(const char* name, Material* material, Vector3 origin) : material(NULL), mesh(NULL), totalRadius(0.0f){
	
	//hemos de dejar esto un poco más boniquete.
	this->Type = "EntityMesh";
	this->Name = name;
	this->material = material;
	this->origin = origin;
	mesh = new Mesh();

	maxs = Vector3(0,0,0);
	mins = Vector3(0,0,0);
	this->options = 0x00; //WHAT THE FUCK?

	lowMesh = NULL;
	parent = NULL;
	collision_model = NULL;

	Model.setTranslation(origin.x, origin.y, origin.z);

}

bool EntityMesh::loadLowMesh(const char* filename){

	
	std::string str = filename;
	std::string ext = str.substr( str.size() - 4,4 );
	std::string completeStr = "assets/models/bin/" + str;

	auto iterator = Mesh::MeshManager.find(completeStr);
	if (iterator != Mesh::MeshManager.end()){
		lowMesh = iterator->second;
		return true;
	}

	if (ext == ".obj" || ext == ".OBJ"){
		lowMesh = new Mesh();
		std::string name = "assets/models/obj/" + str;
		return lowMesh->loadOBJ(name.c_str());
	}
	else if (ext == ".ase" || ext == ".ASE"){
		lowMesh = new Mesh();
		std::string name = "assets/models/ase/" + str;
		return lowMesh->loadASE(name.c_str());
	}
	else if (ext == ".hym" || ext == ".HYM"){
		lowMesh = new Mesh();
		std::string name = "assets/models/bin/" + str;
		return lowMesh->loadBIN(name.c_str());
	}

	return false;
}

EntityMesh::~EntityMesh()
{
	delete(collision_model);
}

bool EntityMesh::loadMesh(const char* filename){
	
	std::string str = filename;
	std::string ext = str.substr( str.size() - 4,4 );
	std::string completeStr = "assets/models/bin/" + str;

	auto iterator = Mesh::MeshManager.find(completeStr);
	if (iterator != Mesh::MeshManager.end()){
		mesh = iterator->second;
		return true;
	}

	if (ext == ".obj" || ext == ".OBJ"){
		std::string name = "assets/models/obj/" + str;
		return mesh->loadOBJ(name.c_str());
	}
	else if (ext == ".ase" || ext == ".ASE"){
		std::string name = "assets/models/ase/" + str;
		return mesh->loadASE(name.c_str());
	}
	else if (ext == ".hym" || ext == ".HYM"){
		std::string name = "assets/models/bin/" + str;
		return mesh->loadBIN(name.c_str());
	}

	return false;
}

void EntityMesh::enableFlags(){
	
	//blending options
	if (material->blendTest == material->blend){
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else if (material->blendTest == material->add){
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	}

	if (material->cullTest == material->none){
		glDisable(GL_CULL_FACE);
	}

	if (material->cullTest == material->front){
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
	}
	else if (material->cullTest == material->back){
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}


	//depth options
	if (material->depthTest == material->disable){
		glDisable(GL_DEPTH_TEST);
	}

}

void EntityMesh::disableFlags(){

	if (material->depthTest == material->disable)
		glEnable( GL_DEPTH_TEST );

	if (material->blendTest != material->default)
		glDisable(GL_BLEND);

	if (material->cullTest == material->none)
		glEnable(GL_CULL_FACE);

	if (material->cullTest == material->front)
		glCullFace(GL_BACK);
}
void EntityMesh::render(int primitive, Camera * camera, EntityLight light){

	//In case parent has a lowmesh, and the childrens do not,
	if (getOrigin().distance(camera->eye) > LOD_DISTANCE && parent != NULL && ((EntityMesh*)parent)->lowMesh && this->lowMesh == NULL)
		return;

	//Renderizamos los hijos antes del el padre
	for (auto it = children.begin(); it != children.end(); ++it){
		if ((*it)->options & 0x05)
			continue;
		((EntityMesh*)(*it))->render(GL_TRIANGLES, camera, light);
	}

	this->enableFlags();
	material->shader->enable();

	Matrix44 viewProjection = camera->viewprojection_matrix;
	Matrix44 mvp = this->getGlobalMatrix() * viewProjection;
	Matrix44 modelT = this->getGlobalMatrix().getRotationOnly();

	//Matrices
	material->shader->setMatrix44("model_matrix", getGlobalMatrix());
	material->shader->setMatrix44("normal_matrix", modelT);
	material->shader->setMatrix44("mvp", mvp);

	//Texturas
	material->shader->setTexture("color_texture", material->colormap->texture_id);
	material->shader->setTexture("color_textureN", material->normalmap->texture_id);

	//Propiedades del material
	material->shader->setFloat("alpha", material->alpha);

	//Posiciones de la luz y de la camara
	material->shader->setVector3("light_pos", light.position);
	material->shader->setVector3("eye_pos", camera->eye);

	//Propiedades de la luz
	material->shader->setFloat("id", light.diffuse_factor);
	material->shader->setFloat("is", light.specular_factor);
	material->shader->setVector3("light_color", light.color);

	material->shader->setFloat("time", SDL_GetTicks() * 20);

	for (unsigned int x = 0; x < material->floats.size(); x++){
		material->shader->setFloat(material->floats[x].first.c_str(), *material->floats[x].second);
	}
	for (unsigned int x = 0; x < material->matrices.size(); x++){
		material->shader->setMatrix44(material->matrices[x].first.c_str(), *material->matrices[x].second);
	}
	for (unsigned int x = 0; x < material->maps.size(); x++){
		material->shader->setTexture(material->maps[x].first.c_str(), material->maps[x].second->texture_id);
	}

	if (getOrigin().distance(camera->eye) < LOD_DISTANCE || lowMesh == NULL)
		this->mesh->render(primitive, material->shader, true);
	else if (getOrigin().distance(camera->eye) > LOD_DISTANCE && lowMesh != NULL && parent == NULL)
		this->lowMesh->render(primitive, material->shader, true);

	material->shader->disable();
	this->disableFlags();

}


void EntityMesh::render(int primitives, RenderStack * stack){

	//In case parent has a lowmesh, and the childrens do not,
	if (getOrigin().distance(stack->camera->eye) > LOD_DISTANCE && parent != NULL && ((EntityMesh*)parent)->lowMesh && this->lowMesh == NULL)
		return;

	//Renderizamos los hijos antes del el padre
	for (auto it = children.begin(); it != children.end(); ++it){
		if((*it)->options & 0x05)
			continue;
		((EntityMesh*)(*it))->render(GL_TRIANGLES, stack);
	}

	this->enableFlags();
	stack->shader->enable();

	Matrix44 viewProjection = stack->camera->viewprojection_matrix;
	Matrix44 mvp = this->getGlobalMatrix() * viewProjection;
	Matrix44 modelT = this->getGlobalMatrix().getRotationOnly();

	//Variables from entitymesh attributes
	stack->shader->setMatrix44("model_matrix", getGlobalMatrix());
	stack->shader->setMatrix44("normal_matrix", modelT);
	stack->shader->setMatrix44("mvp", mvp);
	stack->shader->setTexture("color_texture", material->colormap->texture_id);
	stack->shader->setTexture("color_textureN", material->normalmap->texture_id);
	stack->shader->setFloat("alpha", material->alpha);

	//Variables from the stack
	stack->shader->setVector3("light_pos", stack->lights[0]->position);
	stack->shader->setVector3("eye_pos", stack->camera->eye);
	stack->shader->setFloat("id", stack->lights[0]->diffuse_factor);
	stack->shader->setFloat("is", stack->lights[0]->specular_factor);
	stack->shader->setVector3("light_color", stack->lights[0]->color);
	stack->shader->setFloat("time", SDL_GetTicks() * 20);

	for (unsigned int x = 0; x < stack->maps.size(); x++){
		stack->shader->setTexture(stack->maps[x].first.c_str(), stack->maps[x].second->texture_id);
	}
	for (unsigned int x = 0; x < stack->floats.size(); x++){
		stack->shader->setFloat(stack->floats[x].first.c_str(), stack->floats[x].second);
	}


	if (getOrigin().distance(stack->camera->eye) < LOD_DISTANCE || lowMesh == NULL)
		this->mesh->render(primitives, stack->shader, true);
	else if (getOrigin().distance(stack->camera->eye) > LOD_DISTANCE && lowMesh != NULL && parent == NULL)
		this->lowMesh->render(primitives, stack->shader, true);

	stack->shader->disable();
	this->disableFlags();


}

void EntityMesh::drawNormals(Vector3 color, float distance){

	std::vector< Vector3 > vertices = mesh->vertices; //here we store the vertices
	std::vector< Vector3 > normals = mesh->normals;	 //here we store the normals

	for (unsigned int i = 0; i < mesh->vertices.size(); i++){
		glColor3f(color.x, color.y, color.z);
		glPointSize(3.0);

		vertices[i] = Model * vertices[i];

		glBegin(GL_LINES);
		glVertex3d(vertices[i].x, vertices[i].y, vertices[i].z);
		glVertex3d(vertices[i].x + (distance * normals[i].x), vertices[i].y + (distance * normals[i].y), vertices[i].z + (distance*normals[i].z));
		glEnd();
	}
}

void EntityMesh::calculateTotalRadius(){
	
	//Vector3 maxs, misn;
	maxs.x = -INFINITE;
	maxs.y = -INFINITE;
	maxs.z = -INFINITE;
	mins.x = INFINITE;
	mins.y = INFINITE;
	mins.z = INFINITE;

	//Si es padre y tiene hijos
	if(parent == NULL && children.size() != 0){
		for(int i = 0; i < children.size(); i++)
		{
			Mesh * mesh;
			EntityMesh * hijo = (EntityMesh*) children[i];
			mesh = hijo->mesh;
			for(unsigned int i = 1; i < mesh->vertices.size(); i++){
			//------------------MAXS--------------------
			if(mesh->vertices[i].x > maxs.x)
				maxs.x = mesh->vertices[i].x;
			if(mesh->vertices[i].y > maxs.y)
				maxs.y = mesh->vertices[i].y;
			if(mesh->vertices[i].z > maxs.z)
				maxs.z = mesh->vertices[i].z;

			//-----------------MINS---------------------
			if(mesh->vertices[i].x < mins.x)
				mins.x = mesh->vertices[i].x;
			if(mesh->vertices[i].y < mins.y)
				mins.y = mesh->vertices[i].y;
			if(mesh->vertices[i].z < mins.z)
				mins.z = mesh->vertices[i].z;
			}
		}
	}

	Vector3 centerO = (maxs+mins);
	centerO.x /= 2.0f;
	centerO.y /= 2.0f;
	centerO.z /= 2.0f;

	Vector3 halfSize;

	halfSize = maxs - centerO;

	this->totalRadius = halfSize.length();
}

void EntityMesh::calculateBB()
{	
	BoundingBoxMesh.vertices.clear();

	BoundingBoxMesh.vertices.push_back(Vector3(mins.x, mins.y, mins.z));
	BoundingBoxMesh.vertices.push_back(Vector3(maxs.x, mins.y, mins.z));
	BoundingBoxMesh.vertices.push_back(Vector3(maxs.x, mins.y, maxs.z));
	BoundingBoxMesh.vertices.push_back(Vector3(mins.x, mins.y, maxs.z));

	BoundingBoxMesh.vertices.push_back(Vector3(mins.x, maxs.y, mins.z));
	BoundingBoxMesh.vertices.push_back(Vector3(maxs.x, maxs.y, mins.z));
	BoundingBoxMesh.vertices.push_back(Vector3(maxs.x, maxs.y, maxs.z));
	BoundingBoxMesh.vertices.push_back(Vector3(mins.x, maxs.y, maxs.z));


}

void EntityMesh::drawBB(Matrix44 viewproj)
{  
	
	std::vector< Vector3 > v = BoundingBoxMesh.vertices; //here we store the vertices

	for (unsigned int i = 0; i < v.size(); i++)
	{
		v[i] = getGlobalMatrix() * v[i];
	}

	glColor3f(BBcolor.x, BBcolor.y, BBcolor.z);
	if(v.size())
	{
	glBegin(GL_LINE_STRIP);
	glVertex3d(v[0].x, v[0].y, v[0].z);
	glVertex3d(v[1].x, v[1].y, v[1].z);
	glVertex3d(v[2].x, v[2].y, v[2].z);
	glVertex3d(v[3].x, v[3].y, v[3].z);
	glVertex3d(v[0].x, v[0].y, v[0].z);
	glVertex3d(v[4].x, v[4].y, v[4].z);
	glVertex3d(v[5].x, v[5].y, v[5].z);
	glVertex3d(v[1].x, v[1].y, v[1].z);
	glVertex3d(v[5].x, v[5].y, v[5].z);
	glVertex3d(v[6].x, v[6].y, v[6].z);
	glVertex3d(v[2].x, v[2].y, v[2].z);
	glVertex3d(v[6].x, v[6].y, v[6].z);
	glVertex3d(v[7].x, v[7].y, v[7].z);
	glVertex3d(v[3].x, v[3].y, v[3].z);
	glVertex3d(v[7].x, v[7].y, v[7].z);
	glVertex3d(v[4].x, v[4].y, v[4].z);
	glEnd();
	}
	glColor3f(1, 1, 1);

}

void EntityMesh::actualizeCollisionModel()
{
	if(collision_model)
		collision_model->setTransform(getGlobalMatrix().m);
}

void EntityMesh::buildCollisionTree(){

	assert( collision_model == NULL);

	//Si es una EntityMesh con hijos
	if(children.size()){
		collision_model = newCollisionModel3D();
		unsigned int numberTriangles = 0;
		std::vector<Vector3> vertices = std::vector<Vector3>();

		//Cogemos la cantidad total de triangulos y guardamos los vertices para no volver a iterar
		for(auto child = children.begin(); child != children.end(); child++)
		{
			if((*child)->Type = "EntityMesh")
			{
				EntityMesh * childEm = (EntityMesh*)(*child);
				numberTriangles += childEm->mesh->vertices.size();
				for(unsigned int i = 0; i < childEm->mesh->vertices.size(); i++)
					vertices.push_back(childEm->mesh->vertices[i]);
			}
		}
		collision_model->setTriangleNumber(numberTriangles/ 3);

		for(unsigned int i = 0; i < vertices.size(); i += 3)
			collision_model->addTriangle(vertices[i].v,vertices[i+2].v,vertices[i+2].v);
	
		collision_model->finalize();
	}
	else{
		collision_model = newCollisionModel3D();
		collision_model->setTriangleNumber(mesh->vertices.size()/3);
		for(unsigned int i = 0; i < mesh->vertices.size(); i += 3)	
			collision_model->addTriangle(mesh->vertices[i].v,mesh->vertices[i+1].v,mesh->vertices[i+2].v);
		collision_model->finalize();
	}
}


void EntityMesh::bulletCollision(Bullet & _bullet)
{
	//std::cout << "Bala lanzada por " << _bullet.author << " choca con: " << id << std::endl;
	_bullet.ttl = -1.0f;
}

void EntityMesh::setRotation(EntityMesh * entity, Vector3 direction, float time){

	Rotaters rot;
	rot.entity = entity;
	rot.time = time;
	rot.orient = direction;
	rot.previoustime = 0.0;

	EntityMesh::rotaters.push_back(rot);
}
void EntityMesh::rotateAll(){
	
	for (unsigned int x = 0; x < rotaters.size(); x++){
		rotaters[x].entity->rotateOverTime(rotaters[x].previoustime, rotaters[x].time, rotaters[x].orient);
	}
}

void EntityMesh::rotateOverTime(float & previous, float time, Vector3 direction){


	float currentTime = SDL_GetTicks();
	int timeInterval = currentTime - previous;
	float value = ((float)timeInterval / time);

	if (timeInterval > time){
		previous = currentTime;
	}

	//Entity * entd = World::Instance()->models[10]->getChildren("ax_carrier_antenna");
	Vector3 newCenter = ((EntityMesh*)this)->mesh->center;
	this->Model.setTranslation(-newCenter.x, -newCenter.y, -newCenter.z);
	this->Model.rotate(6.28318531 * value, direction);
	this->Model.traslate(newCenter.x, newCenter.y, newCenter.z);

}

void EntityMesh::rotateOverAngle(float angle, Vector3 direction)
{
	Vector3 newCenter = ((EntityMesh*)this)->mesh->center;
	this->Model.setTranslation(-newCenter.x, -newCenter.y, -newCenter.z);
	this->Model.rotate(angle, direction);
	this->Model.traslate(newCenter.x, newCenter.y, newCenter.z);
}