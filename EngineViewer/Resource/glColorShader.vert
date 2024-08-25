#version 460 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec4 vertexColor;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec3 normalVector;

out vec4 outVertexColor;
out vec3 posWorld;
out vec3 posProj;
out vec3 normal;
out vec2 outTexCoord;

layout (std140, binding=0) uniform VertexConstBuffer
{
    mat4 model;         // 0
    mat4 view;      // 64
    mat4 projection;    //  128
    mat4 invModel;  // 192
};

/*
디폴트 옵션
- shared layout 이라고 불리는 uniform 메모리 layout을 사용
- 여러 program에서 공유됨
- 

std140
- 변수 타입에 따라 저마다의 offset을 명시해 메모리 레이아웃 설정
- 각 변수는 base alignment를 갖음
- 스칼라: N
- 벡터: 2N 또는 4N
- 
*/
void main() {
    outVertexColor = vertexColor;
    outTexCoord = texCoord;
    posWorld = (model * vec4(vertexPosition, 1.0f)).xyz;
    gl_Position = projection * view * model * vec4(vertexPosition, 1.0f);
    posProj = gl_Position.xyz;
    normal = (invModel * vec4(normalVector, 1.f)).xyz;
}