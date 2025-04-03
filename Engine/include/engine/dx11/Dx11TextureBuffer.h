#pragma once

#include <string>
#include <filesystem>
#include <iostream>
#include <exception>

#include "directxtk/DDSTextureLoader.h"
#include "engine/dx11/Dx11EngineManager.h"
#include "engine/dx11/Dx11ComputeBuffer.h"
#include "engine/interface/TextureBufferBase.h"

class Dx11TextureBuffer : TextureBufferBase
{
public:
  SHARED_PTR(Dx11TextureBuffer);

  /***
   * Staging Texture
   * - CPU, GPU 모두 접근이 가능한 텍스쳐
   * - CPU에서 알고리즘 수행을 위해 활용
   */
  void CreateStagingTexture(std::string file, int mipLevel = 0, int arraySize = 1);

  void CreateMipmapTexture(Dx11TextureBuffer::SharedPtr stagingTexture);

  void CreateUATexture(int height, int width,
                       const DXGI_FORMAT pixelFormat = DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT);

  virtual void CreateShaderResourceTexture(std::string file);

  virtual void LoadDDS(std::string file, bool isCubemap = true);

  void CopyFrom(Dx11ComputeBuffer::SharedPtr uav);

  void CopyTo(Microsoft::WRL::ComPtr<ID3D11Texture2D> copyTarget);

  void UseUAV(int index)
  {
    auto contextPtr = Dx11EngineManager::instance().GetContextPtr();
    contextPtr->CSSetUnorderedAccessViews(index, 1, textureUAV.GetAddressOf(), NULL);
  }
  void UnuseUAV(int index)
  {
    auto contextPtr = Dx11EngineManager::instance().GetContextPtr();
    contextPtr->CSSetUnorderedAccessViews(index, 1, textureUAV.GetAddressOf(), NULL);
  }
  void UseSRV(int index)
  {
    auto contextPtr = Dx11EngineManager::instance().GetContextPtr();
    contextPtr->CSSetShaderResources(index, 1, textureResourceView.GetAddressOf());
    contextPtr->VSSetShaderResources(index, 1, textureResourceView.GetAddressOf());
    contextPtr->PSSetShaderResources(index, 1, textureResourceView.GetAddressOf());
  }
  void UseRenderTarget(int index)
  {
    auto contextPtr = Dx11EngineManager::instance().GetContextPtr();
    // const float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    // contextPtr->ClearRenderTargetView(textureRTV.Get(), clearColor);
    contextPtr->OMSetRenderTargets(index, textureRTV.GetAddressOf(), NULL);
  }
  void RenderCompute(int GroupX, int GroupY, int GroupZ)
  {
    auto contextPtr = Dx11EngineManager::instance().GetContextPtr();
    contextPtr->Dispatch(GroupX, GroupY, GroupZ);
    ID3D11ShaderResourceView* nullSRV[12] = {
      0,
    };
    ID3D11UnorderedAccessView* nullUAV[12] = {
      0,
    };
    contextPtr->CSSetUnorderedAccessViews(0, 12, nullUAV, NULL);
    contextPtr->VSSetShaderResources(0, 12, nullSRV);
    contextPtr->PSSetShaderResources(0, 12, nullSRV);
    contextPtr->CSSetShaderResources(0, 12, nullSRV);
  }

  int width, height, channels;

  Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> textureUAV;
  Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>  textureResourceView;
  Microsoft::WRL::ComPtr<ID3D11RenderTargetView>    textureRTV;
  Microsoft::WRL::ComPtr<ID3D11Texture2D>           texture;
};