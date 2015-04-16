#include <dlfcn.h>
#include <cstdio>

#include <string>
#include <algorithm>

#include "game.h"


Game::
Game() :
  turn_ {0}, bank_ {0}, coins_available_ {0}, n_ {0}, m_ {0}, vision_size_ {1},
  saver_handle_ {nullptr}, thief_handle_ {nullptr}
{
  // TODO(naum): Generate random terrain
  auto v = 2 * vision_size_ + 1;
  vision_.resize(v * v);
}

Game::
~Game()
{
#ifndef NDEBUG
  printf("[GAME] Deleting...\n");
#endif

  if (saver_handle_)
    dlclose(saver_handle_);

#ifndef NDEBUG
  printf("[GAME] Saver handle deleted!\n");
#endif

  if (thief_handle_)
    dlclose(thief_handle_);

#ifndef NDEBUG
  printf("[GAME] Thief handle deleted!\n");
#endif
}

void Game::
setVisionSize(int vision_size)
{
  vision_size_ = vision_size;

  auto v = 2 * vision_size_ + 1;
  vision_.resize(v * v);
}

bool Game::
loadTerrain(const char* filename)
{
  FILE* file = std::fopen(filename, "r");
  if (!file)
  {
    fclose(file);
    return false;
  }

  bool r = loadTerrainFromFile(file);
  fclose(file);
  return r;
}

bool Game::
loadTerrainFromFile(FILE* file)
{
  fscanf(file, "%d%d", &n_, &m_);

  agent_map_.clear();
  agent_map_.resize(n_ * m_);

  terrain_.clear();
  terrain_.resize(n_ * m_);

  coins_available_ = 0;

  bool okay = true;
  int a;
  Terrain t;
  for (int i = 0; i < n_ * m_; ++i)
  {
    if (std::feof(file))
    {
      okay = false;
      break;
    }

    fscanf(file, "%1d", &a);
    t = static_cast<Terrain>(a);
    terrain_[i] = t;
    if (t == Terrain::SAVER)
    {
      terrain_[i] = Terrain::NONE;
      savers_pos_.push_back(i);
      agent_map_[i] = Terrain::SAVER;
    }
    else if (t == Terrain::THIEF)
    {
      terrain_[i] = Terrain::NONE;
      thieves_pos_.push_back(i);
      agent_map_[i] = Terrain::THIEF;
    }
    else if (t == Terrain::COIN)
      coins_available_++;
  }

  original_terrain_ = terrain_;
  original_coins_available_ = coins_available_;

  return okay;
}

bool Game::
loadPlayers(std::string saverfile,
            std::string thieffile)
{
  std::string saverstr = "./" + saverfile,
              thiefstr = "./" + thieffile;

  Agent* (*maker)(const int);

#ifndef NDEBUG
  printf("[GAME] Loading players...\n");
#endif

  // Saver
  if (saver_handle_)
    dlclose(saver_handle_);

  saver_handle_ = dlopen(saverstr.c_str(), RTLD_LAZY);
  if (!saver_handle_)
  {
    fprintf(stderr, "Could not load saver: %s\n", dlerror());
    return false;
  }

#ifndef NDEBUG
  printf("[GAME] Saver handle ok!\n");
#endif

  maker = (Agent* (*)(const int)) dlsym(saver_handle_, "maker");

  if (!maker)
  {
    fprintf(stderr, "Could not find make symbol: %s\n", dlerror());
    return false;
  }

#ifndef NDEBUG
  printf("[GAME] Saver maker ok!\n");
#endif

  savers_.clear();
  for (unsigned i = 0; i < savers_pos_.size(); ++i)
    savers_.push_back(maker(vision_size_));

#ifndef NDEBUG
  printf("[GAME] All savers created!\n");
#endif

  // Thief
  if (thief_handle_)
    dlclose(thief_handle_);

  thief_handle_ = dlopen(thiefstr.c_str(), RTLD_LAZY);
  if (!thief_handle_)
  {
    fprintf(stderr, "Could not load thief: %s\n", dlerror());
    return false;
  }

#ifndef NDEBUG
  printf("[GAME] Thief handle ok!\n");
#endif

  maker = (Agent* (*)(const int)) dlsym(thief_handle_, "maker");
  if (!maker)
  {
    fprintf(stderr, "Could not find make symbol: %s\n", dlerror());
    return false;
  }

#ifndef NDEBUG
  printf("[GAME] Thief maker ok!\n");
#endif

  thieves_.clear();
  for (unsigned i = 0; i < thieves_pos_.size(); ++i)
    thieves_.push_back(maker(vision_size_));

#ifndef NDEBUG
  printf("[GAME] All thieves created!\n");
  printf("[GAME] Loading complete!\n");
#endif

  resetAgents();

  return true;
}

void Game::
step()
{
  for (unsigned i = 0; i < savers_.size(); ++i)
  {
    getVision(savers_[i]);
    Direction dir = savers_[i]->walk(&vision_[0]);
    move(savers_[i], dir, true);
  }

  for (unsigned i = 0; i < thieves_.size(); ++i)
  {
    getVision(thieves_[i]);
    Direction dir = thieves_[i]->walk(&vision_[0]);
    move(thieves_[i], dir, false);
  }

  turn_++;
}

void Game::
reset()
{
  turn_ = 0;
  bank_ = 0;
  coins_available_ = original_coins_available_;

  terrain_ = original_terrain_;

  resetAgents();
}

const std::vector<Terrain> Game::
terrain() const
{
  std::vector<Terrain> t = terrain_;
  for (int i = 0; i < n_ * m_; ++i)
    if (agent_map_[i] != Terrain::NONE)
      t[i] = agent_map_[i];
  return t;
}

void Game::
getVision(const Agent* p)
{
  int x = p->x(),
      y = p->y();
  int s = 2 * vision_size_ + 1;

  Terrain t;
  for (int i = 0; i < 2 * vision_size_ + 1; ++i)
  {
    for (int j = 0; j < 2 * vision_size_ + 1; ++j)
    {
      int px = x + (j - vision_size_),
          py = y + (i - vision_size_);
      if (py < 0 || py >= n_ ||
          px < 0 || px >= m_)
        vision_[i * s + j] = Terrain::WALL;
      else
      {
        t = agent_map_[py * m_ + px];
        if (t == Terrain::NONE)
          vision_[i * s + j] = terrain_[py * m_ + px];
        else
          vision_[i * s + j] = t;
      }
    }
  }
}

void Game::
move(Agent* p, Direction dir, bool is_saver)
{
  int x = p->x(),
      y = p->y();
  int target_x = -1,
      target_y = -1;

  if (dir == Direction::UP && y > 0) // UP
    target_x = x, target_y = y - 1;
  else if (dir == Direction::DOWN && y < n_ - 1) // DOWN
    target_x = x, target_y = y + 1;
  else if (dir == Direction::LEFT && x > 0) // LEFT
    target_x = x - 1, target_y = y;
  else if (dir == Direction::RIGHT && x < m_ - 1) // RIGHT
    target_x = x + 1, target_y = y;

  if (target_x < 0)
    return;

  int xy = mapXY(target_x, target_y);
  Terrain& tile = terrain_[xy];

  if (tile == Terrain::NONE && agent_map_[xy] == Terrain::NONE)
    moveAgent(p, dir, is_saver);
  else if (is_saver)
  {
    if (agent_map_[xy] == Terrain::THIEF)
      return;

    if (tile == Terrain::COIN)
    {
      p->coins_++;
      coins_available_--;
      tile = Terrain::NONE;
      moveAgent(p, dir, true);
    }
    else if (tile == Terrain::BANK)
    {
      bank_ += p->coins();
      p->coins_ = 0;
    }
  }
  else if (!is_saver)
  {
    if (agent_map_[xy] == Terrain::SAVER)
    {
      auto s = getAgentByPosition(target_x, target_y);
      if (s != nullptr)
      {
        p->coins_ += s->coins();
        s->coins_ = 0;
      }
      else
      {
        fprintf(stderr, "[ERROR] Null agent on position (%d, %d)\n",
                target_x, target_y);
      }
    }
    else if (tile == Terrain::COIN)
      moveAgent(p, dir, false);
  }
}

void Game::
moveAgent(Agent* p, Direction dir, bool is_saver)
{
  int x = p->x(), y = p->y();

  if (dir == Direction::UP)        y--;
  else if (dir == Direction::DOWN) y++;
  else if (dir == Direction::LEFT) x--;
  else if (dir == Direction::RIGHT) x++;

  if (agent_map_[y * m_ + x] != Terrain::NONE)
  {
    fprintf(stderr, "[ERROR] Moving to position already ocupied (%d, %d)",
            x, y);
    return;
  }

  agent_map_[p->y() * m_ + p->x()] = Terrain::NONE;

  if (dir == Direction::UP)        p->y_ = p->y_ - 1;
  else if (dir == Direction::DOWN) p->y_ = p->y_ + 1;
  else if (dir == Direction::LEFT) p->x_ = p->x_ - 1;
  else if (dir == Direction::RIGHT) p->x_ = p->x_ + 1;

  agent_map_[p->y() * m_ + p->x()] = (is_saver ? Terrain::SAVER :
                                             Terrain::THIEF);
}

Agent* Game::
getAgentByPosition(int x, int y)
{
  for (unsigned i = 0; i < savers_.size(); ++i)
    if (savers_[i]->x() == x && savers_[i]->y() == y)
      return savers_[i];

  for (unsigned i = 0; i < thieves_.size(); ++i)
    if (thieves_[i]->x() == x && thieves_[i]->y() == y)
      return thieves_[i];

  return nullptr;
}

int Game::
saversTotalCoins() const
{
  int coins = 0; // bank?
  for (unsigned i = 0; i < savers_.size(); ++i)
    coins += savers_[i]->coins();
  return coins;
}

int Game::
thievesTotalCoins() const
{
  int coins = 0;
  for (unsigned i = 0; i < thieves_.size(); ++i)
    coins += thieves_[i]->coins();
  return coins;
}

std::vector<int> Game::
saversCoins() const
{
  std::vector<int> coins;
  for (unsigned i = 0; i < savers_.size(); ++i)
    coins.push_back(savers_[i]->coins());
  return coins;
}

std::vector<int> Game::
thievesCoins() const
{
  std::vector<int> coins;
  for (unsigned i = 0; i < thieves_.size(); ++i)
    coins.push_back(thieves_[i]->coins());
  return coins;
}

Winner Game::
winning() const
{
  int savers_coins = bank_,
      thieves_coins = 0;
  for (unsigned i = 0; i < savers_.size(); ++i)
    savers_coins += savers_[i]->coins();
  for (unsigned i = 0; i < thieves_.size(); ++i)
    thieves_coins += thieves_[i]->coins();

  if      (savers_coins > thieves_coins)
    return Winner::SAVERS;
  else if (savers_coins < thieves_coins)
    return Winner::THIEVES;
  else
    return Winner::DRAW;
}

void Game::
resetAgents()
{
#ifndef NDEBUG
  printf("[GAME] Positioning agents\n");
#endif
  agent_map_.clear();
  agent_map_.insert(agent_map_.begin(),
                    terrain_.size(),
                    Terrain::NONE);

  int pos;
  for (unsigned i = 0; i < savers_pos_.size(); ++i)
  {
    pos = savers_pos_[i];
    agent_map_[pos] = Terrain::SAVER;
    savers_[i]->x_ = pos % m_;
    savers_[i]->y_ = pos / m_;
    savers_[i]->coins_ = 0;
  }

  for (unsigned i = 0; i < thieves_pos_.size(); ++i)
  {
    pos = thieves_pos_[i];
    agent_map_[pos] = Terrain::THIEF;
    thieves_[i]->x_ = pos % m_;
    thieves_[i]->y_ = pos / m_;
    thieves_[i]->coins_ = 0;
  }


#ifndef NDEBUG
  printf("[GAME] Positioning ok!\n");
#endif
}
