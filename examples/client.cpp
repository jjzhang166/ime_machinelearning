#include <cstdio>

#include "client/client.h"

int main()
{
  if (!client::initializeGfx())
    return 1;

  if (!client::createWindow(640, 480, "Client Example"))
    return 2;

  client::World world;
  world.setGravity(0.f, 10.f);


  client::Body body {world, 100, 100};

  {
    b2CircleShape circle;
    circle.m_p.Set(0.0f, 0.0f);
    circle.m_radius = 100.0f;
    body.addFixture(&circle, 1.0f, 0.3f, 0.3f);
  }

  client::Body ground {world, 100, 300};
  ground.setStatic(true);
  {
    b2PolygonShape poly;
    poly.SetAsBox(100.f, 50.f);
    ground.addShape(&poly);
  }

  client::setTime(0.0);
  while (!client::windowShouldClose())
  {
    if (client::getTime() >= world.getTimeStep())
    {
      world.step();
      client::setTime(0.0);
    }

    world.render();

    client::pollEvents();
    client::swapBuffers();
  }

  client::terminateGfx();
}
