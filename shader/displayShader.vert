#version 330 core

in vec4 position;
in vec3 normal;
in vec3 colour;

out  float hight;
out  vec3 col;
out  vec3 nor;
out  vec3 eyeDir;
out  vec4 shadowCoord;

uniform mat4 mvpMatrix;
uniform vec3 cameraPos;
uniform mat4 shadowMapMatrix;

void main() {
	hight = position.z / 50;
	col = colour;
	nor = normal;

	//direction of the eye (from the camera to the vertex, because reflexion of lightDir is from the light to the fragment)
	eyeDir = normalize(position.xyz - cameraPos); 

	//coordinates of the vertex for the shadow map
	shadowCoord = shadowMapMatrix * position;
	
	//Output position of the vertex
	gl_Position = mvpMatrix * position;
}
