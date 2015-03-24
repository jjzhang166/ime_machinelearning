#include <chrono>

#include "utils/rng.h"

namespace rng
{

unsigned seed_ = std::chrono::system_clock::now().time_since_epoch().count();
std::mt19937 generator_ {seed_};
std::uniform_real_distribution<float> real_dist_ {0.0f, 1.0f};

int randInt(int a, int b)
{
  // TODO(naum): Use uniform distribution
  return (generator_() % (b - a)) + a;
}

float randFloat()
{
  return real_dist_(generator_);
}

bool randBool()
{
  return (generator_() % 2 == 0) ? true : false;
}

}
