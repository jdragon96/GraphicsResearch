#pragma once

#include <vector>

#include "engine/model/VertexData.h"

template <typename T>
class MeshData
{
public:
	MeshData()
	{
	}

	std::vector<T>        vertices;
	std::vector<uint32_t> indices;
};