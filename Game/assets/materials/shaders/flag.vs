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
uniform float time;

//vars to pass to the pixel shader
varying vec3 v_position;
varying vec3 v_world_position;
varying vec2 v_uv;
varying vec4 v_color;
varying vec3 v_normal;

#define PI 3.14159265

//Flag macro definitions
#define overallScale	5

#define w1scale		1.73
#define w1angle		(4 * (PI/180))
#define w1speed		0.9
#define w1amp		0

#define w2scale		0.75
#define w2angle		(-10 * (PI/180))
#define w2speed		1.5
#define w2amp		0

#define w3scale		0.31
#define w3angle		(15 * (PI/180))
#define w3speed		6.2
#define w3amp		0.05

void main()
{	

	v_normal = (normal_matrix * vec4(a_normal, 0.0) ).xyz;

	v_position = a_vertex;
	v_world_position = (model_matrix * vec4( a_vertex, 1.0) ).xyz;

	//store the color in the varying var to use it from the pixel shader
	v_color = a_color;

	float	U =			a_uv.x;
	float	V =			a_uv.y;
	#define	w1scaleU	2*PI/w1scale * cos(w1angle)
	#define	w1scaleV	2*PI/w1scale * sin(w1angle)
	float	w1 = (w1scaleU * U) + (w1scaleV * V) + (2 * PI * time/100000 * w1speed);
	#define	w2scaleU	2*PI/w2scale * cos(w2angle)
	#define	w2scaleV	2*PI/w2scale * sin(w2angle)
	float	w2 = (w2scaleU * U) + (w2scaleV * V) + (2 * PI * time/100000 * w2speed);
	#define	w3scaleU	2*PI/w3scale * cos(w3angle)
	#define	w3scaleV	2*PI/w3scale * sin(w3angle)
	float	w3 = (w3scaleU * U) + (w3scaleV * V) + (2 * PI * time/100000 * w3speed);

	float offsetAmount = overallScale *v_color.a * ( (w1amp*sin(w1)) + (w2amp*sin(w2)) + (w3amp*sin(w3)) );

	vec4 returnPos;
	returnPos.xyz = v_position.xyz + (vec3(offsetAmount,offsetAmount,offsetAmount) * v_normal);
	returnPos.w = 1.0;

	//returnPos = vec4(returnPos.x,a_vertex.y,returnPos.z,1.0);
	returnPos.y = returnPos.y - cos(0.3 * v_position.x) - 2;
	returnPos.z = returnPos.z + cos(0.5 * v_position.x) + 1;
	v_uv = a_uv;

	gl_Position = mvp * vec4( returnPos.xyz, 1.0 );
}