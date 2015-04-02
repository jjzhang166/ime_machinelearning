#include <cstdio>

#include "client/gfx.h"

namespace gfx
{

GLFWwindow* window;
float window_w,
      window_h;
float camera_x,
      camera_y,
      camera_w,
      camera_h;

bool initialize()
{
  if (!glfwInit())
  {
    fprintf(stderr, "Could not initialize GLFW!\n");
    return false;
  }
  return true;
}

void terminate()
{
  glfwTerminate();
}

// TODO(naum): Do this automatically
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
    fprintf(stderr, "Could not initialize GLFW Window!\n");
    terminate();
    return false;
  }

  glfwMakeContextCurrent(window);

  // Setup OpenGL
  camera_w = window_w;
  camera_h = window_h;
  setCameraPosition(w / 2.f, h / 2.f);

  return true;
}

bool windowShouldClose()
{
  return glfwWindowShouldClose(window);
}

float getWindowWidth()
{
  return window_w;
}

float getWindowHeight()
{
  return window_h;
}

void pollEvents()
{
  glfwPollEvents();
}

void swapBuffers()
{
  glfwSwapBuffers(window);
  glClear(GL_COLOR_BUFFER_BIT);
}

void setCameraPosition(float x, float y)
{
  camera_x = x;
  camera_y = y;
  updateCamera();
}

void setCameraWidth(float w)
{
  camera_w = w;
  camera_h = window_h * w / window_w;
  updateCamera();
}

void setCameraHeight(float h)
{
  camera_h = h;
  camera_w = window_w * h / window_h;
  updateCamera();
}

void moveCamera(float dx, float dy)
{
  camera_x += dx;
  camera_y += dy;
  updateCamera();
}

void updateCamera()
{
  // XXX(naum): Use modern pipeline?
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(camera_x - camera_w / 2.f, camera_x + camera_w / 2.f,
          camera_y - camera_h / 2.f, camera_y + camera_h / 2.f,
          1.f, -1.f);
  glMatrixMode(GL_MODELVIEW);
}

float getCameraWidth()
{
  return camera_w;
}

float getCameraHeight()
{
  return camera_h;
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
