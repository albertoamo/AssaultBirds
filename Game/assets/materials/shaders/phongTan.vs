//global variables from the CPU
attribute vec3 a_vertex;
attribute vec3 a_normal;
attribute vec2 a_uv;
attribute vec4 a_color;
attribute vec3 a_tangent;

//Matrices
uniform mat4 mvp;
uniform mat4 model_matrix;
uniform mat4 normal_matrix;

//Atributos Camara y luz
uniform vec3 eye_pos;	 //POSICION DEL EYE

uniform vec3 color;
uniform vec3 light_pos;    //POSICION DE LA LUZ
uniform float id;
uniform float is;

//vars to pass to the pixel shader
varying vec3 v_position;
varying vec3 v_world_position;
varying vec2 v_uv;
varying vec4 v_color;
varying vec3 v_normal;
varying vec3 v_tangent;
varying vec3 v_bitangent;

varying vec4 projCoords;
uniform mat4 texGen;

uniform mat4 shadowMat;
varying vec4 shadowCoords;

void main()
{	
	v_uv = a_uv;
	v_color = a_color;
	v_position = a_vertex;
	v_normal = (normal_matrix * vec4(a_normal, 0.0) ).xyz;
	v_tangent = (normal_matrix * vec4(a_tangent, 0.0) ).xyz;
	v_bitangent = cross(v_normal, v_tangent);
	v_world_position = (model_matrix * vec4( a_vertex, 1.0) ).xyz;

	#ifdef REFLECTION
		projCoords = texGen * vec4(v_world_position,1.0);
	#endif

	shadowCoords = shadowMat * vec4(v_world_position,1.0);

	//project the vertex by the model view projection 
	gl_Position = mvp * vec4( a_vertex, 1.0 );
}