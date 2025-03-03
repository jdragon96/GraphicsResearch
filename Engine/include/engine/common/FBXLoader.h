#pragma once

#include <string>
#include <filesystem>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <utility>

#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "engine/Macro.h"
#include "engine/model/MeshData.h"
#include "engine/model/AnimationData.h"

class FBXLoader
{
public:
	FBXLoader();

	std::pair<std::vector<MeshData<SkinnedVertexData>>, AnimationData> Read(std::string resFolderPath,
		std::string filename, float scale = 0.01f);

	void Update(int clipId, int frame);

	Mat4 GetTransform(int boneID)
	{
		return boneTransform[boneID];
	}

	Mat4 Get(int boneID)
	{
		return ((boneTransform[boneID] * offsetMatrix[boneID]));
		return ((boneTransform[boneID] * offsetMatrix[boneID]) * defaultMatrix);
	}

private:
	void FindBones(const aiScene* scene);

	void BoneSorting(aiNode* node, int* counter);

	const aiNode* FindParent(const aiNode* node);

	void ProcessNode(aiNode* node, const aiScene* scene, Mat4 tr, std::vector<MeshData<SkinnedVertexData>>& meshes);

	MeshData<SkinnedVertexData> GetMesh(aiMesh* mesh, const aiScene* scene, bool isGLTF = false,
		bool revertNormal = false);

	std::string ReadTextureFilename(const aiScene* scene, aiMaterial* material, aiTextureType type);

	void ReadAnimation(const aiScene* pScene);

	std::unordered_map<std::string, int> boneNameToID;
	std::vector<std::string>             boneIdToName;
	std::vector<int>                     boneParent;
	std::vector<Mat4>                    offsetMatrix;
	std::vector<Mat4>                    boneTransform;
	Mat4                                 defaultMatrix;

	std::string m_resFolderPath;
	std::string m_fileName;

public:
	std::vector<AnimationClip>               aniClips;
	std::vector<MeshData<SkinnedVertexData>> m_meshes;
};