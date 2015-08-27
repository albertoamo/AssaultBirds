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

//vars to pass to the pixel shader
varying vec3 v_position;
varying vec3 v_world_position;
varying vec2 v_uv;
varying vec4 v_color;
varying vec3 v_normal;

void main()
{	

	v_normal = (normal_matrix * vec4(a_normal, 0.0) ).xyz;
	v_position = a_vertex;
	v_world_position = (model_matrix * vec4( a_vertex, 1.0) ).xyz;

	v_color = a_color;
	v_uv = a_uv;

	gl_Position = mvp * vec4( a_vertex, 1.0 );
}