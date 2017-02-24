#version 330 core

in vec4 position;

uniform mat4 matrix;

void main(void){

	//output: the position of the vertex for the map
	gl_Position =  matrix * position;
}