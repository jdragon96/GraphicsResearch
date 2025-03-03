#include "engine/dx11/Dx11GraphicsPSO.h"
#include <filesystem>

class IncludeHandler : public ID3DInclude
{
public:
  HRESULT __stdcall Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName,
    LPCVOID pParentData, LPCVOID* ppData, UINT* pBytes) override
  {
    std::string filePath = "../../resource/" + std::string(pFileName);
    std::ifstream file(filePath, std::ios::binary);
    if (!file) return E_FAIL;

    file.seekg(0, std::ios::end);
    *pBytes = static_cast<UINT>(file.tellg());
    file.seekg(0, std::ios::beg);

    char* data = new char[*pBytes];
    file.read(data, *pBytes);
    file.close();

    *ppData = data;
    return S_OK;
  }

  HRESULT __stdcall Close(LPCVOID pData) override
  {
    delete[] pData;
    return S_OK;
  }
};

void Dx11GraphicsPSO::operator=(const Dx11GraphicsPSO& pso)
{
  m_vertexShader      = pso.m_vertexShader;
  m_pixelShader       = pso.m_pixelShader;
  m_hullShader        = pso.m_hullShader;
  m_domainShader      = pso.m_domainShader;
  m_geometryShader    = pso.m_geometryShader;
  m_inputLayout       = pso.m_inputLayout;
  m_blendState        = pso.m_blendState;
  m_depthStencilState = pso.m_depthStencilState;
  m_rasterizerState   = pso.m_rasterizerState;
  m_stencilRef        = pso.m_stencilRef;
  for (int i = 0; i < 4; i++)
    m_blendFactor[i] = pso.m_blendFactor[i];
  m_primitiveTopology = pso.m_primitiveTopology;
}

void Dx11GraphicsPSO::SetBlendFactor(const float blendFactor[4])
{
  memcpy(m_blendFactor, blendFactor, sizeof(float) * 4);
}
void Dx11GraphicsPSO::Bind()
{
  auto contextPtr = Dx11EngineManager::instance().GetContextPtr();
  contextPtr->IASetPrimitiveTopology(m_primitiveTopology);
  if (m_inputLayout)
    contextPtr->IASetInputLayout(this->m_inputLayout.Get());
  if (m_vertexShader)
    contextPtr->VSSetShader(this->m_vertexShader.Get(), 0, 0);
  else
    contextPtr->VSSetShader(nullptr, 0, 0);

  if (m_pixelShader)
    contextPtr->PSSetShader(this->m_pixelShader.Get(), 0, 0);
  else
    contextPtr->PSSetShader(nullptr, 0, 0);
}
void Dx11GraphicsPSO::SetVertexShader(std::string path, std::vector<D3D11_INPUT_ELEMENT_DESC> elements,
                                  const std::vector<D3D_SHADER_MACRO> shaderMacros)
{
  auto vtxPath   = std::wstring().assign(path.begin(), path.end());
  auto devicePtr = Dx11EngineManager::instance().GetDevicePtr();

  Microsoft::WRL::ComPtr<ID3DBlob> shaderBlob;
  Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;
  UINT                             compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
  compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
  std::string code = ReadCode(path);

  // 1. 셰이더 코드 컴파일하기
  IncludeHandler includeHandler;
  HRESULT hr =
      D3DCompile(code.c_str(), code.length(), nullptr, shaderMacros.empty() ? NULL : shaderMacros.data(),
                 &includeHandler, "main", "vs_5_0", compileFlags, 0, &shaderBlob, &errorBlob);
  CheckCompile(hr, errorBlob.Get());

  // 2. 셰이더 버퍼 생성하기
  devicePtr->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL,
                                &this->m_vertexShader);

  // 3. 레이아웃 버퍼 생성하기
  devicePtr->CreateInputLayout(elements.data(), UINT(elements.size()), shaderBlob->GetBufferPointer(),
                               shaderBlob->GetBufferSize(), &this->m_inputLayout);
  // std::vector<D3D11_INPUT_ELEMENT_DESC> inputElem = {
  //  { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  //  { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(float) * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  //  { "TEXTURECOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(float) * 7, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  //  { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(float) * 9, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  //  { "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(float) * 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  //  { "BLENDWEIGHT", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(float) * 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  //  { "BLENDINDICES", 0, DXGI_FORMAT_R8G8B8A8_UINT, 0, sizeof(float) * 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  //  { "BLENDINDICES", 1, DXGI_FORMAT_R8G8B8A8_UINT, 0, sizeof(float) * 20 + 4, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  //};
}
void Dx11GraphicsPSO::SetPixelShader(std::string path)
{
  auto pxlPath   = std::wstring().assign(path.begin(), path.end());
  auto devicePtr = Dx11EngineManager::instance().GetDevicePtr();

  Microsoft::WRL::ComPtr<ID3DBlob> shaderBlob;
  Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;
  UINT                             compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
  compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
  std::string code = ReadCode(path);

  // 주의: 쉐이더의 시작점의 이름이 "main"인 함수로 지정
  IncludeHandler includeHandler;
  HRESULT hr = D3DCompile(code.c_str(), code.length(), nullptr, nullptr, &includeHandler, "main",
                          "ps_5_0", compileFlags, 0, &shaderBlob, &errorBlob);

  CheckCompile(hr, errorBlob.Get());
  hr = devicePtr->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL,
                                    &this->m_pixelShader);
  Dx11EngineManager::Check(hr);
}
void Dx11GraphicsPSO::SetGeometryShader(std::string path)
{
}
void Dx11GraphicsPSO::SetHullShader(std::string path)
{
}
void Dx11GraphicsPSO::SetDomainShader(std::string path)
{
}
std::string Dx11GraphicsPSO::ReadCode(std::string path)
{
  std::filesystem::path p(path);
  if (!std::filesystem::exists(p))
    throw std::exception(ResourceUtils::NotExistMsg("File", path).c_str());

  std::ifstream loader;
  loader.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  loader.open(path.c_str());
  std::stringstream codeStream;
  codeStream << loader.rdbuf();
  loader.close();
  return codeStream.str();
}
void Dx11GraphicsPSO::CheckCompile(HRESULT hr, ID3DBlob* errorBlob)
{
  if (FAILED(hr))
  {
    // 파일이 없을 경우
    if ((hr & D3D11_ERROR_FILE_NOT_FOUND) != 0)
    {
      std::cout << "File not found." << std::endl;
    }

    // 에러 메시지가 있으면 출력
    if (errorBlob)
    {
      std::cout << "Shader compile error\n" << (char*)errorBlob->GetBufferPointer() << std::endl;
    }
  }
}