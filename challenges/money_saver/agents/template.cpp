#include <chrono>
#include <random>

#include "challenges/money_saver/agent.h"

class Template : public Agent
{
public:
  Template(const int vision_size) : Agent {vision_size},
    randDir {static_cast<int>(Direction::NONE),
             static_cast<int>(Direction::NUM_DIRECTIONS)}
  {
    gen.seed(std::chrono::system_clock::now().time_since_epoch().count());
  }

  virtual Direction walk(Terrain vision[]) override
  {
    return static_cast<Direction>(randDir(gen));
  }
private:
  std::mt19937 gen;
  std::uniform_int_distribution<int> randDir;
};

extern "C" Agent* maker(const int vision_size)
{
  return new Template {vision_size};
}
