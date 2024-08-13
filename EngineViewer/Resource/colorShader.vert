#version 460 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec4 vertexColor;
layout(location = 2) in vec3 normalVector;
layout(location = 3) in vec2 texCoord;

out vec4 outVertexColor;
out vec3 outWorldPosition;
out vec3 posProj;
out vec2 outTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 invModel;

void main() {
    outVertexColor = vertexColor;
    outTexCoord = texCoord;
    outWorldPosition = (model * vec4(vertexPosition, 1.0f)).xyz;
    gl_Position = projection * view * model * vec4(vertexPosition, 1.0f);
    posProj = gl_Position.xyz;
}