#pragma once

#include "engine/Macro.h"
#include "engine/model/CMesh.h"
#include "engine/interface/ConstBufferBase.h"
#include "engine/model/MeshData.h"

template <typename T>
class MeshBufferBase
{
public:
  SHARED_PTR(MeshBufferBase);

  MeshData<T>                       m_mesh;
  ConstBufferBase<CMesh>::SharedPtr m_constBuffer;
};
