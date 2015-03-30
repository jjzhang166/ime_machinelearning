#ifndef CLIENT_WORLD_H
#define CLIENT_WORLD_H

#include <memory>
#include <vector>

#include <Box2D/Box2D.h>

namespace client
{

class Body;

class World
{
public:
  World();

  void setGravity(float accel_x, float accel_y);
  b2Vec2 getGravity() const;

  void step();

  void render();

  //
  float getTimeStep() const { return time_step_; }

protected:
  void registerBody(Body* body);
  void unregisterBody(Body* body);

private:
  friend class Body;

  b2World world_;
  std::vector<Body*> bodies_; // references

  float time_step_;
  int velocity_iterations_;
  int position_iterations_;
};

}

#endif //CLIENT_WORLD_H
