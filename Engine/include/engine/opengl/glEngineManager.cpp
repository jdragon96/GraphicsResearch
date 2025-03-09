#include "engine/opengl/glEngineManager.h"
#include <iostream>

glEngineManager::glEngineManager() : EngineBase()
{
}
glEngineManager::~glEngineManager()
{
  glfwDestroyWindow(m_window);
  glfwTerminate();
}
void glEngineManager::Initialize()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  //  안티엘리어싱 옵션
  //  https://stackoverflow.com/questions/12329082/glcreateshader-is-crashing
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
  glfwWindowHint(GLFW_SAMPLES, 4);

  m_window =
      glfwCreateWindow(m_screenOption.width, m_screenOption.height, m_screenOption.title.c_str(), nullptr, nullptr);
  if (!m_window)
  {
    throw std::runtime_error("Cloud not OpenGL window...");
  }

  glfwMakeContextCurrent(m_window);
  glViewport(0, 0, m_screenOption.width, m_screenOption.height);

  glewExperimental = GL_TRUE;
  auto err         = glewInit();
  if (GLEW_OK != err)
  {
    throw std::exception("Can't initialize glew instance");
    exit(1);
  }
  glfwSetWindowUserPointer(m_window, this);

  //>> 마우스 클릭 이벤트
  glfwSetMouseButtonCallback(m_window, [](GLFWwindow* w, int button, int action, int mods) {
    auto win = (glEngineManager*)glfwGetWindowUserPointer(w);
    if (action == GLFW_RELEASE)
    {
      win->m_isMousePressed = false;
    }
    else
    {
      win->m_isMousePressed = true;
      switch (button)
      {
        case 0:
          win->m_buttonType = MouseButtonType::LEFT;
          break;
        case 1:
          win->m_buttonType = MouseButtonType::RIGHT;
          break;
        case 2:
          win->m_buttonType = MouseButtonType::SCROLL;
          break;
      }
    }
  });
  //>> 마우스 커서 위치 콜백함수
  glfwSetCursorPosCallback(m_window, [](GLFWwindow* w, double x, double y) {
    auto win = (glEngineManager*)glfwGetWindowUserPointer(w);
    if (!win->m_isMousePressed)
    {
      win->m_prevPosX = x;
      win->m_prevPosY = y;
      return;
    }
    if (win->m_camPtr == nullptr)
      return;

    switch (win->m_buttonType)
    {
      case MouseButtonType::LEFT:
        win->m_camPtr->UpdateOrbit(x - win->m_prevPosX, y - win->m_prevPosY);
        break;
      case MouseButtonType::RIGHT:
        win->m_camPtr->UpdateTranslate(x - win->m_prevPosX, y - win->m_prevPosY);
        break;
    }
    win->m_prevPosX = x;
    win->m_prevPosY = y;
  });

  //>> 프레임버퍼 사이즈 변경 콜백함수
  glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* w, int width, int height) {
    auto win = (glEngineManager*)glfwGetWindowUserPointer(w);
    if (win->m_camPtr == nullptr)
      return;
    win->m_camPtr->SetRatio((float)width / (float)height);
    glViewport(0, 0, width, height);
  });

  //>> 마우스 스크롤 이벤트
  glfwSetScrollCallback(m_window, [](GLFWwindow* w, double xoffset, double yoffset) {
    auto win = (glEngineManager*)glfwGetWindowUserPointer(w);
    if (win->m_camPtr == nullptr)
      return;
    if (yoffset > 0)
      win->m_camPtr->UpdateZoom(true);
    else
      win->m_camPtr->UpdateZoom(false);
  });

  //>> 키보드 클릭 이벤트
  glfwSetKeyCallback(m_window, [](GLFWwindow* w, int key, int scancode, int action, int mods) {
    //
    //
  });

  glfwSetErrorCallback([](int error, const char* description) { std::cout << description << std::endl; });
  InitFrameBuffer();
  InitImGui();
}
void glEngineManager::Run()
{
  glEnable(GL_DEPTH_TEST);
  // Face culling 옵션 활성화
  glEnable(GL_CULL_FACE);
  // Back triangle은 모두 랜더링 하지 않음
  glCullFace(GL_BACK);
  // triangle을 CCW 방향으로 그림
  glFrontFace(GL_CCW);
  // 투명 텍스쳐 활성화
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_POINT_SMOOTH);
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_POLYGON_SMOOTH);
  prevFunc();
  while (!glfwWindowShouldClose(m_window))
  {
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwPollEvents();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    // glBindFramebuffer(GL_FRAMEBUFFER, this->m_frameBuffer);
    // glClearColor(0.f, 0.0f, 0.0f, 1.0f);
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // glEnable(GL_DEPTH_TEST);
    imguiFunc();
    renderFunc();
    // glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // glDisable(GL_DEPTH_TEST);
    // glClearColor(0, 0, 0, 0);
    //  glClear(GL_COLOR_BUFFER_BIT);
    postProcessingFunc();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(m_window);
  }
}
void glEngineManager::InitFrameBuffer()
{
  glGenFramebuffers(1, &this->m_frameBuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, this->m_frameBuffer);
  // FrameBuffer를 생성시 반드시 하나의 texture buffer를 첨부해야함
  // 1. textureBuffer
  glGenTextures(1, &this->m_textureBuffer);
  glBindTexture(GL_TEXTURE_2D, this->m_textureBuffer);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->m_screenOption.width, this->m_screenOption.height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // glBindTexture(GL_TEXTURE_2D, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->m_textureBuffer, 0);
  // 2. renderBuffer
  glGenRenderbuffers(1, &this->m_renderBuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, this->m_renderBuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, this->m_screenOption.width, this->m_screenOption.height);
  // glBindRenderbuffer(GL_RENDERBUFFER, 0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->m_renderBuffer);
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
  {
    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    throw std::exception("not created frame buffer");
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void glEngineManager::InitImGui()
{
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(m_window, true);
  ImGui_ImplOpenGL3_Init("#version 460");
}