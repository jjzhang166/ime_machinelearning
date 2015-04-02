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
  if (game.loadTerrain("terrain.map"))
    printf("Terrain loaded!\n");
  else
  {
    printf("Error loading terrain!\n");
    return 1;
  }

  if (game.loadPlayers(argv[1], argv[2]))
    printf("Players loaded!\n");
  else
  {
    printf("Error loading players!\n");
    return 1;
  }

  for (int t = 0; t < 10; ++t)
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

  return 0;
}
