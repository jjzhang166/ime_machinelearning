#include <cstdio>

#include "client/client.h"

int main()
{
  if (!gfx::initialize())
    return 1;

  if (!gfx::createWindow(640, 480, "Client Example"))
    return 2;

  gfx::World world;
  world.setGravity(0.f, 10.f);


  gfx::Body body {world, 100, 100};

  {
    b2PolygonShape poly;
    poly.SetAsBox(50.f, 50.f);
    body.addShape(&poly);

    b2CircleShape circle;
    circle.m_p.Set(200.0f, 0.0f);
    circle.m_radius = 100.0f;
    body.addFixture(&circle, 1.0f, 0.3f, 0.3f);
  }

  gfx::Body ground {world, 100, 300};
  ground.setStatic(true);
  {
    b2PolygonShape poly;
    poly.SetAsBox(100.f, 50.f);
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
