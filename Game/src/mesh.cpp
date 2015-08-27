                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 #include "mesh.h"
#include <cassert>
#include "includes.h"
#include "shader.h"

#include "extra\textparser.h"

#include <string>
#include <sys/stat.h>
 
std::vector<std::string> tokenize(const std::string& source, const char* delimiters, bool process_strings = false);
Vector2 parseVector2(const char* text);
Vector3 parseVector3(const char* text, const char separator);
void fillTangents();

Mesh::MMap Mesh::MeshManager = Mesh::MMap();

Mesh::Mesh()
{
	vertices = std::vector< Vector3 >();
	VBOLoaded = false;
	radius = 0.0;
	collision_model = NULL;

	vertices_vbo_id = 0;
	normals_vbo_id = 0;
	uvs_vbo_id = 0;
	colors_vbo_id = 0;
	tangents_vbo_id = 0;

}

Mesh::~Mesh()
{
	if(vertices_vbo_id)
		glDeleteBuffers(1, &vertices_vbo_id);
	if(normals_vbo_id)
		glDeleteBuffers(1, &normals_vbo_id);
	if(uvs_vbo_id)
		glDeleteBuffers(1, &uvs_vbo_id);
	if(colors_vbo_id)
		glDeleteBuffers(1, &colors_vbo_id);
	if(tangents_vbo_id)
		glDeleteBuffers(1, &tangents_vbo_id);

	if (vertices.size() > 0){
		vertices.clear();
		normals.clear();
		uvs.clear();
		tangents.clear();
	}

	//delete(collision_model);
}

void Mesh::deleteMeshes(){
	for(auto it = MeshManager.begin(); it != MeshManager.end(); it++)
		delete(it->second);

	MeshManager.clear();
}

void Mesh::clear()
{
	vertices.clear();
	normals.clear();
	uvs.clear();
	colors.clear();

	//glDeleteBuffersARB(......)
}

void Mesh::render(int primitive)
{
	assert(vertices.size() && "No vertices in this mesh");

    glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, &vertices[0] );

	if (normals.size())
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, 0, &normals[0] );
	}

	if (uvs.size())
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2,GL_FLOAT, 0, &uvs[0] );
	}

	if (colors.size())
	{
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(4,GL_FLOAT, 0, &colors[0] );
	}

	glDrawArrays(primitive, 0, (GLsizei)vertices.size() );
	glDisableClientState(GL_VERTEX_ARRAY);

	if (normals.size())
		glDisableClientState(GL_NORMAL_ARRAY);
	if (uvs.size())
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	if (colors.size())
		glDisableClientState(GL_COLOR_ARRAY);

}

void Mesh::render( int primitive, Shader* sh, bool VBO){
	if(VBO)
		renderVBO(primitive, sh);
	else
		renderDFL(primitive, sh);
}

void Mesh::renderDFL( int primitive, Shader* sh )
{
	if(!sh || !sh->compiled)
        return render(primitive);
    
	assert(vertices.size() && "No vertices in this mesh");

    int location = sh->getAttribLocation("a_vertex");
	assert(location != -1 && "No a_vertex found in shader");

	if(location == -1)
		return;

    glEnableVertexAttribArray(location);
    glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 0, &vertices[0] );

    if (normals.size())
    {
        location = sh->getAttribLocation("a_normal");
        if(location != -1)
        {
            glEnableVertexAttribArray(location);
            glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 0, &normals[0] );
        }
    }
    
	if (uvs.size())
	{
        location = sh->getAttribLocation("a_uv");
        if(location != -1)
        {
            glEnableVertexAttribArray(location);
            glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, 0, &uvs[0] );
        }
    }
    
	if (colors.size())
	{
        location = sh->getAttribLocation("a_color");
        if(location != -1)
        {
            glEnableVertexAttribArray(location);
            glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, 0, &colors[0] );
        }
    }

	if(tangents.size()){
		location = sh->getAttribLocation("a_tangent");
		if(location != -1){
			glEnableVertexAttribArray(location);
            glVertexAttribPointer(location, 3, GL_FLOAT, GL_TRUE, 0, &normals[0] );
		}
	
	}
    
	glDrawArrays(primitive, 0, (GLsizei)vertices.size() );

	glDisableVertexAttribArray(sh->getAttribLocation("a_vertex"));

	if(sh->getAttribLocation("a_normal") != -1)
		glDisableVertexAttribArray(sh->getAttribLocation("a_normal"));
	
	if(sh->getAttribLocation("a_uv") != -1)
		glDisableVertexAttribArray(sh->getAttribLocation("a_uv"));

	if(sh->getAttribLocation("a_color") != -1)
		glDisableVertexAttribArray(sh->getAttribLocation("a_color"));

	if(sh->getAttribLocation("a_tangent") != -1)
		glDisableVertexAttribArray(sh->getAttribLocation("a_tangent"));
    
}

void Mesh::upload(){

	glGenBuffers( 1, &vertices_vbo_id );
	glBindBuffer( GL_ARRAY_BUFFER, vertices_vbo_id ); //bind the handler
	glBufferData( GL_ARRAY_BUFFER, vertices.size()*3*sizeof(float), &vertices[0], GL_STATIC_DRAW ); //upload data

	if(normals.size()){
		glGenBuffers( 1, &normals_vbo_id );
		glBindBuffer( GL_ARRAY_BUFFER, normals_vbo_id ); //bind the handler
		glBufferData( GL_ARRAY_BUFFER, normals.size()*3*sizeof(float), &normals[0], GL_STATIC_DRAW ); //upload data
	}

	if(uvs.size()){
		glGenBuffers( 1, &uvs_vbo_id );
		glBindBuffer( GL_ARRAY_BUFFER, uvs_vbo_id ); //bind the handler
		glBufferData( GL_ARRAY_BUFFER, uvs.size()*2*sizeof(float), &uvs[0], GL_STATIC_DRAW ); //upload data
	}

	if(colors.size()){
		glGenBuffers( 1, &colors_vbo_id );
		glBindBuffer( GL_ARRAY_BUFFER, colors_vbo_id ); //bind the handler
		glBufferData( GL_ARRAY_BUFFER, colors.size()*4*sizeof(float), &colors[0], GL_STATIC_DRAW ); //upload data
	}

	if(tangents.size()){
		glGenBuffers( 1, &tangents_vbo_id );
		glBindBuffer( GL_ARRAY_BUFFER, tangents_vbo_id ); //bind the handler
		glBufferData( GL_ARRAY_BUFFER, tangents.size()*3*sizeof(float), &tangents[0], GL_STATIC_DRAW ); //upload data
	}

	VBOLoaded = true;

}

void Mesh::renderVBO( int primitive, Shader* sh )
{
	if(!sh || !sh->compiled)
        return render(primitive);
    
	//assert(vertices.size() && "No vertices in this mesh");
	if(vertices.size() == 0)
		return ;

	if(!VBOLoaded)
		upload();

    int location = sh->getAttribLocation("a_vertex");
	assert(location != -1 && "No a_vertex found in shader");

	if(location == -1)
		return;

	if(vertices_vbo_id!= 0){
    glEnableVertexAttribArray(location);
	glBindBuffer( GL_ARRAY_BUFFER, vertices_vbo_id ); //bind the buffer
    glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

    if (normals_vbo_id != 0)
    {
        location = sh->getAttribLocation("a_normal");

        if(location != -1)
        {
			glEnableVertexAttribArray(location);
			glBindBuffer( GL_ARRAY_BUFFER, normals_vbo_id ); //bind the buffer
			glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 0, 0);
        }
    }
    
	if (uvs_vbo_id != 0)
	{
        location = sh->getAttribLocation("a_uv");
        if(location != -1)
        {
			glEnableVertexAttribArray(location);
			glBindBuffer( GL_ARRAY_BUFFER, uvs_vbo_id ); //bind the buffer
			glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, 0, 0);
        }
    }
    
	if (colors_vbo_id != 0)
	{
        location = sh->getAttribLocation("a_color");
        if(location != -1)
        {
            glEnableVertexAttribArray(location);
            glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, 0, &colors[0] );
        }
    }
	

	if(tangents_vbo_id != 0){
		location = sh->getAttribLocation("a_tangent");
		if(location != -1){

			glEnableVertexAttribArray(location);
			glBindBuffer( GL_ARRAY_BUFFER, tangents_vbo_id ); //bind the buffer
			glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}
	
	}
	glDrawArrays(primitive, 0, (GLsizei)vertices.size() );

	glBindBuffer( GL_ARRAY_BUFFER, 0 ); //bind the buffer

	glDisableVertexAttribArray(sh->getAttribLocation("a_vertex"));

	if(sh->getAttribLocation("a_normal") != -1)
		glDisableVertexAttribArray(sh->getAttribLocation("a_normal"));
	
	if(sh->getAttribLocation("a_uv") != -1)
		glDisableVertexAttribArray(sh->getAttribLocation("a_uv"));

	if(sh->getAttribLocation("a_color") != -1)
		glDisableVertexAttribArray(sh->getAttribLocation("a_color"));

	if(sh->getAttribLocation("a_tangent") != -1)
		glDisableVertexAttribArray(sh->getAttribLocation("a_tangent"));

}

void Mesh::createQuad(float center_x, float center_y, float w, float h, bool flip_uvs)
{
	vertices.clear();
	normals.clear();
	uvs.clear();
	colors.clear();
	tangents.clear();

	//create six vertices (3 for upperleft triangle and 3 for lowerright)

	vertices.push_back(Vector3(center_x + w, center_y + h, 0.0f));
	vertices.push_back(Vector3(center_x, center_y, 0.0f));
	vertices.push_back(Vector3(center_x + w, center_y, 0.0f));
	vertices.push_back(Vector3(center_x, center_y + h, 0.0f));
	vertices.push_back(Vector3(center_x, center_y, 0.0f));
	vertices.push_back(Vector3(center_x + w, center_y + h, 0.0f));

	//texture coordinates

	uvs.push_back(Vector2(1.0f, flip_uvs ? 0.0f : 1.0f));
	uvs.push_back(Vector2(0.0f, flip_uvs ? 1.0f : 0.0f));
	uvs.push_back(Vector2(1.0f, flip_uvs ? 1.0f : 0.0f));
	uvs.push_back(Vector2(0.0f, flip_uvs ? 0.0f : 1.0f));
	uvs.push_back(Vector2(0.0f, flip_uvs ? 1.0f : 0.0f));
	uvs.push_back(Vector2(1.0f, flip_uvs ? 0.0f : 1.0f));

	//all of them have the same normal
	normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
	normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
	normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
	normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
	normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
	normals.push_back(Vector3(0.0f, 0.0f, 1.0f));

}

void Mesh::createQuad(Vector2 pos, Vector2 size, Vector2 uvmapf, Vector2 uvmape, bool flip_uvs)
{
	//vertices.clear();
	//normals.clear();
	//uvs.clear();
	//colors.clear();

	//create six vertices (3 for upperleft triangle and 3 for lowerright)

	vertices.push_back(Vector3(pos.x + size.x, pos.y + size.y, 0.0f));
	vertices.push_back(Vector3(pos.x, pos.y, 0.0f));
	vertices.push_back(Vector3(pos.x + size.x, pos.y, 0.0f));
	vertices.push_back(Vector3(pos.x, pos.y + size.y, 0.0f));
	vertices.push_back(Vector3(pos.x, pos.y, 0.0f));
	vertices.push_back(Vector3(pos.x + size.x, pos.y + size.y, 0.0f));

	//texture coordinates
	/*	uvs.push_back(Vector2(1.0f, 1.0f));
	uvs.push_back(Vector2(0.0f, 0.0f));
	uvs.push_back(Vector2(1.0f, 0.0f));
	uvs.push_back(Vector2(0.0f, 1.0f));
	uvs.push_back(Vector2(0.0f, 0.0f));
	uvs.push_back(Vector2(1.0f, 1.0f));*/


	uvs.push_back(uvmape);
	uvs.push_back(uvmapf);
	uvs.push_back(Vector2(uvmape.x, uvmapf.y));
	uvs.push_back(Vector2(uvmapf.x, uvmape.y));
	uvs.push_back(uvmapf);
	uvs.push_back(uvmape);


	//all of them have the same normal
	normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
	normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
	normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
	normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
	normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
	normals.push_back(Vector3(0.0f, 0.0f, 1.0f));

	//Mesh::MeshManager.insert(std::pair<std::string, Mesh*>("2DQuad", this));
}


void Mesh::createPlane(float size)
{
	//vertices.clear();
	//normals.clear();
	//uvs.clear();
	//colors.clear();

	//create six vertices (3 for upperleft triangle and 3 for lowerright)

	vertices.push_back( Vector3(size,0,size) );
	vertices.push_back( Vector3(size,0,-size) );
	vertices.push_back( Vector3(-size,0,-size) );
	vertices.push_back( Vector3(-size,0,size) );
	vertices.push_back( Vector3(size,0,size) );
	vertices.push_back( Vector3(-size,0,-size) );

	//all of them have the same normal
	normals.push_back( Vector3(0,1,0) );
	normals.push_back( Vector3(0,1,0) );
	normals.push_back( Vector3(0,1,0) );
	normals.push_back( Vector3(0,1,0) );
	normals.push_back( Vector3(0,1,0) );
	normals.push_back( Vector3(0,1,0) );

	//texture coordinates
	uvs.push_back( Vector2(1,1) );
	uvs.push_back( Vector2(1,0) );
	uvs.push_back( Vector2(0,0) );
	uvs.push_back( Vector2(0,1) );
	uvs.push_back( Vector2(1,1) );
	uvs.push_back( Vector2(0,0) );

	//Mesh::MeshManager.insert(std::pair<std::string, Mesh*>("2Dplane", this)); 

}

bool Mesh::loadASE(const char* filename){

	TextParser Parser;
	if(!Parser.create(filename)){
		std::cout << filename << " not found." << std::endl;
		return false;
	} 
	std::cout << "Reading " << filename << std::endl;
	int i = 0;
	
	char*comparar; 
	while (!Parser.eof()){
		comparar = Parser.getword(); 
		if (comparar == NULL)
		{
			break;
		}
	}
	std::cout << "Numero d'objectes: " << i << "." << std::endl;

	Parser.reset();

	Parser.seek("*MESH_NUMVERTEX"); 
	unsigned int numVertex = Parser.getint(); 

	Parser.seek("*MESH_NUMFACES"); 
	unsigned int numFaces = Parser.getint();

	Vector3 vertexIndexed; 
	Vector3 * indexedVertices = (Vector3*) malloc(numVertex * sizeof(Vector3));

	for (unsigned int i = 0; i < numVertex; i++)
	{
		Parser.seek("*MESH_VERTEX"); 
		Parser.getword();
		vertexIndexed.x = Parser.getfloat(); 
		vertexIndexed.y = Parser.getfloat();
		vertexIndexed.z = Parser.getfloat();
		indexedVertices[i] = vertexIndexed; 
	}

	vertices.resize(numFaces*3);
	int ind = 0;
	for(unsigned int i = 0; i < numFaces; i++){ 
		Parser.seek("*MESH_FACE"); 
			Parser.seek("A:"); vertices[ind] = indexedVertices[Parser.getint()]; 
			ind++;
			Parser.seek("B:"); vertices[ind] = indexedVertices[Parser.getint()];
			ind++;
			Parser.seek("C:"); vertices[ind] = indexedVertices[Parser.getint()];
			ind++;
	}

	free(indexedVertices);

	//TEXTURE VERTEX
	Parser.seek("*MESH_NUMTVERTEX");
	unsigned int numTVertex = Parser.getint(); 

	Vector2 uv; 
	Vector2 * indexed_uv = (Vector2*) malloc(numTVertex * sizeof(Vector2));

	for(unsigned int i = 0; i < numTVertex; i++){
		Parser.seek("*MESH_TVERT");
		Parser.getword();
			uv.x = Parser.getfloat();
			uv.y = Parser.getfloat(); 
			indexed_uv[i] = uv; 
	}

	Parser.seek("*MESH_NUMTVFACES");
	unsigned int numTFaces = Parser.getint();

	uvs.resize(numTFaces*3);

	ind = 0;
	for(unsigned int i = 0; i < numTFaces; i++){
		Parser.seek("*MESH_TFACE");
		Parser.getword();
		uvs[ind] = (indexed_uv[Parser.getint()]);
		ind++;
		uvs[ind] = (indexed_uv[Parser.getint()]);
		ind ++;
		uvs[ind] = (indexed_uv[Parser.getint()]);
		ind++;
	}

	free(indexed_uv);

	//NORMALES

	Vector3 normal;
	normals.resize(numFaces*3);
	for(unsigned int i = 0; i < (numFaces*3); i++){
		Parser.seek("*MESH_VERTEXNORMAL");
		Parser.getword();
		normal.x = Parser.getfloat();
		normal.y = Parser.getfloat();
		normal.z = Parser.getfloat();
		normals[i] = normal;
	}

	fillTangents();

	/*
	for(int i = 0; i<tangents.size(); i++){
		if(i%3 == 0)
			std::cout << std::endl;
		
		std::cout << "Tangent[" << i << "]: " << tangents[i].x << "  " << tangents[i].y << "  " << tangents[i].z << std::endl;
	}*/
	
	std::string str = std::string(filename);
	Mesh::MeshManager.insert(std::pair<std::string, Mesh*>(str, this)); 

	return true; //Deberíamos meter condiciones del palo, si no se encuentra el archivo pon este bool a false, y devolver eso
				 //Printando mensajito de error. 
}

bool Mesh::loadOBJ(const char* filename)
{
    struct stat stbuffer;
 
    std::cout << "Loading Mesh: " << filename << std::endl;
 
    FILE* f = fopen(filename,"rb");
    if (f == NULL)
    {
        std::cerr << "File not found: " << filename << std::endl;
        return false;
    }
 
    stat(filename,&stbuffer);
 
    unsigned int size = stbuffer.st_size;
    char* data = new char[size+1];
    fread(data,size,1,f);
    fclose(f);
    data[size] = 0;
 
    char* pos = data;
    char line[255];
    int i = 0;
 
    std::vector<Vector3> indexed_positions;
    std::vector<Vector3> indexed_normals;
    std::vector<Vector2> indexed_uvs;
 
    const float max_float = 10000000;
    const float min_float = -10000000;
 
    unsigned int vertex_i = 0;
 
    //parse file
    while(*pos != 0)
    {
        if (*pos == '\n') pos++;
        if (*pos == '\r') pos++;
 
        //read one line
        i = 0;
        while(i < 255 && pos[i] != '\n' && pos[i] != '\r' && pos[i] != 0) i++;
        memcpy(line,pos,i);
        line[i] = 0;
        pos = pos + i;
 
        //std::cout << "Line: \"" << line << "\"" << std::endl;
        if (*line == '#' || *line == 0) continue; //comment
 
        //tokenize line
        std::vector<std::string> tokens = tokenize(line," ");
 
        if (tokens.empty()) continue;
 
        if (tokens[0] == "v" && tokens.size() == 4)
        {
            Vector3 v( atof(tokens[1].c_str()), atof(tokens[2].c_str()), atof(tokens[3].c_str()) );
            indexed_positions.push_back(v);
        }
        else if (tokens[0] == "vt" && tokens.size() == 4)
        {
            Vector2 v( atof(tokens[1].c_str()), atof(tokens[2].c_str()) );
            indexed_uvs.push_back(v);
        }
        else if (tokens[0] == "vn" && tokens.size() == 4)
        {
            Vector3 v( atof(tokens[1].c_str()), atof(tokens[2].c_str()), atof(tokens[3].c_str()) );
            indexed_normals.push_back(v);
        }
        else if (tokens[0] == "s") //surface? it appears one time before the faces
        {
            //process mesh
            if (uvs.size() == 0 && indexed_uvs.size() )
                uvs.resize(1);
        }
        else if (tokens[0] == "f" && tokens.size() >= 4)
        {
            Vector3 v1,v2,v3;
            v1 = parseVector3( tokens[1].c_str(), '/' );
 
            for (int iPoly = 2; iPoly < tokens.size() - 1; iPoly++)
            {
                v2 = parseVector3( tokens[iPoly].c_str(), '/' );
                v3 = parseVector3( tokens[iPoly+1].c_str(), '/' );
 
                vertices.push_back( indexed_positions[ unsigned int(v1.x) -1 ] );
                vertices.push_back( indexed_positions[ unsigned int(v2.x) -1] );
                vertices.push_back( indexed_positions[ unsigned int(v3.x) -1] );
                //triangles.push_back( VECTOR_INDICES_TYPE(vertex_i, vertex_i+1, vertex_i+2) ); //not needed
                vertex_i += 3;
 
                if (indexed_uvs.size() > 0)
                {
                    uvs.push_back( indexed_uvs[ unsigned int(v1.y) -1] );
                    uvs.push_back( indexed_uvs[ unsigned int(v2.y) -1] );
                    uvs.push_back( indexed_uvs[ unsigned int(v3.y) -1] );
                }
 
                if (indexed_normals.size() > 0)
                {
                    normals.push_back( indexed_normals[ unsigned int(v1.z) -1] );
                    normals.push_back( indexed_normals[ unsigned int(v2.z) -1] );
                    normals.push_back( indexed_normals[ unsigned int(v3.z) -1] );
                }
            }
        }
    }
 
    delete data;
 
	fillTangents();

	std::string str = std::string(filename);
	Mesh::MeshManager.insert(std::pair<std::string, Mesh*>(str, this)); 

    return true;
}
 
 
std::vector<std::string> tokenize(const std::string& source, const char* delimiters, bool process_strings )
{
    std::vector<std::string> tokens;
 
    std::string str;
    char del_size = strlen(delimiters);
    const char* pos = source.c_str();
    char in_string = 0;
    int i = 0;
    while(*pos != 0)
    {
        bool split = false;
 
        if (!process_strings || (process_strings && in_string == 0))
        {
            for (i = 0; i < del_size && *pos != delimiters[i]; i++);
            if (i != del_size) split = true;
        }
 
        if (process_strings && (*pos == '\"' || *pos == '\'') )
        { 
            if (!str.empty() && in_string == 0) //some chars remaining
            {
                tokens.push_back(str);
                str.clear();
            }
             
            in_string = (in_string != 0 ? 0 : *pos );
            if (in_string == 0) 
            {
                str += *pos;
                split = true;
            }
        }
 
        if (split)
        {
            if (!str.empty())
            {
                tokens.push_back(str);
                str.clear();
            }
        }
        else
            str += *pos;
        pos++;
    }
    if (!str.empty())
        tokens.push_back(str);
    return tokens;
}
 
 
 
Vector2 parseVector2(const char* text)
{
    int pos = 0;
    char num[255];
    const char* start = text;
    const char* current = text;
    Vector2 result;
 
    while(1)
    {
        if (*current == ',' || (*current == '\0' && current != text) )
        {
            strncpy(num, start, current - start);
            num[current - start] = '\0';
            start = current + 1;
            switch(pos)
            {
                case 0: result.x = (float)atof(num); break;
                case 1: result.y = (float)atof(num); break;
                default: return result; break;
            }
            ++pos;
            if (*current == '\0')
                break;
        }
 
        ++current;
    }
    return result;
}
 
Vector3 parseVector3(const char* text, const char separator)
{
    int pos = 0;
    char num[255];
    const char* start = text;
    const char* current = text;
    Vector3 result;
 
    while(1)
    {
        if (*current == separator || (*current == '\0' && current != text) )
        {
            strncpy(num, start, current - start);
            num[current - start] = '\0';
            start = current + 1;
            if (num[0] != 'x') //¿?
                switch(pos)
                {
                    case 0: result.x = (float)atof(num); break;
                    case 1: result.y = (float)atof(num); break;
                    case 2: result.z = (float)atof(num); break;
                    default: return result; break;
                }
 
            ++pos;
            if (*current == '\0')
                break;
        }
 
        ++current;
    }
 
    return result;
};


void Mesh::fillTangents(){

	tangents.resize(vertices.size());
	for (int i = 0; i < vertices.size(); i += 3)
	{

		Vector3 v0 = vertices[i];
		Vector3 v1 = vertices[i+1];
		Vector3 v2 = vertices[i+2];

		Vector3 Edge1 = v1 - v0;
		Vector3 Edge2 = v2 - v0;

		Vector2 uv0 = uvs[i];
		Vector2 uv1 = uvs[i+1];
		Vector2 uv2 = uvs[i+2];

		float DeltaU1 = v1.x - v0.x;
		float DeltaV1 = v1.y - v0.y;
		float DeltaU2 = v2.x - v0.x;
		float DeltaV2 = v2.y - v0.y;

		float divisor = (DeltaU1 * DeltaV2 - DeltaU2 * DeltaV1);

		float f = 1.0f;

		if(divisor != 0){
			f = 1.0f / float(divisor);}

		Vector3 Tangent;

		Tangent.x = f * (DeltaV2 * Edge1.x - DeltaV1 * Edge2.x);
		Tangent.y = f * (DeltaV2 * Edge1.y - DeltaV1 * Edge2.y);
		Tangent.z = f * (DeltaV2 * Edge1.z - DeltaV1 * Edge2.z);
		
		Tangent.normalize();

		tangents[i] = tangents[i] + Tangent;
		tangents[i+1] = tangents[i+1] + Tangent;
		tangents[i+2] = tangents[i+2] + Tangent;
	}
}
/*
void Mesh::fillTangents(){

	tangents.resize(vertices.size());
	for (int i = 0; i < vertices.size(); i += 3)
	{
		Vector3 t; 
		Vector3 b; 
		Vector3 c1 = normals[i].cross(Vector3(0.0,0.0,1.0));//cross(vertex_normal, vec3(0.0, 0.0, 1.0)); 
		Vector3 c2 = normals[i].cross(Vector3(0.0,1.0,0.0));//cross(vertex_normal, vec3(0.0, 1.0, 0.0)); 

		if (c1.length() > c2.length())
		  t = c1;	
		else
		  t = c2;	

		tangents[i] = t.normalize();
	}
}*/
bool Mesh::writeBIN(const char* filename){

	FILE* binFile = fopen(filename, "wb");
	if(binFile != NULL){
		std::cout << "Writing " << filename << "." << std::endl;
		unsigned int longg = vertices.size();
		fwrite(&longg, sizeof(unsigned int), 1, binFile);
		fwrite(&vertices[0], sizeof(Vector3), vertices.size(), binFile);
		fwrite(&normals[0], sizeof(Vector3), normals.size(), binFile);
		fwrite(&tangents[0], sizeof(Vector3), tangents.size(), binFile);
		fwrite(&uvs[0], sizeof(Vector2), uvs.size(), binFile);
		fclose(binFile);
	return true;
	}
	std::cout << "An error has ocurred writing " << filename << "." << std::endl;
	return false;
}

bool Mesh::loadBIN(const char* filename){
	FILE* binFile = fopen(filename, "rb");

	if(binFile != NULL){
		//std::cout << "Reading " << filename << "." << std::endl;
		unsigned int vectorSize;

		fread(&vectorSize, sizeof(unsigned int), 1, binFile);
		vertices.resize(vectorSize);
		normals.resize(vectorSize);
		tangents.resize(vectorSize);
		uvs.resize(vectorSize);

		fread(&vertices[0], sizeof(Vector3), vectorSize, binFile);
		fread(&normals[0], sizeof(Vector3), vectorSize, binFile);
		fread(&tangents[0], sizeof(Vector3), vectorSize, binFile);
		fread(&uvs[0], sizeof(Vector2), vectorSize, binFile);

		fclose(binFile);

		std::string str = std::string(filename);
		Mesh::MeshManager.insert(std::pair<std::string, Mesh*>(str, this)); 

		calculateRadius();

	return true;
	}
	std::cout << "An error has ocurred loading " << filename << "." << std::endl;

	return false;
}
/*
void Mesh::calculateRadius(){
	Vector3 average = Vector3(0,0,0);

	float distance = 0.0;

	for (unsigned int i = 0; i < vertices.size(); i++)
	{
		average.x += vertices[i].x;
		average.y += vertices[i].y;
		average.z += vertices[i].z;
	}

	average.x /= (float)vertices.size();
	average.y /= (float)vertices.size();
	average.z /= (float)vertices.size();

	for (unsigned int i = 0; i < vertices.size(); i++)
	{
		float aux = average.distance(vertices[i]);
		if(aux > distance)
			distance = aux;
	}

	this->radius = distance;
	std::cout << "radius " << this->radius << std::endl;
}
*/

void Mesh::calculateRadius(){

	Vector3 max, min;
	max = vertices[0];
	min = vertices[0];

		for(unsigned int i = 1; i < vertices.size(); i++){
			//------------------MAXS--------------------
			if(vertices[i].x > max.x)
				max.x = vertices[i].x;
			if(vertices[i].y > max.y)
				max.y = vertices[i].y;
			if(vertices[i].z > max.z)
				max.z = vertices[i].z;

			//-----------------MINS---------------------
			if(vertices[i].x < min.x)
				min.x = vertices[i].x;
			if(vertices[i].y < min.y)
				min.y = vertices[i].y;
			if(vertices[i].z < min.z)
				min.z = vertices[i].z;
		}

	Vector3 centerO = (max+min);
	centerO.x /= 2.0f;
	centerO.y /= 2.0f;
	centerO.z /= 2.0f;

	Vector3 halfSize;

	halfSize = max - centerO;

	this->radius = halfSize.length();
	
	this->center = centerO;

}

/*
void Mesh::buildCollisionTree(){

	assert( collision_model == NULL);

	collision_model = newCollisionModel3D();

	collision_model->setTriangleNumber(vertices.size() / 3);

	for(int i = 0; i < vertices.size(); i = i + 3){
		collision_model->addTriangle(vertices[i].v,vertices[i+1].v,vertices[i+2].v);
	}
	 
	collision_model->finalize();
}
*/