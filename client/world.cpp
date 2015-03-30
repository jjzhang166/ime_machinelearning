#include <cstdio>
#include <algorithm>

#include "client/world.h"
#include "client/body.h"

namespace gfx
{

World::
World() :
  world_ {b2Vec2 {0.f, 0.f}},
  time_step_ {1.f / 60.f},
  velocity_iterations_ {8},
  position_iterations_ {3}
{}

void World::
setGravity(float accel_x, float accel_y)
{
  world_.SetGravity(b2Vec2 {accel_x, accel_y});
}

b2Vec2 World::
getGravity() const
{
  return world_.GetGravity();
}

void World::
step()
{
  world_.Step(time_step_, velocity_iterations_, position_iterations_);
}

void World::
render()
{
  for (unsigned i = 0; i < bodies_.size(); ++i)
    bodies_[i]->render();
}

void World::
registerBody(Body* body)
{
  bodies_.push_back(body);
  fprintf(stderr, "Body registered. count: %zd\n", bodies_.size());
}

void World::
unregisterBody(Body* body)
{
  bodies_.erase(std::remove(std::begin(bodies_),
                            std::end(bodies_),
                            body),
                std::end(bodies_));
  fprintf(stderr, "Body registered. count: %zd\n", bodies_.size());
}

}
