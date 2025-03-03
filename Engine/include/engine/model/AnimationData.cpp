#include "engine/model/AnimationData.h"

void AnimationClip::Key::Update()
{
	transform = transform.setIdentity();
	auto scaleMat = Eigen::Scaling(scale.x(), scale.y(), scale.z()).toDenseMatrix();
	auto rotMat = rot.toRotationMatrix();

	transform.block<3, 3>(0, 0) = (scaleMat * rotMat);
	transform(0, 3) = pos.x();
	transform(1, 3) = pos.y();
	transform(2, 3) = pos.z();
}

AnimationData::AnimationData()
{
	animZeroPosTransform.setIdentity();
	offsetAnimation = Vec3(0, 0, 0);
}

Mat4 AnimationData::Transform(int boneID)
{
	return (animZeroPosTransform * (boneTransform[boneID] * offsetMatrix[boneID]));
}

Vec3 AnimationData::GetLastPosition()
{
	return aniClips[0].keys[0][aniClips[0].keys[0].size() - 1].pos;
}

void AnimationData::Update(int clipId, int frame)
{
	auto& clip = aniClips[clipId];

	for (int boneId = 0; boneId < boneTransform.size(); boneId++)
	{
		auto& keys = clip.keys[boneId];
		auto      index = frame % keys.size();
		const int parentIdx = boneParent[boneId];
		Mat4      parentMatrix;

		// 1. 부로 뼈 찾기
		if (parentIdx >= 0)
			parentMatrix = boneTransform[parentIdx];
		else
			parentMatrix.setIdentity();

		// 2. 부모 뼈가 있는경우, Forword kinematics를 수행
		if (keys.size() > 0)
		{
			auto key = keys[index];
			boneTransform[boneId] = parentMatrix * key.transform;
		}
	}
}