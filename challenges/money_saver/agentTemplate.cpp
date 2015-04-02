#include "agent.h"

class Template : public Agent
{
public:
  virtual Direction walk(Terrain vision[], int vision_size)
  {
    return Direction::DOWN;
  }
};

extern "C" Agent* maker()
{
  return new Template;
}
