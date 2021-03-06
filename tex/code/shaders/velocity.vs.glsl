#version 330 core

in vec3 position; out vec4 currPos; out vec4 prevPos;

uniform mat4 currMVP; uniform mat4 prevMVP; uniform mat4 proj;

void main(){
	vec4 pos = vec4(position, 1.0);
	currPos = currMVP * pos;
	prevPos = prevMVP * pos;

	gl_Position = proj * currPos;
}
