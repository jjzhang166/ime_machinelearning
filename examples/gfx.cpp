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
    gfx::setCameraPosition(320 + 10 * gfx::getTime(), 240);

    glPushMatrix();
    glTranslatef(320.f, 240.f, 0.f);
    glRotatef(100 * gfx::getTime(), 0.f, 0.f, 1.f);
    glBegin(GL_TRIANGLES);
    glColor3f(1.f, 0.f, 0.f); glVertex3f(  0.f, 107.f, 0.f);
    glColor3f(0.f, 1.f, 0.f); glVertex3f(-92.f, -53.f, 0.f);
    glColor3f(0.f, 0.f, 1.f); glVertex3f( 92.f, -53.f, 0.f);
    glEnd();
    glPopMatrix();

    gfx::pollEvents();
    gfx::swapBuffers();
  }

  gfx::terminate();

  return 0;
}
