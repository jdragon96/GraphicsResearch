#include "engine/dx11/Dx11ComputeBuffer.h"

Dx11ComputeBuffer::Dx11ComputeBuffer()
{
}

void Dx11ComputeBuffer::Initialize(int X, int Y, DXGI_FORMAT cellFormat)
{
  auto devPtr = Dx11EngineManager::instance().GetDevicePtr();

  D3D11_TEXTURE2D_DESC dsOption;
  ZeroMemory(&dsOption, sizeof(dsOption));
  dsOption.Width     = X;
  dsOption.Height    = Y;
  dsOption.MipLevels = 1;
  dsOption.ArraySize = 1;
  // dsOption.Format             = DXGI_FORMAT::DXGI_FORMAT_R16_FLOAT;
  dsOption.Format             = cellFormat;
  dsOption.Usage              = D3D11_USAGE_DEFAULT;
  dsOption.BindFlags          = D3D11_BIND_FLAG::D3D11_BIND_UNORDERED_ACCESS;
  dsOption.CPUAccessFlags     = 0;
  dsOption.MiscFlags          = 0;
  dsOption.SampleDesc.Count   = 1;
  dsOption.SampleDesc.Quality = 0;

  D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
  ZeroMemory(&uavDesc, sizeof(uavDesc));
  uavDesc.Format             = dsOption.Format;
  uavDesc.ViewDimension      = D3D11_UAV_DIMENSION_TEXTURE2D;
  uavDesc.Texture2D.MipSlice = 0;

  Dx11EngineManager::Check(devPtr->CreateTexture2D(&dsOption, nullptr, m_2dTexture.GetAddressOf()));
  Dx11EngineManager::Check(devPtr->CreateUnorderedAccessView(m_2dTexture.Get(), &uavDesc, m_UAV.GetAddressOf()));
  m_Type   = EDataType::D2D;
  m_width  = X;
  m_height = Y;
  m_format = cellFormat;
}

void Dx11ComputeBuffer::Initialize(int X, int Y, int Z, DXGI_FORMAT cellFormat)
{
  auto devPtr = Dx11EngineManager::instance().GetDevicePtr();

  // D3D11_TEXTURE3D_DESC dsOption;
  // ZeroMemory(&dsOption, sizeof(dsOption));
  // dsOption.Width              = X;
  // dsOption.Height             = Y;
  // dsOption.Depth              = Z;
  // dsOption.MipLevels          = 1;
  // dsOption.ArraySize          = 1;
  // dsOption.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
  // dsOption.Usage              = D3D11_USAGE_DEFAULT;
  // dsOption.BindFlags          = D3D11_BIND_DEPTH_STENCIL;
  // dsOption.CPUAccessFlags     = 0;
  // dsOption.MiscFlags          = 0;
  // dsOption.SampleDesc.Count   = 1;
  // dsOption.SampleDesc.Quality = 0;
  //  Dx11EngineManager::instance().InitTexture(dsOption, m_3dTexture);

  // D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
  // ZeroMemory(&uavDesc, sizeof(uavDesc));
  // uavDesc.Format             = desc.Format;
  // uavDesc.ViewDimension      = D3D11_UAV_DIMENSION_TEXTURE2D;
  // uavDesc.Texture2D.MipSlice = 0;
  // Dx11EngineManager::Check(devPtr->CreateUnorderedAccessView(m_2dTexture.Get(), &uavDesc, m_UAV.GetAddressOf()));
  m_Type = EDataType::D3D;
}

void Dx11ComputeBuffer::Execute(int GroupX, int GroupY, int GroupZ)
{
  auto contextPtr = Dx11EngineManager::instance().GetContextPtr();
  contextPtr->CSSetShaderResources(0, m_srv.size(), m_srv.data());
  contextPtr->CSSetUnorderedAccessViews(0, 1, m_UAV.GetAddressOf(), NULL);
  contextPtr->Dispatch(GroupX, GroupY, GroupZ);
  ID3D11ShaderResourceView* nullSRV[6] = {
    0,
  };
  contextPtr->CSSetShaderResources(0, 6, nullSRV);
  ID3D11UnorderedAccessView* nullUAV[6] = {
    0,
  };
  contextPtr->CSSetUnorderedAccessViews(0, 6, nullUAV, NULL);
}
void Dx11ComputeBuffer::CopyTo(Microsoft::WRL::ComPtr<ID3D11Texture2D> copyTarget)
{
  auto contextPtr = Dx11EngineManager::instance().GetContextPtr();
  contextPtr->CopyResource(copyTarget.Get(), m_2dTexture.Get());
}
void Dx11ComputeBuffer::CopyTo(Microsoft::WRL::ComPtr<ID3D11Texture3D> copyTarget)
{
}

void Dx11ComputeBuffer::Barrier()
{
}
void Dx11ComputeBuffer::SetTexture(std::vector<ID3D11ShaderResourceView*> textures)
{
  m_srv = textures;
}