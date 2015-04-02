#include "person.h"

class Test : public Person
{
public:
  Test(const int& n, const int& m, const int& vision_size) :
    Person(n, m, vision_size)
  {}

  virtual Direction walk(Terrain vision[])
  {
    return Direction::DOWN;
  }
};

extern "C" Person* maker(const int& n, const int& m, const int& vision_size)
{
  return new Test(n, m, vision_size);
}
