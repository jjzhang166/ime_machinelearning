#ifndef PERSON_H
#define PERSON_H

enum class Direction
{
  NONE, UP, DOWN, LEFT, RIGHT, NUM_DIRECTIONS
};

enum class Terrain
{
  NONE, WALL, COIN, BANK, SAVER, THIEF, NUM_TERRAINS
};

class Game;

class Agent
{
public:
  Agent(const int vision) : vision_size {vision}
  {}

  virtual ~Agent() {}
  virtual Direction walk(Terrain vision[]) =0;

  int x() const { return x_; }
  int y() const { return y_; }
  int coins() const { return coins_; }

  const int vision_size;
private:
  int x_, y_;
  int coins_;

  friend class Game;
};

#endif //PERSON_H
