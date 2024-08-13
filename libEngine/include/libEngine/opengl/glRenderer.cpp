#include "libEngine/opengl/glRenderer.h"

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
  // glfwSetWindowUserPointer(m_window, &WindowsManager::instance());
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
}  // namespace libEngine