#version 460 core

in vec4 outVertexColor;
in vec3 posWorld;
in vec3 posProj;
in vec3 normal;
in vec2 outTexCoord;
out vec4 FragColor;

struct Light
{
    vec3 direction;
    float dummy1;
    vec3 position;
    float dummy2;
};

struct Material
{
    vec3 ambient; // ��ü�� ���� ����
    float ambientFactor; // ��ü �������� ����
    vec3 diffuse; // �л걤 ����(��ü�� �ݻ��ϴ� �� ����)
    float shininess; // ����
    vec3 specular; // �ݻ籤 ����(���� �ݻ�Ǵ� �� ����)
    float specularFactor; // �ݻ籤 ����
};

layout (std140, binding=1) uniform PixelConstBuffer
{
    vec3 camWorld;
    bool useTexture;
    Light light;
    Material material;
};

void main(){
    // 1. ambient color
    vec3 ambientColor = material.ambient * material.ambientFactor;
    // 2. diffuse color
    vec3 toLight = normalize(camWorld - posWorld);
    float lambertFactor = max(dot(toLight, normal), 0);
    vec3 diffuseColor = lambertFactor * material.diffuse * material.shininess;
    // 3. sepcular color
    FragColor = vec4(ambientColor + diffuseColor, 1);
}