#pragma once

#include <iostream>

#include "engine/dx11/Dx11MeshBuffer.h"
#include "physx/PxPhysicsAPI.h"
#include "Eigen/Dense"

template <typename T>
Dx11MeshBuffer<T>::Dx11MeshBuffer() : m_rigidDynamic(nullptr), m_mass(9.8f)
{
}

template <typename T>
void Dx11MeshBuffer<T>::SetMass(float mass)
{
  m_mass = mass;
}

template <typename T>
void Dx11MeshBuffer<T>::SetPxTransform(physx::PxTransform t)
{
  if (m_isSimulatedObject)
  {
    m_rigidDynamic->setGlobalPose(t);
  }
  this->m_constBuffer->m_bufferData.world(0, 3) = t.p.x;
  this->m_constBuffer->m_bufferData.world(1, 3) = t.p.y;
  this->m_constBuffer->m_bufferData.world(2, 3) = t.p.z;
}

template <typename T>
void Dx11MeshBuffer<T>::UseSimulation(bool flag)
{
  m_isSimulatedObject = flag;
}

template <typename T>
void Dx11MeshBuffer<T>::SetObjectType(ObjectType type)
{
  m_rigidType = type;
}

template <typename T>
void Dx11MeshBuffer<T>::SetPosition(Vec3 pos)
{
  if (m_isSimulatedObject)
  {
    physx::PxTransform localTm(physx::PxVec3(pos.x(), pos.y(), pos.z()));
    m_rigidDynamic->setGlobalPose(localTm);
  }
  this->m_constBuffer->m_bufferData.world(0, 3) = pos.x();
  this->m_constBuffer->m_bufferData.world(1, 3) = pos.y();
  this->m_constBuffer->m_bufferData.world(2, 3) = pos.z();
}

template <typename T>
Vec3 Dx11MeshBuffer<T>::GetPosition()
{
  return Vec3(this->m_constBuffer->m_bufferData.world(0, 3), this->m_constBuffer->m_bufferData.world(1, 3),
              this->m_constBuffer->m_bufferData.world(2, 3));
}

template <typename T>
inline void Dx11MeshBuffer<T>::SetRotation(float deg)
{
  double          radians        = deg * 3.141592 / 180.0;
  Eigen::Matrix3f rotationMatrix = Eigen::AngleAxisf(radians, Eigen::Vector3f::UnitY()).toRotationMatrix();
  this->m_constBuffer->m_bufferData.world.block<3, 3>(0, 0) = rotationMatrix;
}

template <typename T>
void Dx11MeshBuffer<T>::SetPSO(Dx11GraphicsPSO::SharedPtr pso)
{
  m_pso = pso;
}

template <typename T>
void Dx11MeshBuffer<T>::SetMesh(MeshData<T> mesh)
{
  this->m_mesh = mesh;
}

template <typename T>
void Dx11MeshBuffer<T>::SetTransform(Mat4 mat)
{
  this->m_constBuffer->m_bufferData.world = mat;
}

template <typename T>
void Dx11MeshBuffer<T>::SetPixelTexture(std::vector<Dx11TextureBuffer::SharedPtr> buffers)
{
  m_pixelTextureBuffer = buffers;
}

template <typename T>
void Dx11MeshBuffer<T>::SetVertexTexture(std::vector<Dx11TextureBuffer::SharedPtr> buffers)
{
  m_vertexTextureBuffer = buffers;
}

template <typename T>
void Dx11MeshBuffer<T>::Initialize()
{
  auto devPtr = Dx11EngineManager::instance().GetDevicePtr();

  //   1. Vertex Buffer 생성
  D3D11_BUFFER_DESC bufferDesc;
  ZeroMemory(&bufferDesc, sizeof(bufferDesc));
  bufferDesc.Usage               = D3D11_USAGE::D3D11_USAGE_IMMUTABLE;
  bufferDesc.ByteWidth           = UINT(sizeof(T) * this->m_mesh.vertices.size());
  bufferDesc.BindFlags           = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
  bufferDesc.CPUAccessFlags      = 0;
  bufferDesc.StructureByteStride = sizeof(T);

  D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
  vertexBufferData.pSysMem                = this->m_mesh.vertices.data();
  vertexBufferData.SysMemPitch            = 0;
  vertexBufferData.SysMemSlicePitch       = 0;

  const HRESULT hr = devPtr->CreateBuffer(&bufferDesc, &vertexBufferData, m_vertexBuffer.GetAddressOf());
  if (FAILED(hr))
  {
    std::cout << "Vertex CreateBuffer() failed. " << std::hex << hr << std::endl;
  };

  // 2. Index buffer 생성
  D3D11_BUFFER_DESC indexBufferDesc   = {};
  indexBufferDesc.Usage               = D3D11_USAGE::D3D11_USAGE_IMMUTABLE;  // 초기화 후 변경X
  indexBufferDesc.ByteWidth           = UINT(sizeof(uint32_t) * this->m_mesh.indices.size());
  indexBufferDesc.BindFlags           = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
  indexBufferDesc.CPUAccessFlags      = 0;  // 0 if no CPU access is necessary.
  indexBufferDesc.StructureByteStride = sizeof(uint32_t);

  D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
  indexBufferData.pSysMem                = this->m_mesh.indices.data();
  indexBufferData.SysMemPitch            = 0;
  indexBufferData.SysMemSlicePitch       = 0;
  const HRESULT IndexHr = devPtr->CreateBuffer(&indexBufferDesc, &indexBufferData, m_indexBuffer.GetAddressOf());
  if (FAILED(IndexHr))
  {
    std::cout << "Index CreateBuffer() failed. " << std::hex << hr << std::endl;
  };

  // 3. Meshbuffer Const buffer 초기화
  {
    this->m_constBuffer = Dx11ConstantBuffer<CMesh>::MakeShared();
    this->m_constBuffer->m_bufferData.world.setIdentity();
    this->m_constBuffer->m_bufferData.worldInv.setIdentity();
    this->m_constBuffer->Initialize(EConstBufferType::VERTEX_MODEL);
  }
  // 4. BBox 생성하기
  {
    bboxMax = Vec3(-1000, -1000, -1000);
    bboxMin = Vec3(1000, 1000, 1000);
    for (auto& vertex : this->m_mesh.vertices)
    {
      if (vertex.x >= bboxMax.x())
        bboxMax[0] = vertex.x;
      if (vertex.y >= bboxMax.y())
        bboxMax[1] = vertex.y;
      if (vertex.z >= bboxMax.z())
        bboxMax[2] = vertex.z;
      if (vertex.x < bboxMin.x() && vertex.x < bboxMax.x())
        bboxMin[0] = vertex.x;
      if (vertex.y < bboxMin.y() && vertex.y < bboxMax.y())
        bboxMin[1] = vertex.y;
      if (vertex.z < bboxMin.z() && vertex.z < bboxMax.z())
        bboxMin[2] = vertex.z;
    }
    bboxSize = bboxMax - bboxMin;
    std::cout << bboxSize << std::endl;
  }
  // 5. PhysX 객체 만들기
  {
    if (m_isSimulatedObject)
    {
      physx::PxShape* shape = Dx11EngineManager::instance().gPhysics->createShape(
          physx::PxBoxGeometry(bboxSize.x() * 0.5f, bboxSize.y() * 0.5f, bboxSize.z() * 0.5f),
          *Dx11EngineManager::instance().gMaterial);
      physx::PxTransform localTm(physx::PxVec3(0, 0, 0));
      m_rigidDynamic = Dx11EngineManager::instance().gPhysics->createRigidDynamic(localTm);
      m_rigidDynamic->attachShape(*shape);
      physx::PxRigidBodyExt::updateMassAndInertia(*m_rigidDynamic, m_mass);
      Dx11EngineManager::instance().gScene->addActor(*m_rigidDynamic);
      switch (m_rigidType)
      {
        case ObjectType::WEAPON:
          m_rigidDynamic->setName("WEAPON");
          break;
        case ObjectType::ENEMY:
          m_rigidDynamic->setName("ENEMY");
          break;
        case ObjectType::CHARACTER:
          m_rigidDynamic->setName("CHARACTER");
          break;
      }
      shape->release();
    }
  }
  // 6. ㅇㅇㅇ
  {
    // EngineManager::instance().enemyHash[m_rigidDynamic] = this;
  }
}

// template <typename T>
// void MeshBuffer<T>::Simulate()
//{
//	physx::PxTransform localTm(physx::PxVec3(
//		m_constBuffer->m_bufferData.world(0, 3),
//		m_constBuffer->m_bufferData.world(1, 3),
//		m_constBuffer->m_bufferData.world(2, 3)));
//	m_rigidDynamic->setGlobalPose(localTm);
// }

template <typename T>
void Dx11MeshBuffer<T>::Render()
{
  auto contextPtr = Dx11EngineManager::instance().GetContextPtr();
  if (m_isSimulatedObject && m_rigidDynamic)
  {
    auto pos = m_rigidDynamic->getGlobalPose();
    SetPosition(Vec3(pos.p.x, pos.p.y, pos.p.z));
  }
  m_pso->Bind();
  {
    this->m_constBuffer->m_bufferData.worldInv      = this->m_constBuffer->m_bufferData.world.inverse();
    this->m_constBuffer->m_bufferData.useReflection = 0;
    this->m_constBuffer->Update();
  }
  UINT stride = sizeof(T);
  UINT offset = 0;
  if (!m_pixelTextureBuffer.empty())
  {
    std::vector<ID3D11ShaderResourceView*> textures;
    textures.resize(m_pixelTextureBuffer.size());
    for (int index = 0; index < m_pixelTextureBuffer.size(); ++index)
      textures[index] = m_pixelTextureBuffer[index]->textureResourceView.Get();
    contextPtr->PSSetShaderResources(0, m_pixelTextureBuffer.size(), textures.data());
  }
  if (!m_vertexTextureBuffer.empty())
  {
    std::vector<ID3D11ShaderResourceView*> textures;
    textures.resize(m_vertexTextureBuffer.size());
    for (int index = 0; index < m_vertexTextureBuffer.size(); ++index)
      textures[index] = m_vertexTextureBuffer[index]->textureResourceView.Get();
    contextPtr->VSSetShaderResources(0, m_vertexTextureBuffer.size(), textures.data());
  }
  this->m_constBuffer->Bind();
  contextPtr->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
  contextPtr->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
  // contextPtr->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
  contextPtr->DrawIndexed(UINT(this->m_mesh.indices.size()), 0, 0);
}

template <typename T>
void Dx11MeshBuffer<T>::Reflect(Vec3 pos, Vec3 normal)
{
  //
  normal.normalize();
  float d = pos.dot(normal);

  Mat4 reflectModel;
  reflectModel(0, 0) = 1.f - 2.f * normal.x() * normal.x();
  reflectModel(0, 1) = -2.f * normal.x() * normal.y();
  reflectModel(0, 2) = -2.f * normal.x() * normal.z();
  reflectModel(0, 3) = 0.f;
  reflectModel(1, 0) = -2.f * normal.y() * normal.x();
  reflectModel(1, 1) = 1.f - 2.f * normal.y() * normal.y();
  reflectModel(1, 2) = -2.f * normal.y() * normal.z();
  reflectModel(1, 3) = 0.f;
  reflectModel(2, 0) = -2.f * normal.z() * normal.x();
  reflectModel(2, 1) = -2.f * normal.z() * normal.y();
  reflectModel(2, 2) = 1.f - 2.f * normal.z() * normal.z();
  reflectModel(2, 3) = 0.f;
  reflectModel(3, 0) = -2.f * d * normal.x();
  reflectModel(3, 1) = -2.f * d * normal.y();
  reflectModel(3, 2) = -2.f * d * normal.z();
  reflectModel(3, 3) = 1.f;
  reflectModel       = reflectModel.transpose().eval();

  auto contextPtr = Dx11EngineManager::instance().GetContextPtr();
  if (m_isSimulatedObject && m_rigidDynamic)
  {
    auto pos = m_rigidDynamic->getGlobalPose();
    SetPosition(Vec3(pos.p.x, pos.p.y, pos.p.z));
  }
  m_pso->Bind();
  {
    this->m_constBuffer->m_bufferData.worldInv      = this->m_constBuffer->m_bufferData.world.inverse();
    this->m_constBuffer->m_bufferData.reflection = reflectModel;
    this->m_constBuffer->m_bufferData.useReflection = 1;
    this->m_constBuffer->Update();
  }
  UINT stride = sizeof(T);
  UINT offset = 0;
  if (!m_pixelTextureBuffer.empty())
  {
    std::vector<ID3D11ShaderResourceView*> textures;
    textures.resize(m_pixelTextureBuffer.size());
    for (int index = 0; index < m_pixelTextureBuffer.size(); ++index)
      textures[index] = m_pixelTextureBuffer[index]->textureResourceView.Get();
    contextPtr->PSSetShaderResources(0, m_pixelTextureBuffer.size(), textures.data());
  }
  if (!m_vertexTextureBuffer.empty())
  {
    std::vector<ID3D11ShaderResourceView*> textures;
    textures.resize(m_vertexTextureBuffer.size());
    for (int index = 0; index < m_vertexTextureBuffer.size(); ++index)
      textures[index] = m_vertexTextureBuffer[index]->textureResourceView.Get();
    contextPtr->VSSetShaderResources(0, m_vertexTextureBuffer.size(), textures.data());
  }
  this->m_constBuffer->Bind();
  contextPtr->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
  contextPtr->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
  contextPtr->DrawIndexed(UINT(ConstBufferBase<T>::m_mesh.indices.size()), 0, 0);
}