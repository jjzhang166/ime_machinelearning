#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>

#include "agent.h"

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
  void getVision(const Agent* p);
  void move(Agent* p, Direction dir, bool is_saver);
  void movePerson(Agent* p, Direction dir, bool is_saver);
  Agent* getPersonByPosition(int x, int y);

  int turn_;
  int bank_;
  int coins_available_;

  const int n_;
  const int m_;
  const int vision_size_;
  std::vector<Terrain> agent_map_;
  std::vector<Terrain> terrain_;
  std::vector<Terrain> vision_;

  void* saver_handle_;
  void* thief_handle_;
  std::vector<int> savers_pos_, thieves_pos_;
  std::vector<Agent*> savers_, thieves_;
};

#endif //GAME_H
