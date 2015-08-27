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

//vars to pass to the pixel shader
varying vec3 v_position;
varying vec3 v_world_position;

void main()
{	

	v_position = a_vertex;
	v_world_position = (model_matrix * vec4( a_vertex, 1.0) ).xyz;

	gl_Position = mvp * vec4( a_vertex, 1.0 );
}