#include "engine/common/FBXLoader.h"
#include "engine/common/SystemUtils.h"
#include "engine/common/ResourceUtils.h"
#include "engine/model/AnimationData.h"
#include "engine/model/MeshData.h"
#include <filesystem>

FBXLoader::FBXLoader()
{
}

std::pair<std::vector<MeshData<SkinnedVertexData>>, AnimationData> FBXLoader::Read(std::string resFolderPath,
                                                                                   std::string filename, float scale)
{
  m_resFolderPath = resFolderPath;
  m_fileName      = filename;

  std::string fbxPath = resFolderPath + filename;

  // 1. 파일 체크
  SystemUtils::PrintFalseMsg(ResourceUtils::FileCheck(fbxPath), fbxPath + " 리소스가 없음", true);

  // 2. 파일 로드 시작
  Assimp::Importer importer;
  const aiScene*   pScene = importer.ReadFile(fbxPath, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

  // 3. 뼈 찾기
  FindBones(pScene);

  // 4. 뼈 정렬하기
  int counter = 0;
  BoneSorting(pScene->mRootNode, &counter);
  boneIdToName.resize(boneNameToID.size());
  for (auto& i : boneNameToID)
  {
    auto id          = i.second;
    auto name        = i.first;
    boneIdToName[id] = name;
  }
  boneParent.resize(boneNameToID.size(), -1);  // parent가 -1이면 root를 의미
  offsetMatrix.resize(boneIdToName.size(), Mat4().setIdentity());
  boneTransform.resize(boneIdToName.size(), Mat4().setIdentity());

  // 5. 매쉬 로드하기
  Mat4 tr;
  tr.setIdentity();
  ProcessNode(pScene->mRootNode, pScene, tr, m_meshes);

  // 6. 애니메이션 로드하기
  if (pScene->HasAnimations())
    ReadAnimation(pScene);

  for (auto& mesh : m_meshes)
  {
    for (auto& vertex : mesh.vertices)
    {
      vertex.x *= scale;
      vertex.y *= scale;
      vertex.z *= scale;
    }
  }
  for (auto& mat : boneTransform)
  {
    mat(0, 3) *= scale;
    mat(1, 3) *= scale;
    mat(2, 3) *= scale;
  }
  for (auto& mat : offsetMatrix)
  {
    mat(0, 3) *= scale;
    mat(1, 3) *= scale;
    mat(2, 3) *= scale;
  }
  for (auto& clip : aniClips)
  {
    for (auto& bone : clip.keys)
    {
      for (auto& key : bone)
      {
        key.pos *= scale;
        key.Update();
      }
    }
  }
  AnimationData anim;
  anim.aniClips      = aniClips;
  anim.boneParent    = boneParent;
  anim.boneTransform = boneTransform;
  anim.offsetMatrix  = offsetMatrix;

  return { m_meshes, anim };
}
void FBXLoader::FindBones(const aiScene* scene)
{
  if (scene)
  {
    // 1. 뼈 이름 탐색하기
    for (uint32_t index = 0; index < scene->mNumMeshes; ++index)
    {
      const auto* mesh = scene->mMeshes[index];
      if (mesh->HasBones())
      {
        for (uint32_t boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
        {
          const aiBone* bone                = mesh->mBones[boneIndex];
          boneNameToID[bone->mName.C_Str()] = -1;
        }
      }
    }
  }
}
void FBXLoader::BoneSorting(aiNode* node, int* counter)
{
  if (node)
  {
    if (boneNameToID.count(node->mName.C_Str()))
    {
      boneNameToID[node->mName.C_Str()] = *counter;
      *counter += 1;
    }

    for (int i = 0; i < node->mNumChildren; i++)
    {
      auto child = node->mChildren[i];
      BoneSorting(child, counter);
    }
  }
}
void FBXLoader::ProcessNode(aiNode* node, const aiScene* scene, Mat4 tr,
                            std::vector<MeshData<SkinnedVertexData>>& meshes)
{
  // 1. 부모 노드가 존재하는지 체크한다.
  if (FindParent(node->mParent) && boneNameToID.count(node->mName.C_Str()))
  {
    const auto boneId = boneNameToID[node->mName.C_Str()];
    auto       parent = FindParent(node->mParent);

    if (parent)
      boneParent[boneId] = boneNameToID[parent->mName.C_Str()];
  }

  // 4.
  Mat4 m(&node->mTransformation.a1);
  // m.transposeInPlace();
  m = tr * m;
  std::cout << node->mName.C_Str() << std::endl;

  for (int i = 0; i < node->mNumMeshes; i++)
  {
    aiMesh* mesh    = scene->mMeshes[node->mMeshes[i]];
    auto    newMesh = GetMesh(mesh, scene);
    // for (auto& v : newMesh.vertices)
    //{
    //   // auto vtx = m.(Vec4(v.x, v.y, v.z, 1));
    //   auto vtx = m * Vec4(v.x, v.y, v.z, 1);
    //   v.SetPosition(vtx.x(), vtx.y(), vtx.z());
    // }
    // for (auto& v : newMesh.skinnedVertices)
    //{
    //   auto vtx = m * Vec4(v.x, v.y, v.z, 1);
    //   v.SetPosition(vtx.x(), vtx.y(), vtx.z());
    // }
    meshes.push_back(newMesh);
  }

  for (int i = 0; i < node->mNumChildren; i++)
  {
    this->ProcessNode(node->mChildren[i], scene, m, meshes);
  }
}
const aiNode* FBXLoader::FindParent(const aiNode* node)
{
  if (!node)
    return nullptr;
  if (boneNameToID.count(node->mName.C_Str()) > 0)
    return node;
  return FindParent(node->mParent);
}
MeshData<SkinnedVertexData> FBXLoader::GetMesh(aiMesh* mesh, const aiScene* scene, bool isGLTF, bool revertNormal)
{
  MeshData<SkinnedVertexData> newMesh;
  // auto&               vertices        = newMesh.vertices;
  auto& vertices = newMesh.vertices;
  auto& indices  = newMesh.indices;

  // Walk through each of the mesh's vertices
  for (int i = 0; i < mesh->mNumVertices; i++)
  {
    SkinnedVertexData vertex;

    vertex.x = mesh->mVertices[i].x;
    vertex.y = mesh->mVertices[i].y;
    vertex.z = mesh->mVertices[i].z;

    vertex.SetColor(0, 1, 0, 1);

    vertex.nx = mesh->mNormals[i].x;
    if (isGLTF)
    {
      vertex.ny = mesh->mNormals[i].z;
      vertex.nz = -mesh->mNormals[i].y;
    }
    else
    {
      vertex.ny = mesh->mNormals[i].y;
      vertex.nz = mesh->mNormals[i].z;
    }

    if (revertNormal)
    {
      vertex.nx *= -1.0f;
      vertex.ny *= -1.0f;
      vertex.nz *= -1.0f;
    }

    if (mesh->mTextureCoords[0])
    {
      vertex.tx = (float)mesh->mTextureCoords[0][i].x;
      vertex.ty = (float)mesh->mTextureCoords[0][i].y;
    }
    vertices.push_back(vertex);
  }

  for (int i = 0; i < mesh->mNumFaces; i++)
  {
    aiFace face = mesh->mFaces[i];
    for (int j = 0; j < face.mNumIndices; j++)
      indices.push_back(face.mIndices[j]);
  }

  if (mesh->HasBones())
  {
    int                             count = 0;
    std::vector<std::vector<float>> boneWeights(vertices.size());
    std::vector<std::vector<int>>   boneIndices(vertices.size());

    for (uint32_t i = 0; i < mesh->mNumBones; i++)
    {
      const aiBone* bone = mesh->mBones[i];

      if (boneNameToID.count(bone->mName.C_Str()) > 0)
      {
        auto boneid          = boneNameToID[bone->mName.C_Str()];
        offsetMatrix[boneid] = Mat4((float*)&bone->mOffsetMatrix);

        // 이 뼈가 영향을 주는 Vertex의 개수
        for (uint32_t j = 0; j < bone->mNumWeights; j++)
        {
          aiVertexWeight weight = bone->mWeights[j];
          assert(weight.mVertexId < boneIndices.size());
          boneIndices[weight.mVertexId].push_back(boneid);
          boneWeights[weight.mVertexId].push_back(weight.mWeight);
        }
      }
    }

    // 예전에는 Vertex 하나에 영향을 주는 Bone은 최대 4개
    // 요즘은 더 많을 수도 있는데 모델링 소프트웨어에서 조정하거나
    // 읽어들이면서 weight가 너무 작은 것들은 뺄 수도 있음
    // 읽어들이면서 weight가 너무 작은 것들은 뺄 수도 있음
    int maxBones = 0;
    for (int i = 0; i < boneWeights.size(); i++)
    {
      maxBones = std::max(maxBones, int(boneWeights[i].size()));
    }

    // skinnedVertices.resize(vertices.size());
    for (int i = 0; i < vertices.size(); i++)
    {
      // skinnedVertices[i].SetPosition(vertices[i].x, vertices[i].y, vertices[i].z);
      // skinnedVertices[i].SetNormal(vertices[i].nx, vertices[i].ny, vertices[i].nz);
      // skinnedVertices[i].SetTextureCoord(vertices[i].tx, vertices[i].ty);
      // skinnedVertices[i].SetColor(0, 1, 0, 1);

      for (int j = 0; j < boneWeights[i].size(); j++)
      {
        vertices[i].blendWeights[j] = boneWeights[i][j];
        vertices[i].boneIndices[j]  = boneIndices[i][j];
      }
    }
  }
  if (mesh->mMaterialIndex >= 0)
  {
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    ReadTextureFilename(scene, material, aiTextureType_BASE_COLOR);
    ReadTextureFilename(scene, material, aiTextureType_DIFFUSE);
  }

  // http://assimp.sourceforge.net/lib_html/materials.html
  // if (mesh->mMaterialIndex >= 0)
  //{
  //  aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

  //  newMesh.albedoTextureFilename = ReadTextureFilename(scene, material, aiTextureType_BASE_COLOR);
  //  if (newMesh.albedoTextureFilename.empty())
  //  {
  //    newMesh.albedoTextureFilename = ReadTextureFilename(scene, material, aiTextureType_DIFFUSE);
  //  }
  //  newMesh.emissiveTextureFilename  = ReadTextureFilename(scene, material, aiTextureType_EMISSIVE);
  //  newMesh.heightTextureFilename    = ReadTextureFilename(scene, material, aiTextureType_HEIGHT);
  //  newMesh.normalTextureFilename    = ReadTextureFilename(scene, material, aiTextureType_NORMALS);
  //  newMesh.metallicTextureFilename  = ReadTextureFilename(scene, material, aiTextureType_METALNESS);
  //  newMesh.roughnessTextureFilename = ReadTextureFilename(scene, material, aiTextureType_DIFFUSE_ROUGHNESS);
  //  newMesh.aoTextureFilename        = ReadTextureFilename(scene, material, aiTextureType_AMBIENT_OCCLUSION);
  //  if (newMesh.aoTextureFilename.empty())
  //  {
  //    newMesh.aoTextureFilename = ReadTextureFilename(scene, material, aiTextureType_LIGHTMAP);
  //  }
  //  newMesh.opacityTextureFilename = ReadTextureFilename(scene, material, aiTextureType_OPACITY);
  //}

  return newMesh;
}
std::string FBXLoader::ReadTextureFilename(const aiScene* scene, aiMaterial* material, aiTextureType type)
{
  if (material->GetTextureCount(type) > 0)
  {
    aiString filepath;
    material->GetTexture(type, 0, &filepath);

    std::string fileName = std::string(std::filesystem::path(filepath.C_Str()).filename().string());
    std::string fullPath = m_resFolderPath + fileName;

    // 1. 실제로 파일이 존재하는지 확인
    if (!ResourceUtils::FileCheck(fullPath))
    {
      // 2. 파일이 없을 경우 혹시 fbx 자체에 Embedded인지 확인
      const aiTexture* texture = scene->GetEmbeddedTexture(filepath.C_Str());
      if (texture)
      {
        // 3. Embedded texture가 존재하고 png일 경우 저장
        if (std::string(texture->achFormatHint).find("png") != std::string::npos)
        {
          std::ofstream fs(fullPath.c_str(), std::ios::binary | std::ios::out);
          fs.write((char*)texture->pcData, texture->mWidth);
          fs.close();
          // 참고: compressed format일 경우 texture->mHeight가 0
        }
      }
    }
    else
    {
      return fullPath;
    }

    return fullPath;
  }
  else
  {
    return "";
  }
}
void FBXLoader::ReadAnimation(const aiScene* pScene)
{
  aniClips.resize(pScene->mNumAnimations);

  for (uint32_t i = 0; i < pScene->mNumAnimations; i++)
  {
    auto& clip = aniClips[i];

    const aiAnimation* ani = pScene->mAnimations[i];

    clip.duration    = ani->mDuration;
    clip.ticksPerSec = ani->mTicksPerSecond;
    clip.keys.resize(boneNameToID.size());
    clip.numChannels = ani->mNumChannels;

    // 애니매이션에 사용되는 뼈 개수 파악
    for (uint32_t c = 0; c < ani->mNumChannels; c++)
    {
      const aiNodeAnim* nodeAnim = ani->mChannels[c];
      const int         boneId   = boneNameToID[nodeAnim->mNodeName.C_Str()];
      clip.keys[boneId].resize(nodeAnim->mNumPositionKeys);

      // 애니메이션의 프레임 별 뼈 변환을 해석
      for (uint32_t k = 0; k < nodeAnim->mNumPositionKeys; k++)
      {
        const auto pos   = nodeAnim->mPositionKeys[k].mValue;
        const auto rot   = nodeAnim->mRotationKeys[k].mValue;
        const auto scale = nodeAnim->mScalingKeys[k].mValue;
        auto&      key   = clip.keys[boneId][k];
        key.pos          = { pos.x, pos.y, pos.z };
        key.scale        = { scale.x, scale.y, scale.z };
        key.rot          = Quat(rot.w, rot.x, rot.y, rot.z);
      }
    }
  }
}
void FBXLoader::Update(int clipId, int frame)
{
  auto& clip = aniClips[clipId];

  for (int boneId = 0; boneId < boneTransform.size(); boneId++)
  {
    auto&     keys      = clip.keys[boneId];
    const int parentIdx = boneParent[boneId];
    Mat4      parentMatrix;
    if (parentIdx >= 0)
      parentMatrix = boneTransform[parentIdx];
    else
      parentMatrix.setIdentity();

    if (keys.size() > 0)
    {
      auto key = keys[frame % keys.size()];
      key.Update();
      boneTransform[boneId] = parentMatrix * key.transform;
    }
  }
}