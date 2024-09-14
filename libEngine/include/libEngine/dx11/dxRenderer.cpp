#include "libEngine/dx11/dxRenderer.h"
#include <iostream>

namespace libEngine
{
// dxRenderer* rendererPtr = nullptr;

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  // if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
  //   return true;
  auto rendererPtr = dxRenderer::instance();
  if (rendererPtr == nullptr)
    return false;

  int zDelta;
  switch (msg)
  {
    case WM_SIZE:
      // Reset and resize swapchain
      break;
    case WM_SYSCOMMAND:
      if ((wParam & 0xfff0) == SC_KEYMENU)  // Disable ALT application menu
        return 0;
      break;
    case WM_MOUSEMOVE:
      if (!rendererPtr->IsMousePressed())
        break;
      switch (rendererPtr->m_buttonType)
      {
        case 0:
          rendererPtr->m_camPtr->UpdateOrbit(LOWORD(lParam) - rendererPtr->prevCursorX,
                                             HIWORD(lParam) - rendererPtr->prevCursorY);
          break;
        case 1:
          rendererPtr->m_camPtr->UpdateTranslate(LOWORD(lParam) - rendererPtr->prevCursorX,
                                                 HIWORD(lParam) - rendererPtr->prevCursorY);
          break;
      }
      rendererPtr->prevCursorX = LOWORD(lParam);
      rendererPtr->prevCursorY = HIWORD(lParam);
      break;
    case WM_LBUTTONUP:
      rendererPtr->UpdateMousePressStatus(false, -1);
      break;
    case WM_LBUTTONDOWN:
      rendererPtr->prevCursorX = LOWORD(lParam);
      rendererPtr->prevCursorY = HIWORD(lParam);
      rendererPtr->UpdateMousePressStatus(true, 0);
      break;
    case WM_RBUTTONUP:
      rendererPtr->UpdateMousePressStatus(false, -1);
      break;
    case WM_RBUTTONDOWN:
      rendererPtr->prevCursorX = LOWORD(lParam);
      rendererPtr->prevCursorY = HIWORD(lParam);
      rendererPtr->UpdateMousePressStatus(true, 1);
      break;
    case WM_KEYDOWN:
      break;
    case WM_MOUSEWHEEL:
      zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
      if (zDelta > 0)
        rendererPtr->m_camPtr->UpdateZoom(true);
      else
        rendererPtr->m_camPtr->UpdateZoom(false);
      break;
    case WM_DESTROY:
      ::PostQuitMessage(0);
      return 0;
  }

  // g_appBase를 이용해서 간접적으로 멤버 함수 호출
  // return g_appBase->MsgProc(hWnd, msg, wParam, lParam);
  return DefWindowProc(hWnd, msg, wParam, lParam);
}

dxRenderer::dxRenderer()
{
  // rendererPtr = this;
}
dxRenderer::~dxRenderer()
{
}
void dxRenderer::Initialize()
{
  InitWindow();
  InitDeviceAndSwapChain();
  InitRenderingTarget(m_mainRenderTarget, m_mainShaderResource, m_renderBuffer);
  InitViewPort(m_mainScreenViewport);
  InitRasterizerState(m_mainRasterizationMode);
  InitDepthBuffer(m_mainDepthBuffer, m_mainDepthView);
  InitDepthStencilState(m_mainDepthState);
  InitSampler(m_mainSamplerState);
}
void dxRenderer::Run()
{
  prevFunc();
  m_context->RSSetViewports(1, &m_mainScreenViewport);
  MSG msg = { 0 };
  while (WM_QUIT != msg.message)
  {
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    else
    {
      // IA: Input-Assembler stage
      // VS: Vertex Shader
      // PS: Pixel Shader
      // RS: Rasterizer stage
      // OM: Output-Merger stage

      float clearColor[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
      m_context->ClearRenderTargetView(m_mainRenderTarget.Get(), clearColor);
      m_context->ClearDepthStencilView(m_mainDepthView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
      m_context->OMSetDepthStencilState(m_mainDepthState.Get(), 0);
      m_context->OMSetRenderTargets(1, m_mainRenderTarget.GetAddressOf(), m_mainDepthView.Get());
      m_context->RSSetState(m_mainRasterizationMode.Get());
      m_context->PSSetSamplers(0, 1, m_mainSamplerState.GetAddressOf());

      //> 메인 랜더링 루프
      renderFunc();
      Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
      dxRenderer::instance()->m_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf()));
      dxRenderer::instance()->m_context->ResolveSubresource(dxRenderer::instance()->m_tempTexture.Get(), 0,
                                                            backBuffer.Get(), 0, DXGI_FORMAT_R8G8B8A8_UNORM);
      postProcessingFunc();
      m_swapChain->Present(1, 0);
    }
  }
}
void dxRenderer::InitWindow()
{
  std::wstring wName;
  wName.assign(m_option.title.begin(), m_option.title.end());

  WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
                    wName.c_str(),      NULL };

  if (!RegisterClassEx(&wc))
  {
    throw std::exception("RegisterClassEx() failed");
  }

  RECT wr = { 0, 0, m_option.width, m_option.height };
  // 툴바를 제외한 진짜 스크린 크기로 조정
  AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, false);
  // 윈도우를 만들때 위에서 계산한 wr 사용
  m_mainWindow = CreateWindow(wc.lpszClassName, wName.c_str(), WS_OVERLAPPEDWINDOW,
                              100,                 // 윈도우 좌측 상단의 x 좌표
                              100,                 // 윈도우 좌측 상단의 y 좌표
                              wr.right - wr.left,  // 윈도우 가로 방향 해상도
                              wr.bottom - wr.top,  // 윈도우 세로 방향 해상도
                              NULL, NULL, wc.hInstance, NULL);
  if (!m_mainWindow)
  {
    throw std::exception("CreateWindow() failed. => \n" + GetLastError());
  }

  ShowWindow(m_mainWindow, SW_SHOWDEFAULT);
  UpdateWindow(m_mainWindow);
}

void dxRenderer::InitDeviceAndSwapChain()
{
  UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
  createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
  auto driverType = D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE;

  Microsoft::WRL::ComPtr<ID3D11Device>        device;
  Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;

  const D3D_FEATURE_LEVEL featureLevels[2] = { D3D_FEATURE_LEVEL_11_0,  // 더 높은 버전이 먼저 오도록 설정
                                               D3D_FEATURE_LEVEL_9_3 };
  D3D_FEATURE_LEVEL       featureLevel;

  // 1. Device 생성, Driver 버전 테스트
  HRESULT hr = D3D11CreateDevice(
      // 기본 어뎁터 사용 시 nullptr 입력
      nullptr,
      // 드라이버 타입 결정
      driverType,
      // 소프트웨어 그라이버가 아닌 이상 무조건 0으로 설정 할 것
      0,
      // 디버그, 2D 호환성 플레그
      createDeviceFlags,
      // 해당 디바이스가 지원하는 Dx 버전
      featureLevels,
      // Dx 버전의 리스트 사이즈(9)
      ARRAYSIZE(featureLevels),
      // Microsoft app store에 올릴 때 사용되는 항목
      D3D11_SDK_VERSION,
      // Direct3D 디바이스 출력 인자
      &device,
      // 디바이스가 지원할 Dx 버전
      &featureLevel,
      // Device의 context
      &context);
  if (FAILED(hr))
  {
    throw std::exception("D3D11CreateDevice() failed.");
  }

  // 참고: Immediate vs deferred context
  // A deferred context is primarily used for multithreading and is not necessary for a
  // single-threaded application.
  // https://learn.microsoft.com/en-us/windows/win32/direct3d11/overviews-direct3d-11-devices-intro#deferred-context
  // 2. 4X MSAA 지원하는지 확인
  UINT numQualityLevels;
  device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &numQualityLevels);
  if (numQualityLevels <= 0)
  {
    // MSAA not supported.
  }

  // 3. Swapchain 생성
  DXGI_SWAP_CHAIN_DESC sd;
  ZeroMemory(&sd, sizeof(sd));
  //> 백버퍼 크기 설정
  sd.BufferDesc.Width  = m_option.width;
  sd.BufferDesc.Height = m_option.height;
  //> 백버퍼 셀의 데이터 타입(32비트 컬러)
  sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  //> 더블 버퍼링
  sd.BufferCount                        = 2;
  sd.BufferDesc.RefreshRate.Numerator   = 60;
  sd.BufferDesc.RefreshRate.Denominator = 1;
  //> 스왑체인 사용 방식 결정(랜더타깃의 출력으로 사용)
  // 1. 입력 허용
  // 2. 출력 허용
  sd.BufferUsage  = DXGI_USAGE_SHADER_INPUT | DXGI_USAGE_RENDER_TARGET_OUTPUT;
  sd.OutputWindow = m_mainWindow;
  //> 풀 스크린 모드
  sd.Windowed = TRUE;
  //> 스크린 크기 변경을 허용한다.
  sd.Flags      = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
  sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

  if (numQualityLevels > 0)
  {
    sd.SampleDesc.Count   = 4;
    sd.SampleDesc.Quality = numQualityLevels - 1;
  }
  else
  {
    sd.SampleDesc.Count   = 1;
    sd.SampleDesc.Quality = 0;
  }

  if (FAILED(device.As(&m_device)))
    throw std::exception("device.AS() failed");

  if (FAILED(context.As(&m_context)))
    throw std::exception("context.As() failed");

  if (FAILED(D3D11CreateDeviceAndSwapChain(0,  // Default adapter
                                           driverType,
                                           0,  // No software device
                                           createDeviceFlags, featureLevels, 1, D3D11_SDK_VERSION, &sd, &m_swapChain,
                                           &m_device, &featureLevel, &m_context)))
  {
    throw std::exception("D3D11CreateDeviceAndSwapChain() failed");
  }
}

void dxRenderer::InitRenderingTarget(Microsoft::WRL::ComPtr<ID3D11RenderTargetView>&   renderTarget,
                                     Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& shaderResource,
                                     Microsoft::WRL::ComPtr<ID3D11Texture2D>&          buffer)
{
  if ((m_swapChain == nullptr) || (m_device == nullptr))
    throw std::exception("Must be initialized swap chain and device");

  m_swapChain->GetBuffer(0, IID_PPV_ARGS(&buffer));
  // m_swapChain->GetBuffer(0, IID_PPV_ARGS(buffer.GetAddressOf()));
  if (buffer)
  {
    // 1. 랜더링 버퍼를 출력으로 취급
    auto result = m_device->CreateRenderTargetView(buffer.Get(), nullptr, renderTarget.GetAddressOf());
    if (FAILED(result))
      throw std::exception("ERROR CreateRenderTargetView");

    // 2. 랜더링 버퍼를 셰이더 입력용 텍스처로 취급
    // back buffer에 있는 이미지를 다른 버퍼에 넣어야 함
    // result = m_device->CreateShaderResourceView(buffer.Get(), nullptr, shaderResource.GetAddressOf());
    // if (FAILED(result))
    //  throw std::exception("ERROR CreateShaderResouorceView");

    // 2.2. shader resource를 backbuffer가 아닌 Texture2D에서 생성하도록 한다.
    // Texture2DMS를 Texture2D로 변환해서 ShaderResource로 사용하면 됩니다.
    D3D11_TEXTURE2D_DESC desc;
    buffer->GetDesc(&desc);
    desc.SampleDesc.Count   = 1;
    desc.SampleDesc.Quality = 0;
    desc.BindFlags          = D3D11_BIND_SHADER_RESOURCE;
    desc.MiscFlags          = 0;

    if (FAILED(m_device->CreateTexture2D(&desc, nullptr, m_tempTexture.GetAddressOf())))
      throw std::exception("ERROR CreateShaderResouorceView");

    // ShaderResource를 (backBuffer가 아니라) tempTexture로부터 생성
    m_device->CreateShaderResourceView(m_tempTexture.Get(), nullptr, m_mainShaderResource.GetAddressOf());
  }
  else
    throw std::exception("ERROR InitRenderingTarget");
}

void dxRenderer::InitViewPort(D3D11_VIEWPORT& viewPort)
{
  ZeroMemory(&viewPort, sizeof(D3D11_VIEWPORT));
  viewPort.TopLeftX = 0;
  viewPort.TopLeftY = 0;
  viewPort.Width    = float(m_option.width);
  viewPort.Height   = float(m_option.height);
  // 최소 뎁스
  viewPort.MinDepth = 0.0f;
  // 최대 뎁스
  viewPort.MaxDepth = 1.0f;
  m_context->RSSetViewports(1, &viewPort);
}

void dxRenderer::InitRasterizerState(Microsoft::WRL::ComPtr<ID3D11RasterizerState>& rasterizerState,
                                     D3D11_FILL_MODE mode, D3D11_CULL_MODE cull)
{
  D3D11_RASTERIZER_DESC rastDesc;
  ZeroMemory(&rastDesc, sizeof(D3D11_RASTERIZER_DESC));
  rastDesc.FillMode              = mode;
  rastDesc.CullMode              = cull;
  rastDesc.FrontCounterClockwise = false;
  rastDesc.DepthClipEnable       = true;  // <- zNear, zFar 확인에 필요
  m_device->CreateRasterizerState(&rastDesc, rasterizerState.GetAddressOf());
}

void dxRenderer::InitDepthBuffer(Microsoft::WRL::ComPtr<ID3D11Texture2D>&        depthBuffer,
                                 Microsoft::WRL::ComPtr<ID3D11DepthStencilView>& depthView)
{
  // 4X MSAA 지원하는지 확인
  UINT numQualityLevels;
  m_device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &numQualityLevels);
  if (numQualityLevels <= 0)
  {
    // TODO: MSAA not supported
  }

  D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
  depthStencilBufferDesc.Width     = m_option.width;
  depthStencilBufferDesc.Height    = m_option.height;
  depthStencilBufferDesc.MipLevels = 1;
  depthStencilBufferDesc.ArraySize = 1;
  depthStencilBufferDesc.Format    = DXGI_FORMAT_D24_UNORM_S8_UINT;
  if (numQualityLevels > 0)
  {
    // Anti aliasing 지원하는 경우, 멀티셈플 개수를 4개로 설정
    depthStencilBufferDesc.SampleDesc.Count   = 4;
    depthStencilBufferDesc.SampleDesc.Quality = numQualityLevels - 1;
  }
  else
  {
    depthStencilBufferDesc.SampleDesc.Count   = 1;
    depthStencilBufferDesc.SampleDesc.Quality = 0;
  }
  depthStencilBufferDesc.Usage          = D3D11_USAGE_DEFAULT;
  depthStencilBufferDesc.BindFlags      = D3D11_BIND_DEPTH_STENCIL;
  depthStencilBufferDesc.CPUAccessFlags = 0;
  depthStencilBufferDesc.MiscFlags      = 0;

  if (FAILED(m_device->CreateTexture2D(&depthStencilBufferDesc, 0, depthBuffer.GetAddressOf())))
  {
    // TODO: CreateTexture2D() failed
    std::cout << "CreateTexture2D() failed." << std::endl;
  }
  if (FAILED(m_device->CreateDepthStencilView(depthBuffer.Get(), 0, &depthView)))
  {
    // TODO: CreateDepthStencilView() failed
    std::cout << "CreateDepthStencilView() failed." << std::endl;
  }
}

void dxRenderer::InitDepthStencilState(Microsoft::WRL::ComPtr<ID3D11DepthStencilState>& depthState)
{
  D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
  ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
  depthStencilDesc.DepthEnable    = true;
  depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
  depthStencilDesc.DepthFunc      = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
  if (FAILED(m_device->CreateDepthStencilState(&depthStencilDesc, depthState.GetAddressOf())))
  {
    // TODO: CreateDepthStencilState() failed
  }
}

void dxRenderer::InitSampler(Microsoft::WRL::ComPtr<ID3D11SamplerState>& sampler)
{
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
  m_device->CreateSamplerState(&sampDesc, sampler.GetAddressOf());
}
}  // namespace libEngine