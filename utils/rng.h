#ifndef RNG_H
#define RNG_H

#include <random>

namespace rng
{

extern unsigned seed_;
extern std::mt19937 generator_;
extern std::uniform_real_distribution<float> real_dist_;

int randInt(int a, int b);
float randFloat();
bool randBool();

}

#endif //RNG_H
