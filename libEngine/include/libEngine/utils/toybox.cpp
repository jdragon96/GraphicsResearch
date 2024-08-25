#include "libEngine/utils/toybox.h"

namespace libEngine
{
MeshData Toybox::MakeCube(float size, float r, float g, float b)
{
  MeshData mesh;

  mesh.vertices.push_back(Vertex(-size, size, -size, r, g, b, 1.f, 0.f, 1.f, 0.f, 0.f, 0.f));
  mesh.vertices.push_back(Vertex(-size, size, size, r, g, b, 1.f, 0.f, 1.f, 0.f, 1.f, 0.f));
  mesh.vertices.push_back(Vertex(size, size, size, r, g, b, 1.f, 0.f, 1.f, 0.f, 1.f, 1.f));
  mesh.vertices.push_back(Vertex(size, size, -size, r, g, b, 1.f, 0.f, 1.f, 0.f, 0.f, 1.f));

  mesh.vertices.push_back(Vertex(-size, -size, -size, r, g, b, 1.f, 0.f, -1.f, 0.f, 0.f, 0.f));
  mesh.vertices.push_back(Vertex(size, -size, -size, r, g, b, 1.f, 0.f, -1.f, 0.f, 1.f, 0.f));
  mesh.vertices.push_back(Vertex(size, -size, size, r, g, b, 1.f, 0.f, -1.f, 0.f, 1.f, 1.f));
  mesh.vertices.push_back(Vertex(-size, -size, size, r, g, b, 1.f, 0.f, -1.f, 0.f, 0.f, 1.f));

  mesh.vertices.push_back(Vertex(-size, -size, -size, r, g, b, 1.f, 0.f, 0.f, -1.f, 0.f, 0.f));
  mesh.vertices.push_back(Vertex(-size, size, -size, r, g, b, 1.f, 0.f, 0.f, -1.f, 1.f, 0.f));
  mesh.vertices.push_back(Vertex(size, size, -size, r, g, b, 1.f, 0.f, 0.f, -1.f, 1.f, 1.f));
  mesh.vertices.push_back(Vertex(size, -size, -size, r, g, b, 1.f, 0.f, 0.f, -1.f, 0.f, 1.f));

  mesh.vertices.push_back(Vertex(-size, -size, size, r, g, b, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f));
  mesh.vertices.push_back(Vertex(size, -size, size, r, g, b, 1.f, 0.f, 0.f, 1.f, 1.f, 0.f));
  mesh.vertices.push_back(Vertex(size, size, size, r, g, b, 1.f, 0.f, 0.f, 1.f, 1.f, 1.f));
  mesh.vertices.push_back(Vertex(-size, size, size, r, g, b, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f));

  mesh.vertices.push_back(Vertex(-size, -size, size, r, g, b, 1.f, -1.f, 0.f, 0.f, 1.f, 0.f));
  mesh.vertices.push_back(Vertex(-size, size, size, r, g, b, 1.f, -1.f, 0.f, 0.f, 1.f, 0.f));
  mesh.vertices.push_back(Vertex(-size, size, -size, r, g, b, 1.f, -1.f, 0.f, 0.f, 1.f, 1.f));
  mesh.vertices.push_back(Vertex(-size, -size, -size, r, g, b, 1.f, -1.f, 0.f, 0.f, 1.f, 1.f));

  mesh.vertices.push_back(Vertex(size, -size, size, r, g, b, 1.f, 1.f, 0.f, 0.f, 0.f, 0.f));
  mesh.vertices.push_back(Vertex(size, -size, -size, r, g, b, 1.f, 1.f, 0.f, 0.f, 1.f, 0.f));
  mesh.vertices.push_back(Vertex(size, size, -size, r, g, b, 1.f, 1.f, 0.f, 0.f, 1.f, 1.f));
  mesh.vertices.push_back(Vertex(size, size, size, r, g, b, 1.f, 1.f, 0.f, 0.f, 0.f, 1.f));

  mesh.indices = {
    0,  1,  2,  0,  2,  3,   // 윗면
    4,  5,  6,  4,  6,  7,   // 아랫면
    8,  9,  10, 8,  10, 11,  // 앞면
    12, 13, 14, 12, 14, 15,  // 뒷면
    16, 17, 18, 16, 18, 19,  // 왼쪽
    20, 21, 22, 20, 22, 23   // 오른쪽
  };

  return mesh;
}
MeshData Toybox::MakeSphere(float radius, float r, float g, float b, float numSlice, float numStack)
{
  MeshData mesh;

  const float dTheta = -(3.141592 * 2) / float(numSlice);
  const float dPhi   = -3.141592 / float(numStack);

  for (int i = 0; i <= numStack; ++i)
  {
    Vertex stackStartPoint;
    stackStartPoint.x = (-radius) * (-sin(dPhi * i));
    stackStartPoint.y = (-radius) * cos(dPhi * i);
    stackStartPoint.z = 0;

    for (int j = 0; j <= numSlice; ++j)
    {
      Vertex vtx;
      vtx.x         = stackStartPoint.x * cos(dTheta * j);
      vtx.y         = stackStartPoint.y;
      vtx.z         = stackStartPoint.x * (-sin(dTheta * j));
      float nFactor = sqrt(pow(vtx.x, 2) + pow(vtx.y, 2) + pow(vtx.z, 2));
      vtx.nx        = vtx.x / nFactor;
      vtx.ny        = vtx.y / nFactor;
      vtx.nz        = vtx.z / nFactor;
      vtx.r         = r;
      vtx.g         = g;
      vtx.b         = b;
      vtx.tx        = float(j) / numSlice;
      vtx.ty        = 1.0f - float(i) / numStack;
      mesh.vertices.push_back(vtx);
    }
  }

  for (int j = 0; j < numStack; j++)
  {
    const int offset = (numSlice + 1) * j;
    for (int i = 0; i < numSlice; i++)
    {
      mesh.indices.push_back(offset + i);
      mesh.indices.push_back(offset + i + numSlice + 1);
      mesh.indices.push_back(offset + i + 1 + numSlice + 1);

      mesh.indices.push_back(offset + i);
      mesh.indices.push_back(offset + i + 1 + numSlice + 1);
      mesh.indices.push_back(offset + i + 1);
    }
  }

  return mesh;
}

std::vector<MeshData> Toybox::ReadObject(std::string path)
{
  std::vector<MeshData> totalObject;

  // 1. 씬 데이터 읽기
  Assimp::Importer importer;
  const aiScene*   scene =
      importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

  // 2. 에러 체크
  // if is Not Zero
  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
  {
    // TODO: Error check
  }
  std::string directory = path.substr(0, path.find_last_of('/'));
  if (scene != nullptr)
  {
    Toybox::processNode(scene->mRootNode, scene, totalObject);
  }

  return totalObject;
}
void Toybox::processNode(aiNode* node, const aiScene* scene, std::vector<MeshData>& models)
{
  // 1. 현재 노드에 위치하여있는 각각의 매쉬를 처리한다.
  for (unsigned int i = 0; i < node->mNumMeshes; i++)
  {
    // 2.
    // node 객체에는 씬 속 실제 객체의 색인값을 포함
    // 씬은 모든 데이터를 포함, 노드는 단순히 조직화된 stuff만 유지
    aiMesh*  mesh = scene->mMeshes[node->mMeshes[i]];
    MeshData model;
    Toybox::processMesh(mesh, scene, model);
    models.push_back(model);
  }
  // after we've processed all of the meshes (if any) we then recursively
  // process each of the children nodes
  for (unsigned int i = 0; i < node->mNumChildren; i++)
  {
    Toybox::processNode(node->mChildren[i], scene, models);
  }
}
void Toybox::processMesh(aiMesh* mesh, const aiScene* scene, MeshData& model)
{
  // Walk through each of the mesh's vertices
  for (unsigned int i = 0; i < mesh->mNumVertices; i++)
  {
    Vertex vertex;
    if (mesh->HasVertexColors(0))
    {
      vertex.r = mesh->mColors[i]->r;
      vertex.g = mesh->mColors[i]->g;
      vertex.b = mesh->mColors[i]->b;
      vertex.a = mesh->mColors[i]->a;
    }
    else
    {
      vertex.r = 1.0f;
      vertex.g = 1.0f;
      vertex.b = 1.0f;
      vertex.a = 1.0f;
    }

    // positions
    if (mesh->HasPositions())
    {
      vertex.x = mesh->mVertices[i].x;
      vertex.y = mesh->mVertices[i].y;
      vertex.z = mesh->mVertices[i].z;
    }
    // normals
    if (mesh->HasNormals())
    {
      vertex.nx = mesh->mNormals[i].x;
      vertex.ny = mesh->mNormals[i].y;
      vertex.nz = mesh->mNormals[i].z;
    }
    // texture coordinates
    if (mesh->HasTextureCoords(0))  // does the mesh contain texture coordinates?
    {
      // glm::vec2 vec;
      // a vertex can contain up to 8 different texture coordinates. We thus
      // make the assumption that we won't use models where a vertex can have
      // multiple texture coordinates so we always take the first set (0).
      vertex.tx = mesh->mTextureCoords[0][i].x;
      vertex.ty = mesh->mTextureCoords[0][i].y;
      //// tangent
      // vector.x = mesh->mTangents[i].x;
      // vector.y = mesh->mTangents[i].y;
      // vector.z = mesh->mTangents[i].z;
      //// bitangent
      // vector.x = mesh->mBitangents[i].x;
      // vector.y = mesh->mBitangents[i].y;
      // vector.z = mesh->mBitangents[i].z;
    }
    else
    {
      vertex.tx = 0.f;
      vertex.ty = 0.f;
    }
    model.vertices.push_back(vertex);
  }
  // 매쉬의 정면(face)를 찾아냄
  // 대응되는 vertex 색인(순번)을 찾아냄(retrieve)
  for (unsigned int i = 0; i < mesh->mNumFaces; i++)
  {
    aiFace face = mesh->mFaces[i];
    // assimp의 face는 triangle이다.
    if (face.mNumIndices != 3)
      continue;
    for (unsigned int j = 0; j < face.mNumIndices; j++)
      model.indices.push_back(face.mIndices[j]);
  }

  // process materials
  // TODO: Material 파싱 로직 만들기
  aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
}
}  // namespace libEngine