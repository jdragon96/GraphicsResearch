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

	virtual void LoadDDS(std::string file)
	{
		auto texturePath = std::wstring().assign(file.begin(), file.end());
		// https://github.com/microsoft/DirectXTK/wiki/DDSTextureLoader
		auto hr = DirectX::CreateDDSTextureFromFileEx(
			Dx11EngineManager::instance().m_device.Get(),
			texturePath.c_str(), 
			0, 
			D3D11_USAGE_DEFAULT,
			D3D11_BIND_SHADER_RESOURCE, 
			0,
			D3D11_RESOURCE_MISC_TEXTURECUBE, // Å¥ºê¸Ê¿ë ÅØ½ºÃç
			DirectX::DX11::DDS_LOADER_FLAGS(false), 
			(ID3D11Resource**)texture.GetAddressOf(),
			textureResourceView.GetAddressOf(), 
			nullptr);

		if (FAILED(hr)) {
			std::cout << "CreateDDSTextureFromFileEx() failed" << std::endl;
		}
	}

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureResourceView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
};