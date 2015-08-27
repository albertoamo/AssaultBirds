#include "loader.h"

//This loads the material. Forgot to add the Null terminator character, so having some troubles and need to do the substring trick
Material * loadMat(const char* filename){

	size_t header[8];
	short alpha;

	std::string fileFolder = std::string("assets/materials/") + std::string(filename);
	FILE * file = fopen(fileFolder.c_str(), "rb");

	if (file != NULL){

		fread(&alpha, sizeof(short), 1, file);
		fread(header, sizeof(header), 1, file);

		char * colorMap = (char *)malloc((int)header[0]);
		char * normalMap = (char *)malloc((int)header[1]);
		char * shaderPS = (char *)malloc((int)header[2]);
		char * shaderVS = (char *)malloc((int)header[3]);

		char * matType = (char *)malloc((int)header[4]);
		char * surfType = (char *)malloc((int)header[5]);
		char * sort = (char *)malloc((int)header[6]);
		char * usage = (char *)malloc((int)header[7]);

		unsigned char blendTest = 0x00;
		unsigned char alphaTest = 0x00;
		unsigned char cullTest = 0x00;
		unsigned char depthTest = 0x00;
		fread(&blendTest, sizeof(unsigned char), 1, file);
		fread(&alphaTest, sizeof(unsigned char), 1, file);
		fread(&cullTest, sizeof(unsigned char), 1, file);
		fread(&depthTest, sizeof(unsigned char), 1, file);

		//std::cout << blendTest << std::endl;
		
		fread(colorMap, 1, (int)header[0], file);
		fread(normalMap, 1, (int)header[1], file);
		fread(shaderPS, 1, (int)header[2], file);
		fread(shaderVS, 1, (int)header[3], file);

		fread(matType, 1, (int)header[4], file);
		fread(surfType, 1, (int)header[5], file);
		fread(sort, 1, (int)header[6], file);
		fread(usage, 1, (int)header[7], file);

		std::string cMap = std::string(colorMap).substr(0, header[0]);
		std::string nMap = std::string(normalMap).substr(0, header[1]);
		std::string shadPS = std::string(shaderPS).substr(0, header[2]);
		std::string shadVS = std::string(shaderVS).substr(0, header[3]);

		std::string matT = std::string(matType).substr(0, header[4]);
		std::string surfT = std::string(surfType).substr(0, header[5]);
		std::string sortd = std::string(sort).substr(0, header[6]);
		std::string usaged = std::string(usage).substr(0, header[7]);


		Material * material = new Material(filename, matType, (int)alpha);
		material->loadShader(shadVS.c_str(), shadPS.c_str());
		material->loadTexture(cMap.c_str(), nMap.c_str());

		material->setAtt(surfT, matT);
		material->blendTest = blendTest;
		material->alphaTest = alphaTest;
		material->cullTest = cullTest;
		material->depthTest = depthTest;

		//Here we free all the pointers

		fclose(file);

		free(colorMap);
		free(normalMap);
		free(shaderPS);
		free(shaderVS);

		free(matType);
		free(surfType);
		free(sort);
		free(usage);

		fclose(file);
		return material;
	}

	fclose(file);
	return NULL;
}

EntityMesh* readData(FILE * file, auxModel * model){

	EntityMesh * mFinal = new EntityMesh(model->name.c_str(), loadMat(model->material.c_str()), model->origin);
	Mesh * newMesh = new Mesh();

	bool loaded = false;
	auto iterator = Mesh::MeshManager.find(model->name.c_str());
	if (iterator != Mesh::MeshManager.end()){
		newMesh = iterator->second;
		loaded = true;
	}
	
	unsigned int vectorSize;
	fread(&vectorSize, sizeof(unsigned int), 1, file);

	std::vector< Vector3 > vertexd;
	newMesh->vertices.resize(vectorSize);
	newMesh->normals.resize(vectorSize);
	newMesh->uvs.resize(vectorSize);
	fread(&newMesh->vertices[0], sizeof(Vector3), vectorSize, file);
	fread(&newMesh->normals[0], sizeof(Vector3), vectorSize, file);
	fread(&newMesh->uvs[0], sizeof(Vector2), vectorSize, file);

	if(!loaded){
		newMesh->calculateRadius();
		//newMesh->buildCollisionTree();
	}

	mFinal->mesh = newMesh;

	Mesh::MeshManager.insert(std::pair<std::string, Mesh*>(model->name.c_str(), newMesh)); 

	return mFinal;
}

void readChildren(FILE * file, auxModel * model, EntityMesh* root){

	if (model->children < 1){
		EntityMesh * adder = readData(file, model);
		((Entity*)root)->addChildren((Entity*)adder);

	}
	else{
		for (unsigned int x = 0; x < model->children; x++){
			size_t sizes[2];
			unsigned int children;

			fread(sizes, 1, sizeof(sizes), file);
			
			//Here we reserve memory for all the pointers we need
			char * name = (char *)malloc((int)sizes[0]);
			char * material = (char *)malloc((int)sizes[1]);

			fread(name, 1, (int)sizes[0], file);
			fread(material, 1, (int)sizes[1], file);
			fread(&children, sizeof(unsigned int), 1, file);

			std::string n_name = std::string(name).substr(0, sizes[0]);
			std::string n_material = std::string(material).substr(0, sizes[1]);

			auxModel * xmodel = new auxModel(n_name, n_material, (int)children, model->origin);
			if ((int)children != 0){
				EntityMesh * auxFather = new EntityMesh(n_name.c_str(), loadMat("None"), model->origin);
				((Entity*)root)->addChildren((Entity*)auxFather);
				free(name); free(material);
				readChildren(file, xmodel, auxFather);
			}
			else{
				readChildren(file, xmodel, root);
			}
			//delete(xmodel);

		}
	}

}

EntityMesh * loadModel(Vector3 position, const char* filename){

	std::string fileFolder = std::string("assets/models/") + std::string(filename);
	FILE * file = fopen(fileFolder.c_str(), "rb");

	if (file == NULL){
		std::cout << "Failed to load: " + std::string(filename) + " [FILE NOT FOUND]." << std::endl;
		return NULL;
	}

	//Here we create the actual entitymesh that will hold everything
	EntityMesh * root;
	EntityMesh * lodMesh;
	EntityMesh * hitboxMesh;

	enum Options { collision = 0x01, animated = 0x02, lod = 0x03, hitbox = 0x04 };

	unsigned char options;
	fread(&options, 1, sizeof(options), file); //reads the bit flags for the mesh
	root = loadEntity(file, position);
	root->options = options;
	/*
	if (options & lod){
		lodMesh = loadEntity(file, position);
	}

	if(options & collision & hitbox){
		hitboxMesh = loadEntity(file, position);
	}*/


	fclose(file);
	return root;
}

//First call to begin drawing the model and it's meshes
EntityMesh * loadEntity(FILE * file, Vector3 position){

	size_t sizes[2];
	unsigned int children;

	fread(sizes, 1, sizeof(sizes), file);

	//Here we reserve memory for all the pointers we need
	char * name = (char *)malloc((int)sizes[0]);
	char * material = (char *)malloc((int)sizes[1]);

	fread(name, 1, (int)sizes[0], file);
	fread(material, 1, (int)sizes[1], file);
	fread(&children, sizeof(unsigned int), 1, file);

	std::string n_name = std::string(name).substr(0, sizes[0]);
	std::string n_material = std::string(material).substr(0, sizes[1]);

	auxModel * model = new auxModel(n_name, n_material, (int)children, Vector3(0,0,0));
	EntityMesh * root;
		
	if((int)children == 0){
		model->origin = position;
		root = readData(file, model);
	}
	else{
		root = new EntityMesh(n_name.c_str(), loadMat("None"), position);
		readChildren(file, model, root);
	}

	free(name); free(material);
	root->calculateTotalRadius();
	//delete(model);
	return root;

}




//-------------Vehicle---------------------

Vehicle* readDataEP(FILE * file, auxModel * model){

	Vehicle * mFinal = new Vehicle(model->name.c_str(), loadMat(model->material.c_str()), model->origin);
	Mesh * newMesh = new Mesh();

	bool loaded = false;
	auto iterator = Mesh::MeshManager.find(model->name.c_str());
	if (iterator != Mesh::MeshManager.end()){
		newMesh = iterator->second;
		loaded = true;
	}
	
	unsigned int vectorSize;
	fread(&vectorSize, sizeof(unsigned int), 1, file);

	std::vector< Vector3 > vertexd;
	newMesh->vertices.resize(vectorSize);
	newMesh->normals.resize(vectorSize);
	newMesh->uvs.resize(vectorSize);
	fread(&newMesh->vertices[0], sizeof(Vector3), vectorSize, file);
	fread(&newMesh->normals[0], sizeof(Vector3), vectorSize, file);
	fread(&newMesh->uvs[0], sizeof(Vector2), vectorSize, file);

	if(!loaded){
		newMesh->calculateRadius();
		//newMesh->buildCollisionTree();
	}

	mFinal->mesh = newMesh;

	Mesh::MeshManager.insert(std::pair<std::string, Mesh*>(model->name.c_str(), newMesh)); 

	return mFinal;
}

Vehicle * loadModelEP(Vector3 position, const char* filename){

	std::string fileFolder = std::string("assets/models/") + std::string(filename);
	FILE * file = fopen(fileFolder.c_str(), "rb");

	if (file == NULL){
		std::cout << "Failed to load: " + std::string(filename) + " [FILE NOT FOUND]." << std::endl;
		return NULL;
	}

	//Here we create the actual entitymesh that will hold everything
	Vehicle * root;
	EntityMesh * lodMesh;
	EntityMesh * hitboxMesh;

	enum Options { collision = 0x01, animated = 0x02, lod = 0x03, hitbox = 0x04 };

	unsigned char options;
	fread(&options, 1, sizeof(options), file); //reads the bit flags for the mesh
	root = loadEntityEP(file, position);
	root->options = options;
	/*
	if (options & lod){
		lodMesh = loadEntity(file, position);
	}

	if(options & collision & hitbox){
		hitboxMesh = loadEntity(file, position);
	}*/


	fclose(file);
	return root;
}
//First call to begin drawing the model and it's meshes
Vehicle * loadEntityEP(FILE * file, Vector3 position){

	size_t sizes[2];
	unsigned int children;

	fread(sizes, 1, sizeof(sizes), file);

	//Here we reserve memory for all the pointers we need
	char * name = (char *)malloc((int)sizes[0]);
	char * material = (char *)malloc((int)sizes[1]);

	fread(name, 1, (int)sizes[0], file);
	fread(material, 1, (int)sizes[1], file);
	fread(&children, sizeof(unsigned int), 1, file);

	std::string n_name = std::string(name).substr(0, sizes[0]);
	std::string n_material = std::string(material).substr(0, sizes[1]);

	auxModel * model = new auxModel(n_name, n_material, (int)children, Vector3(0,0,0));
	Vehicle * root = NULL;
		
	if((int)children == 0){
		model->origin = position;
		root = readDataEP(file, model);
	}
	else{
		root = new Vehicle(n_name.c_str(), loadMat("None"), position);
		readChildrenEP(file, model, root);
	}

	free(name); free(material);
	//delete(model);
	root->calculateTotalRadius();

	return root;

}

void readChildrenEP(FILE * file, auxModel * model, Vehicle* root){

	if (model->children < 1){
		EntityMesh * adder = readData(file, model);
		((Entity*)root)->addChildren((Entity*)adder);

	}
	else{
		for (unsigned int x = 0; x < model->children; x++){
			size_t sizes[2];
			unsigned int children;

			fread(sizes, 1, sizeof(sizes), file);
			
			//Here we reserve memory for all the pointers we need
			char * name = (char *)malloc((int)sizes[0]);
			char * material = (char *)malloc((int)sizes[1]);

			fread(name, 1, (int)sizes[0], file);
			fread(material, 1, (int)sizes[1], file);
			fread(&children, sizeof(unsigned int), 1, file);

			std::string n_name = std::string(name).substr(0, sizes[0]);
			std::string n_material = std::string(material).substr(0, sizes[1]);

			auxModel * xmodel = new auxModel(n_name, n_material, (int)children, model->origin);
			if ((int)children != 0){
				EntityMesh * auxFather = new EntityMesh(n_name.c_str(), loadMat("None"), model->origin);
				((Entity*)root)->addChildren((Entity*)auxFather);
				free(name); free(material);
				readChildren(file, xmodel, auxFather);
			}
			else{
				readChildren(file, xmodel, root);
			}
			//delete(xmodel);
		}
	}

}


//-------------GroundObjective---------------------

GroundObjective* readDataGO(FILE * file, auxModel * model){

	GroundObjective * mFinal = new GroundObjective(model->name.c_str(), loadMat(model->material.c_str()), model->origin);
	Mesh * newMesh = new Mesh();

	bool loaded = false;
	auto iterator = Mesh::MeshManager.find(model->name.c_str());
	if (iterator != Mesh::MeshManager.end()){
		newMesh = iterator->second;
		loaded = true;
	}
	
	unsigned int vectorSize;
	fread(&vectorSize, sizeof(unsigned int), 1, file);

	std::vector< Vector3 > vertexd;
	newMesh->vertices.resize(vectorSize);
	newMesh->normals.resize(vectorSize);
	newMesh->uvs.resize(vectorSize);
	fread(&newMesh->vertices[0], sizeof(Vector3), vectorSize, file);
	fread(&newMesh->normals[0], sizeof(Vector3), vectorSize, file);
	fread(&newMesh->uvs[0], sizeof(Vector2), vectorSize, file);

	if(!loaded){
		newMesh->calculateRadius();
		//newMesh->buildCollisionTree();
	}

	mFinal->mesh = newMesh;

	Mesh::MeshManager.insert(std::pair<std::string, Mesh*>(model->name.c_str(), newMesh)); 

	return mFinal;
}

GroundObjective * loadModelGO(Vector3 position, const char* filename){

	std::string fileFolder = std::string("assets/models/") + std::string(filename);
	FILE * file = fopen(fileFolder.c_str(), "rb");

	if (file == NULL){
		std::cout << "Failed to load: " + std::string(filename) + " [FILE NOT FOUND]." << std::endl;
		return NULL;
	}

	//Here we create the actual entitymesh that will hold everything
	GroundObjective * root;
	EntityMesh * lodMesh;
	EntityMesh * hitboxMesh;

	enum Options { collision = 0x01, animated = 0x02, lod = 0x03, hitbox = 0x04 };

	unsigned char options;
	fread(&options, 1, sizeof(options), file); //reads the bit flags for the mesh
	root = loadEntityGO(file, position);
	root->options = options;
	/*
	if (options & lod){
		lodMesh = loadEntity(file, position);
	}

	if(options & collision & hitbox){
		hitboxMesh = loadEntity(file, position);
	}*/


	fclose(file);
	return root;
}

//First call to begin drawing the model and it's meshes
GroundObjective * loadEntityGO(FILE * file, Vector3 position){

	size_t sizes[2];
	unsigned int children;

	fread(sizes, 1, sizeof(sizes), file);

	//Here we reserve memory for all the pointers we need
	char * name = (char *)malloc((int)sizes[0]);
	char * material = (char *)malloc((int)sizes[1]);

	fread(name, 1, (int)sizes[0], file);
	fread(material, 1, (int)sizes[1], file);
	fread(&children, sizeof(unsigned int), 1, file);

	std::string n_name = std::string(name).substr(0, sizes[0]);
	std::string n_material = std::string(material).substr(0, sizes[1]);

	auxModel * model = new auxModel(n_name, n_material, (int)children, Vector3(0,0,0));
	GroundObjective * root = NULL;
		
	if((int)children == 0){
		model->origin = position;
		root = readDataGO(file, model);
	}
	else{
		root = new GroundObjective(n_name.c_str(), loadMat("None"), position);
		readChildrenGO(file, model, root);
	}

	free(name); free(material);
	//delete(model);
	root->calculateTotalRadius();

	return root;

}

void readChildrenGO(FILE * file, auxModel * model, GroundObjective* root){

	if (model->children < 1){
		EntityMesh * adder = readData(file, model);
		((Entity*)root)->addChildren((Entity*)adder);

	}
	else{
		for (unsigned int x = 0; x < model->children; x++){
			size_t sizes[2];
			unsigned int children;

			fread(sizes, 1, sizeof(sizes), file);
			
			//Here we reserve memory for all the pointers we need
			char * name = (char *)malloc((int)sizes[0]);
			char * material = (char *)malloc((int)sizes[1]);

			fread(name, 1, (int)sizes[0], file);
			fread(material, 1, (int)sizes[1], file);
			fread(&children, sizeof(unsigned int), 1, file);

			std::string n_name = std::string(name).substr(0, sizes[0]);
			std::string n_material = std::string(material).substr(0, sizes[1]);

			auxModel * xmodel = new auxModel(n_name, n_material, (int)children, model->origin);
			if ((int)children != 0){
				EntityMesh * auxFather = new EntityMesh(n_name.c_str(), loadMat("None"), model->origin);
				((Entity*)root)->addChildren((Entity*)auxFather);
				free(name); free(material);
				readChildren(file, xmodel, auxFather);
			}
			else{
				readChildren(file, xmodel, root);
			}
			//delete(xmodel);
		}
	}

}