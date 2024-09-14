#pragma once

#include <vector>
#include <string>

#include "libEngine/shared/Mesh.h"
#include "libEngine/shared/Vertex.h"
#include "math.h"

#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace libEngine
{
class Toybox
{
public:
  static MeshData MakeCube(float length, float r, float g, float b);
  static MeshData MakeSphere(float radius, float r, float g, float b, float numSlice = 20.f, float numStack = 20.f);
  static MeshData MakeSuqare();
  static std::vector<MeshData> ReadObject(std::string path);

private:
  static void processNode(aiNode* node, const aiScene* scene, std::vector<MeshData>& models);
  static void processMesh(aiMesh* mesh, const aiScene* scene, MeshData& model);
};
}  // namespace libEngine