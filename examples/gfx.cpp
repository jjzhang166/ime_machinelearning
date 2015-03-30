#include "client/gfx.h"

int main()
{
  if(!client::initializeGfx())
    return 1;

  if (!client::createWindow(640, 480, "client Example"))
    return 2;

  client::setTime(0.0);

  while (!client::windowShouldClose())
  {
    glClear(GL_COLOR_BUFFER_BIT);

    client::setCameraPosition(10 * client::getTime(), 0);

    glLoadIdentity();
    glBegin(GL_TRIANGLES);
    glColor3f(1.f, 0.f, 0.f); glVertex3f(200.f, 100.f, 0.f);
    glColor3f(0.f, 1.f, 0.f); glVertex3f(100.f, 273.f, 0.f);
    glColor3f(0.f, 0.f, 1.f); glVertex3f(300.f, 273.f, 0.f);
    glEnd();

    client::pollEvents();
    client::swapBuffers();
  }

  client::terminateGfx();

  return 0;
}
