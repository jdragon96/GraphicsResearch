#include "libEngine/opengl/glRenderer.h"
#include <iostream>

namespace libEngine
{
glRenderer::glRenderer()
{
}
glRenderer::~glRenderer()
{
  glfwDestroyWindow(m_window);
  glfwTerminate();
}
void glRenderer::Initialize()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  //  ��Ƽ������� �ɼ�
  //  https://stackoverflow.com/questions/12329082/glcreateshader-is-crashing
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
  glfwWindowHint(GLFW_SAMPLES, 4);

  m_window = glfwCreateWindow(m_option.width, m_option.height, m_option.title.c_str(), nullptr, nullptr);
  if (!m_window)
  {
    throw std::runtime_error("Cloud not OpenGL window...");
  }

  glfwMakeContextCurrent(m_window);
  glViewport(0, 0, m_option.width, m_option.height);

  glewExperimental = GL_TRUE;
  auto err         = glewInit();
  if (GLEW_OK != err)
  {
    throw std::exception("Can't initialize glew instance");
    exit(1);
  }
  glfwSetWindowUserPointer(m_window, this);

  //>> ���콺 Ŭ�� �̺�Ʈ
  glfwSetMouseButtonCallback(m_window, [](GLFWwindow* w, int button, int action, int mods) {
    auto win = (glRenderer*)glfwGetWindowUserPointer(w);
    if (action == GLFW_RELEASE)
    {
      win->UpdateMousePressStatus(false);
    }
    else
    {
      win->UpdateMousePressStatus(true, button);
    }
  });
  //>> ���콺 Ŀ�� ��ġ �ݹ��Լ�
  glfwSetCursorPosCallback(m_window, [](GLFWwindow* w, double x, double y) {
    auto win = (glRenderer*)glfwGetWindowUserPointer(w);
    if (!win->IsMousePressed())
    {
      win->prevCursorX = x;
      win->prevCursorY = y;
      return;
    }
    if (win->m_camPtr == nullptr)
      return;

    switch (win->GetButtonType())
    {
      case GLFW_MOUSE_BUTTON_LEFT:
        win->m_camPtr->UpdateOrbit(x - win->prevCursorX, y - win->prevCursorY);
        break;
      case GLFW_MOUSE_BUTTON_RIGHT:
        win->m_camPtr->UpdateTranslate(x - win->prevCursorX, y - win->prevCursorY);
        break;
    }
    win->prevCursorX = x;
    win->prevCursorY = y;
  });

  //>> �����ӹ��� ������ ���� �ݹ��Լ�
  glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* w, int width, int height) {
    auto win = (glRenderer*)glfwGetWindowUserPointer(w);
    if (win->m_camPtr == nullptr)
      return;
    win->m_camPtr->SetRatio((float)width / (float)height);
    glViewport(0, 0, width, height);
  });

  //>> ���콺 ��ũ�� �̺�Ʈ
  glfwSetScrollCallback(m_window, [](GLFWwindow* w, double xoffset, double yoffset) {
    auto win = (glRenderer*)glfwGetWindowUserPointer(w);
    if (win->m_camPtr == nullptr)
      return;
    std::cout << yoffset << std::endl;
    if (yoffset > 0)
      win->m_camPtr->UpdateZoom(true);
    else
      win->m_camPtr->UpdateZoom(false);
  });

  //>> Ű���� Ŭ�� �̺�Ʈ
  glfwSetKeyCallback(m_window, [](GLFWwindow* w, int key, int scancode, int action, int mods) {
    //
    //
  });

  glfwSetErrorCallback([](int error, const char* description) { std::cout << description << std::endl; });
}
void glRenderer::Run()
{
  glEnable(GL_DEPTH_TEST);
  // Face culling �ɼ� Ȱ��ȭ
  glEnable(GL_CULL_FACE);
  // Back triangle�� ��� ������ ���� ����
  glCullFace(GL_BACK);
  // triangle�� CCW �������� �׸�
  glFrontFace(GL_CCW);
  // ���� �ؽ��� Ȱ��ȭ
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
    renderFunc();
    glfwSwapBuffers(m_window);
  }
}
int& glRenderer::GetButtonType()
{
  return m_buttonType;
}
}  // namespace libEngine