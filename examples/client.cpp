#include <cstdio>

#include "client/client.h"

int main()
{
  if (!gfx::initialize())
    return 1;

  if (!gfx::createWindow(640, 480, "Client Example"))
    return 2;
  gfx::setCameraPosition(0, 5);
  gfx::setCameraHeight(10);

  gfx::World world;
  world.setGravity(0.f, -10.f);

  gfx::Body body {world, 0, 8};
  {
    b2CircleShape circle;
    circle.m_p.Set(0.f, 0.f);
    circle.m_radius = 0.2f;
    body.addFixture(&circle, 1.0f, 0.3f, 0.85f);
  }

  gfx::Body ground {world, 0, 0.5, 10};
  ground.setStatic(true);
  {
    b2PolygonShape poly;
    poly.SetAsBox(1.f, 0.5f);
    ground.addShape(&poly);
  }

  gfx::setTime(0.0);
  while (!gfx::windowShouldClose())
  {
    if (gfx::getTime() >= world.getTimeStep())
    {
      world.step();
      gfx::setTime(0.0);
    }

    world.render();

    gfx::pollEvents();
    gfx::swapBuffers();
  }

  gfx::terminate();
}
