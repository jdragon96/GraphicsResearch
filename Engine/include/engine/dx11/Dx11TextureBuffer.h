#pragma once

#include <string>
#include <filesystem>
#include <iostream>
#include <exception>

#include "directxtk/DDSTextureLoader.h"
#include "engine/dx11/Dx11EngineManager.h"

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
  void CreateStagingTexture(std::string file, int mipLevel = 0, int arraySize = 1)
  {
    unsigned char*       img = stbi_load(file.c_str(), &width, &height, &channels, 0);
    std::vector<uint8_t> image;
    image.resize(width * height * 4);
    for (size_t i = 0; i < width * height; i++)
    {
      for (size_t c = 0; c < 3; c++)
      {
        image[4 * i + c] = img[i * channels + c];
      }
      image[4 * i + 3] = 255;
    }

    D3D11_TEXTURE2D_DESC txtDesc;
    ZeroMemory(&txtDesc, sizeof(txtDesc));
    txtDesc.Width            = width;
    txtDesc.Height           = height;
    txtDesc.MipLevels        = mipLevel;
    txtDesc.ArraySize        = arraySize;
    txtDesc.Format           = DXGI_FORMAT_R8G8B8A8_UNORM;
    txtDesc.SampleDesc.Count = 1;
    // Staing 용도로 활용
    txtDesc.Usage = D3D11_USAGE_STAGING;
    // CPU write, read 허용
    txtDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;

    auto devicePtr  = Dx11EngineManager::instance().m_device;
    auto contextPtr = Dx11EngineManager::instance().GetContextPtr();
    devicePtr->CreateTexture2D(&txtDesc, nullptr, texture.GetAddressOf());
    // CPU에서 이미지 데이터 복사
    D3D11_MAPPED_SUBRESOURCE ms;
    contextPtr->Map(texture.Get(), NULL, D3D11_MAP_WRITE, NULL, &ms);
    uint8_t* pData = (uint8_t*)ms.pData;
    for (UINT h = 0; h < UINT(height); h++)
    {
      // 데이터를 한 줄씩 복사
      memcpy(&pData[h * ms.RowPitch], &image[h * width * 4], width * sizeof(uint8_t) * 4);
    }
    contextPtr->Unmap(texture.Get(), NULL);
  }

  void CreateMipmapTexture(Dx11TextureBuffer::SharedPtr stagingTexture)
  {
    D3D11_TEXTURE2D_DESC txtDesc;
    ZeroMemory(&txtDesc, sizeof(txtDesc));
    txtDesc.Width            = stagingTexture->width;
    txtDesc.Height           = stagingTexture->height;
    txtDesc.MipLevels        = 0;  // 밉맵 레벨 최대
    txtDesc.ArraySize        = 1;
    txtDesc.Format           = DXGI_FORMAT_R8G8B8A8_UNORM;
    txtDesc.SampleDesc.Count = 1;
    txtDesc.Usage            = D3D11_USAGE_DEFAULT;  // 스테이징 텍스춰로부터 복사 가능
    txtDesc.BindFlags        = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
    txtDesc.MiscFlags        = D3D11_RESOURCE_MISC_GENERATE_MIPS;  // 밉맵 사용
    txtDesc.CPUAccessFlags   = 0;  // GPU에서 데이터를 카피하기 때문에 0으로
    {
      auto devicePtr  = Dx11EngineManager::instance().m_device;
      auto contextPtr = Dx11EngineManager::instance().GetContextPtr();
      devicePtr->CreateTexture2D(&txtDesc, nullptr, texture.GetAddressOf());
      // Staging texture로부터 이미지를 카피
      // 2 번째 인자 : Mipmap의 level을 의미... 가장 큰 level에 값을 카피함을 의미
      contextPtr->CopySubresourceRegion(texture.Get(), 0, 0, 0, 0, stagingTexture->texture.Get(), 0, nullptr);
      // ResourceView 만들기
      devicePtr->CreateShaderResourceView(texture.Get(), 0, textureResourceView.GetAddressOf());
      // 해상도를 낮춰가며 밉맵 생성
      contextPtr->GenerateMips(textureResourceView.Get());
    }
  }

  virtual void CreateShaderResourceTexture(std::string file)
  {
    unsigned char* img = stbi_load(file.c_str(), &width, &height, &channels, 0);

    // assert(channels == 4);

    // 4채널로 만들어서 복사
    std::vector<uint8_t> image;
    image.resize(width * height * 4);
    for (size_t i = 0; i < width * height; i++)
    {
      for (size_t c = 0; c < 3; c++)
      {
        image[4 * i + c] = img[i * channels + c];
      }
      image[4 * i + 3] = 255;
    }

    // Create texture.
    D3D11_TEXTURE2D_DESC txtDesc = {};
    txtDesc.Width                = width;
    txtDesc.Height               = height;
    txtDesc.MipLevels = txtDesc.ArraySize = 1;
    txtDesc.Format                        = DXGI_FORMAT_R8G8B8A8_UNORM;
    // txtDesc.Format                        = DXGI_FORMAT_R16G16B16A16_FLOAT;
    txtDesc.SampleDesc.Count = 1;
    txtDesc.Usage            = D3D11_USAGE_IMMUTABLE;
    txtDesc.BindFlags        = D3D11_BIND_SHADER_RESOURCE;

    // Fill in the subresource data.
    D3D11_SUBRESOURCE_DATA initData;
    initData.pSysMem     = image.data();
    initData.SysMemPitch = txtDesc.Width * sizeof(uint8_t) * 4;
    // initData.SysMemSlicePitch = 0;
    auto devicePtr = Dx11EngineManager::instance().m_device;
    devicePtr->CreateTexture2D(&txtDesc, &initData, texture.GetAddressOf());
    devicePtr->CreateShaderResourceView(texture.Get(), nullptr, textureResourceView.GetAddressOf());
  }

  virtual void LoadDDS(std::string file, bool isCubemap = true)
  {
    auto texturePath = std::wstring().assign(file.begin(), file.end());
    UINT miscFlags   = 0;
    if (isCubemap)
    {
      miscFlags |= D3D11_RESOURCE_MISC_TEXTURECUBE;
    }
    // https://github.com/microsoft/DirectXTK/wiki/DDSTextureLoader
    auto hr = DirectX::CreateDDSTextureFromFileEx(Dx11EngineManager::instance().m_device.Get(), texturePath.c_str(), 0,
                                                  D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0,
                                                  miscFlags,  // 큐브맵용 텍스춰
                                                  DirectX::DX11::DDS_LOADER_FLAGS(false),
                                                  (ID3D11Resource**)texture.GetAddressOf(),
                                                  textureResourceView.GetAddressOf(), nullptr);

    if (FAILED(hr))
    {
      std::cout << "CreateDDSTextureFromFileEx() failed" << std::endl;
    }
  }

  int                                              width, height, channels;
  Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureResourceView;
  Microsoft::WRL::ComPtr<ID3D11Texture2D>          texture;
};