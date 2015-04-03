#include <chrono>
#include <random>

#include "challenges/money_saver/agent.h"

class Template : public Agent
{
public:
  Template() : randDir {static_cast<int>(Direction::NONE),
                        static_cast<int>(Direction::NUM_DIRECTIONS)}
  {
    gen.seed(std::chrono::system_clock::now().time_since_epoch().count());
  }

  virtual Direction walk(Terrain vision[], int vision_size)
  {
    return static_cast<Direction>(randDir(gen));
  }
private:
  std::mt19937 gen;
  std::uniform_int_distribution<int> randDir;
};

extern "C" Agent* maker()
{
  return new Template;
}
