#version 330 core

in vec4 currPos; in vec4 prevPos;

void main() {
    vec3 cPos = currPos.xyz / currPos.w;
    vec3 pPos = prevPos.xyz / prevPos.w;
    vec3 vel  = cPos - pPos;

    gl_FragColor = vec4(vel, 1);
}