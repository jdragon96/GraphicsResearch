#pragma once

#include "engine/Macro.h"
#include "engine/model/MeshData.h"

template <typename T>
class MeshBufferBase
{
public:
  MeshData<T> m_mesh;
};
