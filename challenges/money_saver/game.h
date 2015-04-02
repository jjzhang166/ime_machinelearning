#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>

#include "person.h"

class Person;

class Game
{
public:
  Game(const int n, const int m, const int vision_size);
  ~Game();

  bool loadTerrain(const char* filename);
  bool loadTerrainFromFile(FILE* file);

  bool loadPlayers(std::string saverfile,
                   std::string thieffile);

  void step();

  int turn() const { return turn_; }
  int coins_available() const { return coins_available_; }

  const std::vector<Terrain> terrain() const { return terrain_; }

private:
  void getVision(const Person* p);
  void move(Person* p, Direction dir, bool is_saver);
  void movePerson(Person* p, Direction dir, bool is_saver);
  Person* getPersonByPosition(int x, int y);

  int turn_;
  int bank_;
  int coins_available_;

  const int n_;
  const int m_;
  const int vision_size_;
  std::vector<Terrain> terrain_;
  std::vector<Terrain> vision_;

  void* saver_handle_;
  void* thief_handle_;
  std::vector<int> savers_pos_, thieves_pos_;
  std::vector<Person*> savers_, thieves_;
};

#endif //GAME_H
