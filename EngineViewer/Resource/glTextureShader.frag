#version 460 core

in vec4 outVertexColor;
in vec3 posProj;
in vec3 outNormalVector;
in vec2 outTexCoord;
out vec4 FragColor;

uniform sampler2D texture0;

void main(){
    FragColor = texture(texture0, outTexCoord);
}