#include <cstdio>

#include "game.h"

int main(int argc, char** argv)
{
  if (argc != 3)
  {
    printf("Usage: %s <saver.so> <thief.so>\n", argv[0]);
    return 1;
  }

  Game game;
  if (!game.loadTerrain("terrain.map"))
  {
    printf("Error loading terrain!\n");
    return 1;
  }

  if (!game.loadPlayers(argv[1], argv[2]))
  {
    printf("Error loading players!\n");
    return 1;
  }

  int max_turns = 200;
  for (int t = 0; t < max_turns; ++t)
  {
    auto terrain = game.terrain();
    printf("*");
    for (int i = 0; i < game.m(); ++i)
      printf("-");
    printf("*");
    printf("\n");

    for (int i = 0; i < game.n(); ++i)
    {
      printf("|");
      for (int j = 0; j < game.m(); ++j)
      {
        auto t = terrain[i * game.m() + j];
        if (t == Terrain::NONE)
          printf(" ");
        else if (t == Terrain::WALL)
          printf("#");
        else if (t == Terrain::COIN)
          printf("0");
        else if (t == Terrain::BANK)
          printf("B");
        else if (t == Terrain::SAVER)
          printf("S");
        else if (t == Terrain::THIEF)
          printf("T");
      }
      printf("|");
      printf("\n");
    }
    printf("*");
    for (int i = 0; i < game.m(); ++i)
      printf("-");
    printf("*");
    printf("\n\n");

    game.step();
  }

  printf("Winner: ");
  auto w = game.winning();
  if (w == Winner::SAVERS)       printf("Saviers\n");
  else if (w == Winner::THIEVES) printf("Thieves\n");
  else                           printf("Draw\n");

  return 0;
}
