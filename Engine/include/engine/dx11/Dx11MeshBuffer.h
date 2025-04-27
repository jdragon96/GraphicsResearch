#pragma once

#include <vector>

#include "engine/Macro.h"
#include "engine/interface/MeshBufferBase.h"
#include "engine/model/MeshData.h"
#include "engine/model/CMesh.h"
#include "engine/dx11/Dx11GraphicsPSO.h"
#include "engine/dx11/Dx11EngineManager.h"
#include "engine/dx11/Dx11ConstantBuffer.h"
#include "engine/dx11/Dx11TextureBuffer.h"

// �׻� b1�� �Է¹ޱ�

enum class ObjectType
{
  WEAPON,
  ENEMY,
  CHARACTER,
};

template <typename T>
class Dx11MeshBuffer : public MeshBufferBase<T>
{
public:
  SHARED_PTR(Dx11MeshBuffer);

  Dx11MeshBuffer();

  void UseSimulation(bool flag);

  void SetMass(float mass);

  void SetPxTransform(physx::PxTransform t);

  void SetPSO(Dx11GraphicsPSO::SharedPtr pso);

  void SetMesh(MeshData<T> mesh);

  void SetObjectType(ObjectType type);

  void SetPosition(Vec3 pos);

  Vec3 GetPosition();

  void SetRotation(float angle);

  void SetTransform(Mat4 pos);

  void Initialize();

  void SetPixelTexture(std::vector<Dx11TextureBuffer::SharedPtr> buffer);

  void SetVertexTexture(std::vector<Dx11TextureBuffer::SharedPtr> buffer);

  void Render();

  void Reflect(Vec3 pos, Vec3 normal);

  // MeshData<T>                               m_mesh;
  Dx11GraphicsPSO::SharedPtr m_pso;
  // Dx11ConstantBuffer<CMesh>::SharedPtr      m_constBuffer;
  std::vector<Dx11TextureBuffer::SharedPtr> m_pixelTextureBuffer;
  std::vector<Dx11TextureBuffer::SharedPtr> m_vertexTextureBuffer;

  Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
  Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;

  Vec3 bboxMax;
  Vec3 bboxSize;
  Vec3 bboxMin;

  ObjectType             m_rigidType;
  physx::PxRigidDynamic* m_rigidDynamic;
  float                  m_mass;

  bool m_isSimulatedObject = true;
};

#include "engine/dx11/Dx11MeshBuffer_inli.h"