#pragma once

#include <stdint.h>
#include "engine/Macro.h"

class VertexData
{
public:
	VertexData();
	VertexData(float x, float y, float z, float r, float g, float b, float a, float nx, float ny, float nz, float tx,
		float ty);
	VertexData(Vec3 pos, float r, float g, float b, float a, float nx, float ny, float nz, float tx,
		float ty);
	void SetPosition(float _x, float _y, float _z);
	void SetColor(float _r, float _g, float _b, float _a);
	void SetNormal(float _nx, float _ny, float _nz);
	void SetTextureCoord(float _tx, float _ty);

	float x;
	float y;
	float z;
	float r;
	float g;
	float b;
	float a;
	float tx;
	float ty;
	float nx;
	float ny;
	float nz;
};

struct SkinnedVertexData : public VertexData
{
	float   blendWeights[8] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };  // BLENDWEIGHT0 and 1
	uint8_t boneIndices[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };                          // BLENDINDICES0 and 1

	// boneWeights�� �ִ� 8����� ���� (Luna ���翡���� 4��)
	// bone�� ���� 256�� ���϶�� ���� uint8_t
};