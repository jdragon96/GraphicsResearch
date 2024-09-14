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

  // g_appBase�� �̿��ؼ� ���������� ��� �Լ� ȣ��
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

      //> ���� ������ ����
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
  // ���ٸ� ������ ��¥ ��ũ�� ũ��� ����
  AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, false);
  // �����츦 ���鶧 ������ ����� wr ���
  m_mainWindow = CreateWindow(wc.lpszClassName, wName.c_str(), WS_OVERLAPPEDWINDOW,
                              100,                 // ������ ���� ����� x ��ǥ
                              100,                 // ������ ���� ����� y ��ǥ
                              wr.right - wr.left,  // ������ ���� ���� �ػ�
                              wr.bottom - wr.top,  // ������ ���� ���� �ػ�
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

  const D3D_FEATURE_LEVEL featureLevels[2] = { D3D_FEATURE_LEVEL_11_0,  // �� ���� ������ ���� ������ ����
                                               D3D_FEATURE_LEVEL_9_3 };
  D3D_FEATURE_LEVEL       featureLevel;

  // 1. Device ����, Driver ���� �׽�Ʈ
  HRESULT hr = D3D11CreateDevice(
      // �⺻ ��� ��� �� nullptr �Է�
      nullptr,
      // ����̹� Ÿ�� ����
      driverType,
      // ����Ʈ���� �׶��̹��� �ƴ� �̻� ������ 0���� ���� �� ��
      0,
      // �����, 2D ȣȯ�� �÷���
      createDeviceFlags,
      // �ش� ����̽��� �����ϴ� Dx ����
      featureLevels,
      // Dx ������ ����Ʈ ������(9)
      ARRAYSIZE(featureLevels),
      // Microsoft app store�� �ø� �� ���Ǵ� �׸�
      D3D11_SDK_VERSION,
      // Direct3D ����̽� ��� ����
      &device,
      // ����̽��� ������ Dx ����
      &featureLevel,
      // Device�� context
      &context);
  if (FAILED(hr))
  {
    throw std::exception("D3D11CreateDevice() failed.");
  }

  // ����: Immediate vs deferred context
  // A deferred context is primarily used for multithreading and is not necessary for a
  // single-threaded application.
  // https://learn.microsoft.com/en-us/windows/win32/direct3d11/overviews-direct3d-11-devices-intro#deferred-context
  // 2. 4X MSAA �����ϴ��� Ȯ��
  UINT numQualityLevels;
  device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &numQualityLevels);
  if (numQualityLevels <= 0)
  {
    // MSAA not supported.
  }

  // 3. Swapchain ����
  DXGI_SWAP_CHAIN_DESC sd;
  ZeroMemory(&sd, sizeof(sd));
  //> ����� ũ�� ����
  sd.BufferDesc.Width  = m_option.width;
  sd.BufferDesc.Height = m_option.height;
  //> ����� ���� ������ Ÿ��(32��Ʈ �÷�)
  sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  //> ���� ���۸�
  sd.BufferCount                        = 2;
  sd.BufferDesc.RefreshRate.Numerator   = 60;
  sd.BufferDesc.RefreshRate.Denominator = 1;
  //> ����ü�� ��� ��� ����(����Ÿ���� ������� ���)
  // 1. �Է� ���
  // 2. ��� ���
  sd.BufferUsage  = DXGI_USAGE_SHADER_INPUT | DXGI_USAGE_RENDER_TARGET_OUTPUT;
  sd.OutputWindow = m_mainWindow;
  //> Ǯ ��ũ�� ���
  sd.Windowed = TRUE;
  //> ��ũ�� ũ�� ������ ����Ѵ�.
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
    // 1. ������ ���۸� ������� ���
    auto result = m_device->CreateRenderTargetView(buffer.Get(), nullptr, renderTarget.GetAddressOf());
    if (FAILED(result))
      throw std::exception("ERROR CreateRenderTargetView");

    // 2. ������ ���۸� ���̴� �Է¿� �ؽ�ó�� ���
    // back buffer�� �ִ� �̹����� �ٸ� ���ۿ� �־�� ��
    // result = m_device->CreateShaderResourceView(buffer.Get(), nullptr, shaderResource.GetAddressOf());
    // if (FAILED(result))
    //  throw std::exception("ERROR CreateShaderResouorceView");

    // 2.2. shader resource�� backbuffer�� �ƴ� Texture2D���� �����ϵ��� �Ѵ�.
    // Texture2DMS�� Texture2D�� ��ȯ�ؼ� ShaderResource�� ����ϸ� �˴ϴ�.
    D3D11_TEXTURE2D_DESC desc;
    buffer->GetDesc(&desc);
    desc.SampleDesc.Count   = 1;
    desc.SampleDesc.Quality = 0;
    desc.BindFlags          = D3D11_BIND_SHADER_RESOURCE;
    desc.MiscFlags          = 0;

    if (FAILED(m_device->CreateTexture2D(&desc, nullptr, m_tempTexture.GetAddressOf())))
      throw std::exception("ERROR CreateShaderResouorceView");

    // ShaderResource�� (backBuffer�� �ƴ϶�) tempTexture�κ��� ����
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
  // �ּ� ����
  viewPort.MinDepth = 0.0f;
  // �ִ� ����
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
  rastDesc.DepthClipEnable       = true;  // <- zNear, zFar Ȯ�ο� �ʿ�
  m_device->CreateRasterizerState(&rastDesc, rasterizerState.GetAddressOf());
}

void dxRenderer::InitDepthBuffer(Microsoft::WRL::ComPtr<ID3D11Texture2D>&        depthBuffer,
                                 Microsoft::WRL::ComPtr<ID3D11DepthStencilView>& depthView)
{
  // 4X MSAA �����ϴ��� Ȯ��
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
    // Anti aliasing �����ϴ� ���, ��Ƽ���� ������ 4���� ����
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