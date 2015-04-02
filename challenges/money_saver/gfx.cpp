#include <cstdio>

#include "client/gfx.h"
#include "client/draw.h"

#include "game.h"

void render(const Game& game);

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

  gfx::initialize();
  gfx::createWindow(500, 500, "Money Saver");
  glClearColor(0.9f, 0.9f, 0.9f, 1.f);

  gfx::setTime(0);
  double timer = gfx::getTime();
  while (!gfx::windowShouldClose())
  {
    render(game);

    while (gfx::getTime() - timer >= 0.5)
    {
      game.step();
      timer += 0.5;
    }

    gfx::pollEvents();
    gfx::swapBuffers();
  }

  return 0;
}

void render(const Game& game)
{
  float w = gfx::getWindowWidth() / game.m(),
        h = gfx::getWindowHeight() / game.n();

  auto terrain = game.terrain();
  for (int i = 0; i < game.n(); ++i)
  {
    for (int j = 0; j < game.m(); ++j)
    {
      //printf("%d", terrain[i * m + j]);
      auto t = terrain[i * game.m() + j];

      if (t == Terrain::WALL)
        glColor3f(0.f, 0.f, 0.f);
      else if (t == Terrain::BANK)
        glColor3f(0.f, 0.8f, 0.f);
      else if (t == Terrain::COIN)
        glColor3f(1.f, 1.f, 0.f);
      else if (t == Terrain::SAVER)
        glColor3f(0.f, 0.f, 1.f);
      else if (t == Terrain::THIEF)
        glColor3f(1.f, 0.f, 0.f);
      else
        glColor3f(1.f, 1.f, 1.f);

      glPushMatrix();
      glTranslatef(w * j + w / 2, gfx::getWindowHeight() - h * i - h / 2, 0.f);
      gfx::drawRect(w * 0.9f, h * 0.9f);
      glPopMatrix();
    }
  }
}
