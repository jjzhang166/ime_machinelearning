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

  agent_map_.resize(n_ * m_);
  terrain_.resize(n_ * m_);

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
  }

  return okay;
}

bool Game::
loadPlayers(std::string saverfile,
            std::string thieffile)
{
  std::string saverstr = "./" + saverfile,
              thiefstr = "./" + thieffile;

  Agent* (*maker)();

#ifndef NDEBUG
  printf("[GAME] Loading players...\n");
#endif

  // Saver
  saver_handle_ = dlopen(saverstr.c_str(), RTLD_LAZY);
  if (!saver_handle_)
  {
    fprintf(stderr, "Could not load saver: %s\n", dlerror());
    return false;
  }

#ifndef NDEBUG
  printf("[GAME] Saver handle ok!\n");
#endif

  maker = (Agent* (*)()) dlsym(saver_handle_, "maker");

  if (!maker)
  {
    fprintf(stderr, "Could not find make symbol: %s\n", dlerror());
    return false;
  }

#ifndef NDEBUG
  printf("[GAME] Saver maker ok!\n");
#endif

  for (unsigned i = 0; i < savers_pos_.size(); ++i)
  {
    savers_.push_back(maker());
    savers_[i]->x_ = savers_pos_[i] % m_;
    savers_[i]->y_ = savers_pos_[i] / m_;
  }
  savers_pos_.clear();

#ifndef NDEBUG
  printf("[GAME] All savers created!\n");
#endif

  // Thief
  thief_handle_ = dlopen(thiefstr.c_str(), RTLD_LAZY);
  if (!thief_handle_)
  {
    fprintf(stderr, "Could not load thief: %s\n", dlerror());
    return false;
  }

#ifndef NDEBUG
  printf("[GAME] Thief handle ok!\n");
#endif

  maker = (Agent* (*)()) dlsym(thief_handle_, "maker");
  if (!maker)
  {
    fprintf(stderr, "Could not find make symbol: %s\n", dlerror());
    return false;
  }

#ifndef NDEBUG
  printf("[GAME] Thief maker ok!\n");
#endif

  for (unsigned i = 0; i < thieves_pos_.size(); ++i)
  {
    thieves_.push_back(maker());
    thieves_[i]->x_ = thieves_pos_[i] % m_;
    thieves_[i]->y_ = thieves_pos_[i] / m_;
  }
  thieves_pos_.clear();

#ifndef NDEBUG
  printf("[GAME] All thieves created!\n");
  printf("[GAME] Loading complete!\n");
#endif

  return true;
}

void Game::
step()
{
  for (unsigned i = 0; i < savers_.size(); ++i)
  {
    getVision(savers_[i]);
    Direction dir = savers_[i]->walk(&vision_[0], vision_size_);
    move(savers_[i], dir, true);
  }

  for (unsigned i = 0; i < thieves_.size(); ++i)
  {
    getVision(thieves_[i]);
    Direction dir = thieves_[i]->walk(&vision_[0], vision_size_);
    move(thieves_[i], dir, false);
  }

  turn_++;
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

  Terrain tile = Terrain::INVALID,
          agent_tile = Terrain::INVALID;
  if (dir == Direction::UP && y > 0) // UP
  {
    tile = terrain_[(y-1) * m_ + x];
    agent_tile = agent_map_[(y-1) * m_ + x];
  }
  else if (dir == Direction::DOWN && y < n_ - 1) // DOWN
  {
    tile = terrain_[(y+1) * m_ + x];
    agent_tile = agent_map_[(y+1) * m_ + x];
  }
  else if (dir == Direction::LEFT && x > 0) // LEFT
  {
    tile = terrain_[y * m_ + x-1];
    agent_tile = agent_map_[y * m_ + x-1];
  }
  else if (dir == Direction::RIGHT) // RIGHT
  {
    tile = terrain_[y * m_ + x+1];
    agent_tile = agent_map_[y * m_ + x+1];
  }


  if (tile == Terrain::NONE && agent_tile == Terrain::NONE)
    moveAgent(p, dir, is_saver);
  else if (is_saver)
  {
    if (agent_tile == Terrain::THIEF)
      return;

    if (tile == Terrain::COIN)
    {
      p->coins_++;
      terrain_[y * m_ + x] = Terrain::NONE;
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
    if (agent_tile == Terrain::SAVER)
    {
      auto s = getAgentByPosition(x, y);
      p->coins_ += s->coins();
      s->coins_ = 0;
    }

    if (tile == Terrain::COIN)
      moveAgent(p, dir, false);
  }
}

void Game::
moveAgent(Agent* p, Direction dir, bool is_saver)
{
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
