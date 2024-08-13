#include <libEngine/dx11/dxShaderBuffer.h>

namespace libEngine
{
dxShaderBuffer::dxShaderBuffer() : ShaderBufferBase()
{
}
dxShaderBuffer::~dxShaderBuffer()
{
}
void dxShaderBuffer::UpdateMat4(std::string name, Mat4* data)
{
  if (name == "projection")
    vertexConstBuffer->data.projection = data->transpose();
  else if (name == "view")
    vertexConstBuffer->data.view = data->transpose();
  else if (name == "model")
    vertexConstBuffer->data.model = data->transpose();
}
void dxShaderBuffer::UpdateVec3(std::string name, Vec3* data)
{
}
void dxShaderBuffer::UpdateVec4(std::string name, Vec4* data)
{
}
void dxShaderBuffer::Bound()
{
  auto contextPtr = dxRenderer::instance()->GetContextPtr();
  contextPtr->IASetInputLayout(inputLayout.Get());
  contextPtr->VSSetShader(vertexShader.Get(), 0, 0);
  contextPtr->PSSetShader(pixelShader.Get(), 0, 0);
  vertexConstBuffer->Update();
  pixelConstBuffer->Update();
}
void dxShaderBuffer::InitBuffers()
{
  InitVertexShader();
  InitPixelShader();
  vertexConstBuffer = dxConstantBuffer<VertexShaderModel>::MakeShared();
  pixelConstBuffer  = dxConstantBuffer<PixelShaderModel>::MakeShared();
  vertexConstBuffer->Initialize(ConstantBufferType::VERTEX);
  pixelConstBuffer->Initialize(ConstantBufferType::PIXEL);
}
void dxShaderBuffer::InitVertexShader()
{
  auto vtxPath   = std::wstring().assign(m_vertexShaderPath.begin(), m_vertexShaderPath.end());
  auto devicePtr = dxRenderer::instance()->GetDevicePtr();

  Microsoft::WRL::ComPtr<ID3DBlob> shaderBlob;
  Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;
  UINT                             compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
  compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

  // 주의: 쉐이더의 시작점의 이름이 "main"인 함수로 지정
  HRESULT hr = D3DCompileFromFile(vtxPath.c_str(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", compileFlags,
                                  0, &shaderBlob, &errorBlob);

  CheckResult(hr, errorBlob.Get());

  std::vector<D3D11_INPUT_ELEMENT_DESC> inputElem = {
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(float) * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEXTURECOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(float) * 7, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(float) * 9, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  };

  devicePtr->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &vertexShader);
  devicePtr->CreateInputLayout(inputElem.data(), UINT(inputElem.size()), shaderBlob->GetBufferPointer(),
                               shaderBlob->GetBufferSize(), &inputLayout);
}
void dxShaderBuffer::InitPixelShader()
{
  auto pxlPath   = std::wstring().assign(m_pixelShaderPath.begin(), m_pixelShaderPath.end());
  auto devicePtr = dxRenderer::instance()->GetDevicePtr();

  Microsoft::WRL::ComPtr<ID3DBlob> shaderBlob;
  Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;
  UINT                             compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
  compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

  // 주의: 쉐이더의 시작점의 이름이 "main"인 함수로 지정
  HRESULT hr = D3DCompileFromFile(pxlPath.c_str(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", compileFlags,
                                  0, &shaderBlob, &errorBlob);

  CheckResult(hr, errorBlob.Get());
  devicePtr->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &pixelShader);
}
void dxShaderBuffer::CheckResult(HRESULT hr, ID3DBlob* errorBlob)
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
}  // namespace libEngine