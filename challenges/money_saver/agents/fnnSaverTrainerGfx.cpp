#include <cstdio>

#include "client/gfx.h"
#include "client/draw.h"

#include "geneticalgorithm/valuegenome.h"
#include "geneticalgorithm/geneticalgorithm.h"

#include "challenges/money_saver/game.h"

#include "fnnSaver.h"

class FnnSaverGenome : public ga::ValueGenome<float, false, false>
{
public:
  FnnSaverGenome() {}
  FnnSaverGenome(unsigned chromo_len) : ValueGenome(chromo_len, -1.0f, 1.0f, 1.0f)
  {}
};

void render(const Game& game);

int main(int argc, char** argv)
{
  if (argc != 3)
  {
    printf("Usage: %s <saver.so> <thief.so>\n", argv[0]);
    return 1;
  }

  // Game
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

  float time_step = 0.2f;
  bool playing = false;
  int max_turns = 200;
  // ----

  // Graphics
  gfx::initialize();
  gfx::createWindow(640, 640, "Money Saver");
  glClearColor(0.9f, 0.9f, 0.9f, 1.f);
  // --------

  // GA
  int population_size = 20;
  unsigned elite = 2;
  double mutation = 0.3, crossover = 0.7;

  unsigned ws = dynamic_cast<FnnSaver*>(game.getSavers()[0])->
    brain.getWeightsCount();

  ga::GeneticAlgorithm<FnnSaverGenome> evolution (population_size, ws, elite,
                                                  mutation, crossover);
  // --

  std::vector<FnnSaverGenome>& population = evolution.getPopulation();
  int currentGenome = 0;

  // Reset function
  std::function<void()> reset = [&]
  {
    game.reset();

    auto savers = game.getSavers();
    for (unsigned i = 0; i < savers.size(); ++i)
    {
      dynamic_cast<FnnSaver*>(savers[i])->brain.setWeights(
        population[currentGenome].extract()
      );
    }

    //dynamic_cast<FnnSaver*>(savers[0])->brain.print();
  };
  //
  reset();

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
      /*
      population[currentGenome].setFitness(game.getTotalCoins() + 1 +
                                           game.saversTotalCoins() -
                                           game.thievesTotalCoins());
      */
      population[currentGenome].setFitness(3 * game.saversTotalCoins() + 1);
      printf("fitness: %lf\n", population[currentGenome].getFitness());

      currentGenome++;
      if (currentGenome >= population.size())
      {
        printf("[GA] Epoch!\n");
        evolution.epoch();
        currentGenome = 0;
      }

      reset();
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

        ImGui::Text("Generation: %d", evolution.getGeneration());
        ImGui::Text("Current Genome: %d", currentGenome);

        if (ImGui::SmallButton(playing ? "Stop" : "Play"))
          playing ^= 1;
        if (ImGui::SmallButton("Print ANN"))
          dynamic_cast<FnnSaver*>(game.getSavers()[0])->brain.print();

        ImGui::InputFloat("Time step", &time_step, 0.010f, 0.100f, 4);
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

        //for (unsigned i = 0; i < population.size(); ++i)
        //  ImGui::Text("%2zd: %.2f", i, population[i].getFitness());
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

    // TODO(naum): evolution.epoch()

    // -------

    gfx::swapBuffers();
  }

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
