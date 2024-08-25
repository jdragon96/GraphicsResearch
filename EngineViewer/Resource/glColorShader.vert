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
����Ʈ �ɼ�
- shared layout �̶�� �Ҹ��� uniform �޸� layout�� ���
- ���� program���� ������
- 

std140
- ���� Ÿ�Կ� ���� �������� offset�� ����� �޸� ���̾ƿ� ����
- �� ������ base alignment�� ����
- ��Į��: N
- ����: 2N �Ǵ� 4N
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