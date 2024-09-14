#include <libEngine/dx11/dxTexture.h>

namespace libEngine
{
dxTexture::dxTexture()
{
  auto               devPtr = dxRenderer::instance()->GetDevicePtr();
  D3D11_SAMPLER_DESC sampDesc;
  ZeroMemory(&sampDesc, sizeof(sampDesc));
  sampDesc.Filter         = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
  sampDesc.AddressU       = D3D11_TEXTURE_ADDRESS_WRAP;
  sampDesc.AddressV       = D3D11_TEXTURE_ADDRESS_WRAP;
  sampDesc.AddressW       = D3D11_TEXTURE_ADDRESS_WRAP;
  sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
  sampDesc.MinLOD         = 0;
  sampDesc.MaxLOD         = D3D11_FLOAT32_MAX;

  // Create the Sample State
  devPtr->CreateSamplerState(&sampDesc, m_samplerState.GetAddressOf());
}
dxTexture::~dxTexture()
{
}
void dxTexture::AddImage(std::string path)
{
  std::filesystem::path p(path);
  if (p.extension() == ".dds")
  {
    Microsoft::WRL::ComPtr<ID3D11Texture2D>          texturePtr;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> rsViewPtr;
    auto                                             devPtr = dxRenderer::instance()->GetDevicePtr();

    auto ddsPath = std::wstring().assign(path.begin(), path.end());

    auto hr = DirectX::CreateDDSTextureFromFileEx(
        // this->m_device.Get(), L"./SaintPetersBasilica/saintpeters.dds", 0,
        devPtr.Get(), ddsPath.c_str(), 0, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0,
        D3D11_RESOURCE_MISC_TEXTURECUBE, DirectX::DDS_LOADER_FLAGS(false), (ID3D11Resource**)texturePtr.GetAddressOf(),
        rsViewPtr.GetAddressOf(), nullptr);
    if (FAILED(hr))
    {
      std::cout << "CreateDDSTextureFromFileEx() failed" << std::endl;
    }
    m_texturesPtr.push_back(texturePtr);
    m_resourceViewsPtr.push_back(rsViewPtr);
    m_resourceViewsRawPtr.push_back(rsViewPtr.Get());
  }
  else
  {
    LoadTexture(path, [this](unsigned char* data, int w, int h, int channel) {
      Microsoft::WRL::ComPtr<ID3D11Texture2D>          texturePtr;
      Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> viewPtr;

      std::vector<uint8_t> texture;
      texture.resize(w * h * 4);
      if (channel == 3)
      {
        for (size_t i = 0; i < w * h; i++)
        {
          for (size_t c = 0; c < 3; c++)
          {
            texture[4 * i + c] = data[i * channel + c];
          }
          texture[4 * i + 3] = 255;
        }
      }
      else if (channel == 4)
      {
        for (size_t i = 0; i < w * h; i++)
        {
          for (size_t c = 0; c < 4; c++)
          {
            texture[4 * i + c] = data[i * channel + c];
          }
        }
      }

      D3D11_TEXTURE2D_DESC txtDesc = {};
      txtDesc.Width                = w;
      txtDesc.Height               = h;
      txtDesc.MipLevels = txtDesc.ArraySize = 1;
      txtDesc.Format                        = DXGI_FORMAT_R8G8B8A8_UNORM;
      txtDesc.SampleDesc.Count              = 1;
      txtDesc.Usage                         = D3D11_USAGE_IMMUTABLE;
      txtDesc.BindFlags                     = D3D11_BIND_SHADER_RESOURCE;

      //// Fill in the subresource data.
      D3D11_SUBRESOURCE_DATA InitData;
      InitData.pSysMem     = texture.data();
      InitData.SysMemPitch = txtDesc.Width * sizeof(uint8_t) * 4;
      // InitData.SysMemSlicePitch = txtDesc.Height * txtDesc.Width * sizeof(uint8_t) * 4;

      auto devPtr = dxRenderer::instance()->GetDevicePtr();
      devPtr->CreateTexture2D(&txtDesc, &InitData, texturePtr.GetAddressOf());
      devPtr->CreateShaderResourceView(texturePtr.Get(), nullptr, viewPtr.GetAddressOf());

      m_texturesPtr.push_back(texturePtr);
      m_resourceViewsPtr.push_back(viewPtr);
      m_resourceViewsRawPtr.push_back(viewPtr.Get());
    });
  }
}

void dxTexture::AddCubemap(std::vector<std::string> pathes)
{
}

void dxTexture::AddDDS(std::string path)
{
  Microsoft::WRL::ComPtr<ID3D11Texture2D>          texturePtr;
  Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> rsViewPtr;
  auto                                             devPtr = dxRenderer::instance()->GetDevicePtr();

  auto ddsPath = std::wstring().assign(path.begin(), path.end());

  auto hr = DirectX::CreateDDSTextureFromFileEx(
      // this->m_device.Get(), L"./SaintPetersBasilica/saintpeters.dds", 0,
      devPtr.Get(), ddsPath.c_str(), 0, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0,
      D3D11_RESOURCE_MISC_TEXTURECUBE, DirectX::DDS_LOADER_FLAGS(false), (ID3D11Resource**)texturePtr.GetAddressOf(),
      rsViewPtr.GetAddressOf(), nullptr);
  if (FAILED(hr))
  {
    std::cout << "CreateDDSTextureFromFileEx() failed" << std::endl;
  }
  m_texturesPtr.push_back(texturePtr);
  m_resourceViewsPtr.push_back(rsViewPtr);
  m_resourceViewsRawPtr.push_back(rsViewPtr.Get());
}
void dxTexture::Bound()
{
  auto ctxPtr = dxRenderer::instance()->GetContextPtr();
  ctxPtr->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());
  ctxPtr->PSSetShaderResources(0, m_resourceViewsRawPtr.size(), m_resourceViewsRawPtr.data());
}
}  // namespace libEngine