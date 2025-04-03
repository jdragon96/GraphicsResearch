#include "engine/dx11/Dx11EngineManager.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
    return true;

  auto contextPtr = Dx11EngineManager::instance().GetContextPtr();
  if (contextPtr == nullptr)
    return true;

  int  zDelta;
  auto bType = Dx11EngineManager::instance().WhichButtonPressed();

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
      if (!Dx11EngineManager::instance().IsPressed())
        break;
      Dx11EngineManager::instance().UpdateCameraOrbit(LOWORD(lParam), HIWORD(lParam));
      break;
    case WM_LBUTTONUP:
      Dx11EngineManager::instance().UpdatePressState(MouseButtonType::LEFT, false);
      break;
    case WM_LBUTTONDOWN:
      Dx11EngineManager::instance().UpdatePressState(MouseButtonType::LEFT, true);
      Dx11EngineManager::instance().UpdateCameraOrbit(LOWORD(lParam), HIWORD(lParam));
      break;
    case WM_RBUTTONUP:
      Dx11EngineManager::instance().UpdatePressState(MouseButtonType::RIGHT, false);
      break;
    case WM_RBUTTONDOWN:
      Dx11EngineManager::instance().UpdatePressState(MouseButtonType::RIGHT, true);
      Dx11EngineManager::instance().UpdateCameraOrbit(LOWORD(lParam), HIWORD(lParam));
      break;
    case WM_KEYDOWN:
      Dx11EngineManager::instance().m_keyPressed[wParam] = true;
      break;
    case WM_KEYUP:
      Dx11EngineManager::instance().m_keyPressed[wParam] = false;
      break;
    case WM_MOUSEWHEEL:
      zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
      Dx11EngineManager::instance().UpdateCameraZoom(zDelta > 0 ? true : false);
      break;
    case WM_DESTROY:
      ::PostQuitMessage(0);
      return 0;
  }

  Dx11EngineManager::instance().m_currentPosX = LOWORD(lParam);
  Dx11EngineManager::instance().m_currentPosY = HIWORD(lParam);
  return DefWindowProc(hWnd, msg, wParam, lParam);
}

Dx11EngineManager::Dx11EngineManager() : EngineBase()
{
  {
    ZeroMemory(&scOption, sizeof(scOption));
    scOption.BufferDesc.Format                  = DXGI_FORMAT_R16G16B16A16_FLOAT;  // HDR용 버퍼
    scOption.BufferCount                        = 2;                               //> 더블 버퍼링
    scOption.BufferDesc.RefreshRate.Numerator   = 60;
    scOption.BufferDesc.RefreshRate.Denominator = 1;
    //> 스왑체인 사용 방식 결정(랜더타깃의 출력으로 사용)
    // 1. 입력 허용
    // 2. 출력 허용
    scOption.BufferUsage = DXGI_USAGE_SHADER_INPUT | DXGI_USAGE_RENDER_TARGET_OUTPUT;
    // scOption.BufferUsage  = DXGI_USAGE_RENDER_TARGET_OUTPUT | ;
    scOption.Windowed   = TRUE;                                    //> 풀 스크린 모드
    scOption.Flags      = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;  // 스크린 크기 변경을 허용한다.
    scOption.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
  }
  {
    // 메인화면을 셰이더 resoure view로 만들기 위한 임시 텍스쳐
    // ZeroMemory(&tdOption, sizeof(tdOption));
    // tdOption.Format             = DXGI_FORMAT_R8G8B8A8_UNORM;
    // tdOption.Usage              = D3D11_USAGE_DEFAULT;  // 스테이징 텍스춰로부터 복사 가능
    // tdOption.MiscFlags          = 0;
    // tdOption.CPUAccessFlags     = 0;
  }
  {
    ZeroMemory(&rsOption, sizeof(rsOption));
    rsOption.FillMode              = D3D11_FILL_MODE::D3D11_FILL_SOLID;
    rsOption.CullMode              = D3D11_CULL_MODE::D3D11_CULL_NONE;
    rsOption.FrontCounterClockwise = false;
    rsOption.DepthClipEnable       = true;
  }
  {
    ZeroMemory(&dsOption, sizeof(dsOption));
    dsOption.Width          = m_screenOption.width;
    dsOption.Height         = m_screenOption.height;
    dsOption.MipLevels      = 1;
    dsOption.ArraySize      = 1;
    dsOption.Format         = DXGI_FORMAT_D24_UNORM_S8_UINT;
    dsOption.Usage          = D3D11_USAGE_DEFAULT;
    dsOption.BindFlags      = D3D11_BIND_DEPTH_STENCIL;
    dsOption.CPUAccessFlags = 0;
    dsOption.MiscFlags      = 0;
  }
  {
    ZeroMemory(&dssOption, sizeof(dssOption));
    dssOption.DepthEnable      = true;
    dssOption.DepthWriteMask   = D3D11_DEPTH_WRITE_MASK_ALL;
    dssOption.DepthFunc        = D3D11_COMPARISON_LESS;
    dssOption.StencilEnable    = false;  // Stencil 불필요
    dssOption.StencilReadMask  = D3D11_DEFAULT_STENCIL_READ_MASK;
    dssOption.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
    // 앞면에 대해서 어떻게 작동할지 설정
    dssOption.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
    dssOption.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    dssOption.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
    dssOption.FrontFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;
    // 뒷면에 대해 어떻게 작동할지 설정 (뒷면도 그릴 경우)
    dssOption.BackFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
    dssOption.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    dssOption.BackFace.StencilPassOp      = D3D11_STENCIL_OP_REPLACE;
    dssOption.BackFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;
  }
  {
    ZeroMemory(&samplerOption, sizeof(samplerOption));
    samplerOption.Filter         = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerOption.AddressU       = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerOption.AddressV       = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerOption.AddressW       = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerOption.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samplerOption.MinLOD         = 0;
    samplerOption.MaxLOD         = D3D11_FLOAT32_MAX;
  }
}

Dx11EngineManager::~Dx11EngineManager()
{
  ImGui_ImplDX11_Shutdown();
  ImGui_ImplWin32_Shutdown();
  ImGui::DestroyContext();
  DestroyWindow(m_mainWindow);
}
void Dx11EngineManager::Initialize()
{
  InitWindow();
  InitDeviceAndSwapChain(scOption);
  {
    // 백버퍼 랜더타겟
    Check(m_swapChain->GetBuffer(0, IID_PPV_ARGS(&m_backBufferTexture)));
    Check(m_device->CreateRenderTargetView(m_backBufferTexture.Get(), nullptr, m_backBufferRTV.GetAddressOf()));
    D3D11_TEXTURE2D_DESC desc;
    m_backBufferTexture->GetDesc(&desc);
    desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    // 이전 프레임 저장용
    // Check(m_device->CreateTexture2D(&desc, nullptr, m_prevBuffer.GetAddressOf()));
    // Check(m_device->CreateShaderResourceView(m_prevBuffer.Get(), nullptr, m_mainSRV.GetAddressOf()));
    // Check(m_device->CreateRenderTargetView(m_prevBuffer.Get(), NULL, m_prevRTV.GetAddressOf()));
    // float buffer 생성 -> 용도는?
    desc.MipLevels = desc.ArraySize = 1;
    desc.Format                     = DXGI_FORMAT_R16G16B16A16_FLOAT;
    desc.Usage     = D3D11_USAGE_DEFAULT;  // ������¡ �ؽ���κ��� ���� ����
    desc.MiscFlags = 0;
    desc.CPUAccessFlags     = 0;
    desc.SampleDesc.Count   = 1;
    desc.SampleDesc.Quality = 0;
    // Check(m_device->CreateTexture2D(&desc, NULL, m_floatBuffer.GetAddressOf()));
    // Check(m_device->CreateRenderTargetView(m_floatBuffer.Get(), NULL, m_floatRTV.GetAddressOf()));
    //  float buffer�� resolve �Ͽ� ���� �� SRV�� RTV
    desc.SampleDesc.Count   = 1;
    desc.SampleDesc.Quality = 0;
    desc.BindFlags          = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
    // Check(m_device->CreateTexture2D(&desc, NULL, m_resolvedBuffer.GetAddressOf()));
    // Check(m_device->CreateTexture2D(&desc, NULL, m_postEffectsBuffer.GetAddressOf()));
    // Check(m_device->CreateShaderResourceView(m_resolvedBuffer.Get(), NULL, m_resolvedSRV.GetAddressOf()));
    // Check(m_device->CreateShaderResourceView(m_postEffectsBuffer.Get(), NULL, m_postEffectsSRV.GetAddressOf()));
    // Check(m_device->CreateRenderTargetView(m_resolvedBuffer.Get(), NULL, m_resolvedRTV.GetAddressOf()));
    // Check(m_device->CreateRenderTargetView(m_postEffectsBuffer.Get(), NULL, m_postEffectsRTV.GetAddressOf()));
  }
  {
  }
  // InitRT(m_mainRTV, m_mainSRV, m_renderTexture, tdOption);
  InitVP(m_mainVP);
  InitRS(m_mainRS, rsOption);
  InitDB(m_depthTexture, m_mainDSV, dsOption);
  InitDSS(dssOption, m_mainDepthState);
  InitSampler(m_mainSamplerState, samplerOption);
  InitImGui();
}

void Dx11EngineManager::Run()
{
  prevFunc();
  m_context->RSSetViewports(1, &m_mainVP);
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
      ImGui_ImplDX11_NewFrame();
      ImGui_ImplWin32_NewFrame();
      ImGui::NewFrame();
      ImGui::Begin("Scene Control");
      ImGui::Text("Average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
      imguiFunc();
      ImGui::End();
      ImGui::Render();
      InitVP(m_mainVP);
      // IA: Input-Assembler stage
      // VS: Vertex Shader
      // PS: Pixel Shader
      // RS: Rasterizer stage
      // OM: Output-Merger stage
      renderFunc();
      postProcessingFunc();
      ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
      m_swapChain->Present(1, 0);
    }
  }
}

void Dx11EngineManager::InitWindow()
{
  std::wstring wName;
  wName.assign(m_screenOption.title.begin(), m_screenOption.title.end());

  WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
                    wName.c_str(),      NULL };

  if (!RegisterClassEx(&wc))
  {
    throw std::exception("RegisterClassEx() failed");
  }

  RECT wr = { 0, 0, m_screenOption.width, m_screenOption.height };
  AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, false);
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
void Dx11EngineManager::InitDeviceAndSwapChain(DXGI_SWAP_CHAIN_DESC desc)
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
  D3D_FEATURE_LEVEL featureLevel;

  // 1. Device ����, Driver ���� �׽�Ʈ
  Dx11EngineManager::Check(D3D11CreateDevice(
      // �⺻ ��� ��� �� nullptr �Է�
      nullptr,
      // ����̹� Ÿ�� ����
      driverType,
      // ����Ʈ���� �׶��̹��� �ƴ� �̻� ������ 0����
      // ���� �� ��
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
      &context));

  // ����: Immediate vs deferred context
  // A deferred context is primarily used for multithreading and is not necessary for a
  // single-threaded application.
  // https://learn.microsoft.com/en-us/windows/win32/direct3d11/overviews-direct3d-11-devices-intro#deferred-context
  // 2. 4X MSAA �����ϴ��� Ȯ��
  UINT numQualityLevels;
  Check(device->CheckMultisampleQualityLevels(desc.BufferDesc.Format, 4, &numQualityLevels));
  // device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &numQualityLevels);
  //  Check(device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8G8A8_UNORM, 4, &numQualityLevels));
  if (numQualityLevels <= 0)
  {
    // MSAA not supported.
  }

  // 3. Swapchain ����
  if (numQualityLevels > 0)
  {
    desc.SampleDesc.Count   = 4;
    desc.SampleDesc.Quality = numQualityLevels - 1;
  }
  else
  {
    desc.SampleDesc.Count   = 1;
    desc.SampleDesc.Quality = 0;
  }
  scOption.BufferDesc.Width  = m_screenOption.width;  //> ����� ũ�� ����
  scOption.BufferDesc.Height = m_screenOption.height;
  desc.SampleDesc.Count      = 1;
  desc.SampleDesc.Quality    = 0;
  desc.OutputWindow          = m_mainWindow;
  Check(device.As(&m_device));
  Check(context.As(&m_context));
  Check(D3D11CreateDeviceAndSwapChain(0,  // Default adapter
                                      driverType,
                                      0,  // No software device
                                      createDeviceFlags, featureLevels, 1, D3D11_SDK_VERSION, &desc, &m_swapChain,
                                      &m_device, &featureLevel, &m_context));
  // Check(D3D11CreateDeviceAndSwapChain(0, driverType, 0, createDeviceFlags, featureLevels, 1, D3D11_SDK_VERSION, &desc,
  //                                     m_swapChain.GetAddressOf(), m_device.GetAddressOf(), &featureLevel,
  //                                     m_context.GetAddressOf()));
}
void Dx11EngineManager::InitVP(D3D11_VIEWPORT& viewPort)
{
  ZeroMemory(&viewPort, sizeof(D3D11_VIEWPORT));
  viewPort.TopLeftX = 0;
  viewPort.TopLeftY = 0;
  viewPort.Width    = float(m_screenOption.width);
  viewPort.Height   = float(m_screenOption.height);
  // 최소 뎁스
  viewPort.MinDepth = 0.0f;
  // 최대 뎁스
  viewPort.MaxDepth = 1.0f;
  m_context->RSSetViewports(1, &viewPort);
}
void Dx11EngineManager::InitRS(Microsoft::WRL::ComPtr<ID3D11RasterizerState>& rs, D3D11_RASTERIZER_DESC desc)
{
  Check(m_device->CreateRasterizerState(&desc, rs.GetAddressOf()));
}
void Dx11EngineManager::InitSampler(Microsoft::WRL::ComPtr<ID3D11SamplerState>& sampler, D3D11_SAMPLER_DESC desc)
{
  Check(m_device->CreateSamplerState(&desc, sampler.GetAddressOf()));
}
void Dx11EngineManager::InitDSS(D3D11_DEPTH_STENCIL_DESC desc, Microsoft::WRL::ComPtr<ID3D11DepthStencilState> ptr)
{
  Check(m_device->CreateDepthStencilState(&desc, ptr.GetAddressOf()));
}
void Dx11EngineManager::ClearDSV(int flag, float depth, float stencil)
{
  m_context->ClearDepthStencilView(m_mainDSV.Get(), flag, depth, stencil);
}
void Dx11EngineManager::InitDB(Microsoft::WRL::ComPtr<ID3D11Texture2D>&        depthBuffer,
                               Microsoft::WRL::ComPtr<ID3D11DepthStencilView>& depthView, D3D11_TEXTURE2D_DESC ds)
{
  // 4X MSAA 지원하는지 확인
  UINT numQualityLevels;
  Check(m_device->CheckMultisampleQualityLevels(DXGI_FORMAT_R16G16B16A16_UNORM, 4, &numQualityLevels));
  if (numQualityLevels <= 0)
  {
    // TODO: MSAA not supported
  }
  if (numQualityLevels > 0)
  {
    ds.SampleDesc.Count   = 4;
    ds.SampleDesc.Quality = numQualityLevels - 1;
  }
  else
  {
    ds.SampleDesc.Count   = 1;
    ds.SampleDesc.Quality = 0;
  }
  ds.Width  = m_screenOption.width;  //> 백버퍼 크기 설정
  ds.Height = m_screenOption.height;

  Check(m_device->CreateTexture2D(&ds, 0, depthBuffer.GetAddressOf()));
  Check(m_device->CreateDepthStencilView(depthBuffer.Get(), 0, &depthView));

  /* D3D11_DEPTH_STENCIL_DESC 옵션 정리
   * https://learn.microsoft.com/en-us/windows/win32/api/d3d11/ns-d3d11-d3d11_depth_stencil_desc
   * StencilRead/WriteMask: 예) uint8 중 어떤 비트를 사용할지
   */

  /* D3D11_DEPTH_STENCILOP_DESC 옵션 정리
   * https://learn.microsoft.com/en-us/windows/win32/api/d3d11/ns-d3d11-d3d11_depth_stencilop_desc
   * StencilPassOp : 둘 다 pass일 때 할 일
   * StencilDepthFailOp : Stencil pass, Depth fail 일 때 할 일
   * StencilFailOp : 둘 다 fail 일 때 할 일
   */
}
void Dx11EngineManager::InitImGui()
{
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  io.DisplaySize = ImVec2(float(m_screenOption.width), float(m_screenOption.height));
  ImGui::StyleColorsLight();

  // Setup Platform/Renderer backends
  if (!ImGui_ImplDX11_Init(m_device.Get(), m_context.Get()))
  {
    return;
  }

  if (!ImGui_ImplWin32_Init(m_mainWindow))
  {
    return;
  }
}