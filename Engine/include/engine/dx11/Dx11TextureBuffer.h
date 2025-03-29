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

  virtual void CreateShaderResourceTexture(std::string file);

  virtual void LoadDDS(std::string file, bool isCubemap = true);

  void CopyFrom(Dx11ComputeBuffer::SharedPtr uav);

  void CopyTo(Microsoft::WRL::ComPtr<ID3D11Texture2D> copyTarget);

  int                                              width, height, channels;
  Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureResourceView;
  Microsoft::WRL::ComPtr<ID3D11Texture2D>          texture;
};