#include "client/gfx.h"

int main()
{
  if(!gfx::initialize())
    return 1;

  if (!gfx::createWindow(640, 480, "Gfx Example"))
    return 2;

  gfx::setTime(0.0);

  while (!gfx::windowShouldClose())
  {
    gfx::setCameraPosition(10 * gfx::getTime(), 0);

    glLoadIdentity();
    glBegin(GL_TRIANGLES);
    glColor3f(1.f, 0.f, 0.f); glVertex3f(200.f, 100.f, 0.f);
    glColor3f(0.f, 1.f, 0.f); glVertex3f(100.f, 273.f, 0.f);
    glColor3f(0.f, 0.f, 1.f); glVertex3f(300.f, 273.f, 0.f);
    glEnd();

    gfx::pollEvents();
    gfx::swapBuffers();
  }

  gfx::terminate();

  return 0;
}
