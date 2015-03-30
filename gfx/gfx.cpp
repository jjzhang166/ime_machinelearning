#include "gfx/gfx.h"

namespace gfx
{

GLFWwindow* window;
int window_w,
    window_h;

bool initialize()
{
  if (!glfwInit())
    return false;
  return true;
}

void terminate()
{
  glfwTerminate();
}

void setErrorCallback(void (*error_callback)(int error, const char* description))
{
  glfwSetErrorCallback(error_callback);
}

void setKeyCallback(void (*key_callback)(GLFWwindow* window, int key, int scancode, int action, int mods))
{
  glfwSetKeyCallback(window, key_callback);
}

bool createWindow(int w, int h, const char* title)
{
  // TODO(naum): Change this
  glfwWindowHint(GLFW_SAMPLES, 2);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  window_w = w;
  window_h = h;
  window = glfwCreateWindow(window_w, window_h, title, 0, 0);
  if (!window)
  {
    terminate();
    return false;
  }

  glfwMakeContextCurrent(window);

  // Setup OpenGL
  // TODO(naum): Use modern pipeline?
  glMatrixMode(GL_PROJECTION);
  glViewport(0, 0, window_w, window_h);
  glOrtho(0, window_w, window_h, 0, 1.f, -1.f);
  glMatrixMode(GL_MODELVIEW);
}

bool windowShouldClose()
{
  return glfwWindowShouldClose(window);
}

void pollEvents()
{
  glfwPollEvents();
}

void swapBuffers()
{
  glfwSwapBuffers(window);
}

void setCameraPosition(int x, int y)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(x, window_w + x, window_h + y, y, 1.f, -1.f);
  glMatrixMode(GL_MODELVIEW);
}

void setTime(double time)
{
  glfwSetTime(time);
}

double getTime()
{
  return glfwGetTime();
}

}
