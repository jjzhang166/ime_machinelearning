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

  gfx::initialize();
  gfx::createWindow(640, 640, "Money Saver");
  glClearColor(0.9f, 0.9f, 0.9f, 1.f);

  int max_turns = 200;

  float time_step = 0.2f;
  bool playing = false;

  bool show_options = true;

  double timer = gfx::getTime();
  while (!gfx::windowShouldClose())
  {
    // Game update
    if (game.turn() < max_turns)
    {
      if (playing)
      {
        while (gfx::getTime() - timer >= time_step)
        {
          game.step();
          timer += time_step;
        }
      }
      else
        timer = gfx::getTime();
    }
    else
    {
      playing = false;
    }

    // Poll events
    gfx::pollEvents();

    // Rendering
    render(game);

    // GUI
    // Game Options
    {
      if (ImGui::Begin("Game Options", nullptr, {0,0}, 0.8f,
                       ImGuiWindowFlags_NoResize |
                       ImGuiWindowFlags_NoMove))
      {
        ImGui::SetWindowPos({0, 0}, ImGuiSetCond_FirstUseEver);
        //ImGui::SetWindowSize({0, 0}, ImGuiSetCond_FirstUseEver);

        if (ImGui::SmallButton(playing ? "Stop" : "Play"))
        {
          playing ^= 1;
          //if (playing)
          //  ImGui::SetWindowCollapsed(true);
        }
        //ImGui::SliderFloat("Time step", &time_step, 0.f, 1.f);
        ImGui::InputFloat("Time step", &time_step, 0.010f, 0.100f, 3);
        ImGui::InputInt("Max turns", &max_turns);

        ImGui::Text("Turn: %3d", game.turn());
        ImGui::TextColored({0.f, 1.f, 0.f, 1.f},
                           "Bank coins   : %3zd", game.bank());
        ImGui::TextColored({0.f, 0.f, 1.f, 1.f},
                           "Savers coins : %3zd", game.saversTotalCoins());
        ImGui::TextColored({1.f, 0.f, 0.f, 1.f},
                           "Thieves coins: %3zd", game.thievesTotalCoins());

        ImGui::Text("Savers:");
        ImGui::Indent();
        auto coins = game.saversCoins();
        unsigned s = game.saversCount();
        for (unsigned i = 0; i < s; ++i)
          ImGui::Text("%2zd: %3d coins", i, coins[i]);
        ImGui::Unindent();

        ImGui::Text("Thieves:");
        ImGui::Indent();
        coins = game.thievesCoins();
        s = game.thievesCount();
        for (unsigned i = 0; i < s; ++i)
          ImGui::Text("%2zd: %3d coins", i, coins[i]);
        ImGui::Unindent();
      }
      ImGui::End();
    }

    // Winner announcement
    if (game.turn() == max_turns)
    {
      if (ImGui::Begin("WINNER", nullptr, {80, 40}, 0.8f,
                   ImGuiWindowFlags_NoMove |
                   ImGuiWindowFlags_NoResize |
                   ImGuiWindowFlags_NoCollapse))
      {
        ImGui::SetWindowPos({gfx::getWindowWidth() - ImGui::GetWindowSize().x, 0});
        auto winner = game.winning();
        ImGui::Text(winner == Winner::SAVERS  ? "SAVERS" :
                    winner == Winner::THIEVES ? "THIEVES" :
                                                "DRAW");
      }
      ImGui::End();
    }

    // -------

    gfx::swapBuffers();
  }

  printf("Winner: ");
  auto w = game.winning();
  if      (w == Winner::SAVERS)  printf("Savers\n");
  else if (w == Winner::THIEVES) printf("Thieves\n");
  else                           printf("Draw\n");

  return 0;
}

void render(const Game& game)
{
  float w = gfx::getWindowWidth() / game.m(),
        h = gfx::getWindowHeight() / game.n();
  float offsetx = 0.f,
        offsety = 0.f;

  if (w > h)
    w = h;
  else
    h = w;

  offsetx = gfx::getWindowWidth() - game.m() * w; offsetx /= 2.f;
  offsety = gfx::getWindowWidth() - game.n() * h; offsety /= 2.f; offsety *= -1;

  auto terrain = game.terrain();
  for (int i = 0; i < game.n(); ++i)
  {
    for (int j = 0; j < game.m(); ++j)
    {
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
      glTranslatef(offsetx + w * j + w / 2,
                   offsety + gfx::getWindowHeight() - h * i - h / 2,
                   0.f);
      gfx::drawRect(w * 0.9f, h * 0.9f);
      glPopMatrix();
    }
  }
}
