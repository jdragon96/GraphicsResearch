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
  SHARED_PTR(Dx11TextureBuffer)

  virtual void LoadJPG(std::string file)
  {
    int width, height, channels;

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
    txtDesc.SampleDesc.Count              = 1;
    txtDesc.Usage                         = D3D11_USAGE_IMMUTABLE;
    txtDesc.BindFlags                     = D3D11_BIND_SHADER_RESOURCE;

    // Fill in the subresource data.
    D3D11_SUBRESOURCE_DATA initData;
    initData.pSysMem     = image.data();
    initData.SysMemPitch = txtDesc.Width * sizeof(uint8_t) * 4;
    // initData.SysMemSlicePitch = 0;
    auto devicePtr = Dx11EngineManager::instance().m_device;
    devicePtr->CreateTexture2D(&txtDesc, &initData, texture.GetAddressOf());
    devicePtr->CreateShaderResourceView(texture.Get(), nullptr, textureResourceView.GetAddressOf());
  }

  virtual void LoadDDS(std::string file)
  {
    auto texturePath = std::wstring().assign(file.begin(), file.end());
    // https://github.com/microsoft/DirectXTK/wiki/DDSTextureLoader
    auto hr = DirectX::CreateDDSTextureFromFileEx(Dx11EngineManager::instance().m_device.Get(), texturePath.c_str(), 0,
                                                  D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0,
                                                  D3D11_RESOURCE_MISC_TEXTURECUBE,  // 큐브맵용 텍스춰
                                                  DirectX::DX11::DDS_LOADER_FLAGS(false),
                                                  (ID3D11Resource**)texture.GetAddressOf(),
                                                  textureResourceView.GetAddressOf(), nullptr);

    if (FAILED(hr))
    {
      std::cout << "CreateDDSTextureFromFileEx() failed" << std::endl;
    }
  }

  Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureResourceView;
  Microsoft::WRL::ComPtr<ID3D11Texture2D>          texture;
};