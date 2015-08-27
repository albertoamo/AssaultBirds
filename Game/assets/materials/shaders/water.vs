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
uniform mat4 texGen;

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
varying vec4 projCoords;

uniform float time;
float waveWidth = 0.6;
float waveHeight = 1.0;

void main()
{	

	v_normal = (normal_matrix * vec4(a_normal, 0.0) ).xyz;
	v_position = a_vertex;
	v_world_position = (model_matrix * vec4( a_vertex, 1.0) ).xyz;
	v_color = a_color;
	v_uv = a_uv;

	projCoords = texGen * vec4(v_world_position,1.0);

	vec3 v = a_vertex;
	//v.y = sin(waveWidth * v.x + time/100000) * cos(waveWidth * v.y + time/100000) * waveHeight*time/100000;

 	gl_Position = mvp * vec4( v, 1.0 );

}