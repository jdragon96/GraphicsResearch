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
    vec3 ambient; // 물체의 고유 색상
    float ambientFactor; // 물체 고유색상 강도
    vec3 diffuse; // 분산광 강도(물체가 반사하는 빛 색상)
    float shininess; // 더미
    vec3 specular; // 반사광 색상(눈에 반사되는 빛 색상)
    float specularFactor; // 반사광 강도
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