#pragma once

#include <string>
#include <filesystem>
#include <unordered_map>
#include <vector>
#include <fstream>

#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "engine/Macro.h"

struct AnimationClip
{
	struct Key
	{
		Vec3 pos = Vec3(0.f, 0.f, 0.f);
		Vec3 scale = Vec3(0.f, 0.f, 0.f);
		Quat rot = Quat();
		Mat4 transform;

		/// <summary>
		/// 애니매이션 Key를 업데이트한다.
		/// </summary>
		void Update();
	};

	std::string                   name;         // Name of this animation clip
	std::vector<std::vector<Key>> keys;         // m_key[boneIdx][frameIdx]
	int                           numChannels;  // Number of bones
	int                           numKeys;      // Number of frames of this animation clip
	double                        duration;     // Duration of animation in ticks
	double                        ticksPerSec;  // Frames per second
};

class AnimationData
{
public:
	enum class AnimationType
	{
		IDLE,
		WALK_FORWARD,
		STOP
	};

	AnimationData();

	/// <summary>
	/// 
	/// </summary>
	/// <param name="boneID"></param>
	/// <returns></returns>
	Mat4 Transform(int boneID);

	/// <summary>
	/// 마지막 위치 데이터를 조회(?)
	/// </summary>
	/// <returns></returns>
	Vec3 GetLastPosition();

	/// <summary>
	/// 특정 프레임에 대해 업데이트를 수행한다.
	/// </summary>
	/// <param name="clipId"></param>
	/// <param name="frame"></param>
	void Update(int clipId, int frame);

	Mat4 animZeroPosTransform;
	std::vector<int>           boneParent;
	std::vector<Mat4>          offsetMatrix;
	std::vector<Mat4>          boneTransform;
	std::vector<AnimationClip> aniClips;
	Vec3                       offsetAnimation;
	AnimationClip::Key         prevRootKey;
};