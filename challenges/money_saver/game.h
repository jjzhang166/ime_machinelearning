#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>

#include "agent.h"

enum class Winner { DRAW, SAVERS, THIEVES };

class Game
{
public:
  Game();
  ~Game();

  void setVisionSize(int vision_size);

  bool loadTerrain(const char* filename);
  bool loadTerrainFromFile(FILE* file);

  bool loadPlayers(std::string saverfile,
                   std::string thieffile);

  void step();

  int turn() const { return turn_; }

  int coinsLeft() const { return coins_available_; }
  int bank() const { return bank_; }
  int saversTotalCoins() const;
  int thievesTotalCoins() const;
  std::vector<int> saversCoins() const;
  std::vector<int> thievesCoins() const;
  Winner winning() const;

  const std::vector<Terrain> terrain() const;

  int n() const { return n_; }
  int m() const { return m_; }

  unsigned saversCount() const { return savers_.size(); }
  unsigned thievesCount() const { return thieves_.size(); }

private:
  void getVision(const Agent* p);
  void move(Agent* p, Direction dir, bool is_saver);
  void moveAgent(Agent* p, Direction dir, bool is_saver);
  Agent* getAgentByPosition(int x, int y);

  int mapXY(int x, int y) const { return y * m_ + x; }

  int turn_;
  int bank_;
  int coins_available_;

  int n_;
  int m_;
  int vision_size_;
  std::vector<Terrain> agent_map_;
  std::vector<Terrain> terrain_;
  std::vector<Terrain> vision_;

  void* saver_handle_;
  void* thief_handle_;
  std::vector<int> savers_pos_, thieves_pos_;
  std::vector<Agent*> savers_, thieves_;
};

#endif //GAME_H
