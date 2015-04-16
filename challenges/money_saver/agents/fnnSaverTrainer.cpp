#include <stack>
#include <cmath>

#include "geneticalgorithm/valuegenome.h"
#include "geneticalgorithm/geneticalgorithm.h"

#include "challenges/money_saver/game.h"

#include "fnnSaver.h"

class FnnSaverGenome : public ga::ValueGenome<float, false, false>
{
public:
  FnnSaverGenome() {}
  FnnSaverGenome(unsigned chromo_len) : ValueGenome(chromo_len, -1.0f, 1.0f, 0.1f)
  {}
};

void printGenome(const FnnSaverGenome& genome)
{
  auto chromo = genome.extract();
  for (unsigned j = 0; j < chromo.size(); ++j)
    printf("%+.2f ", chromo[j]);
  printf("\n");
}

int main(int argc, char** argv)
{
  if (argc != 3)
  {
    printf("Usage: %s <saver.so> <thief.so>\n", argv[0]);
    return 1;
  }

  int population_size = 20, generations = 100;
  unsigned elite = 2;
  double mutation = 0.01, crossover = 0.7;

  int max_turns = 200;
  Game trainer;
  trainer.loadTerrain("terrain.map");
  trainer.loadPlayers(argv[1], argv[2]);
  unsigned ws = dynamic_cast<FnnSaver*>(trainer.getSavers()[0])->
    brain.getWeightsCount();

  auto fitnessFunc = [&] (FnnSaverGenome& genome)
  {
    trainer.loadTerrain("terrain.map");

    auto savers = trainer.getSavers();
    for (unsigned i = 0; i < savers.size(); ++i)
      dynamic_cast<FnnSaver*>(savers[i])->brain.setWeights(genome.extract());

    // Simulation
    for (int i = 0; i < max_turns; ++i)
      trainer.step();

    return static_cast<double>(trainer.saversTotalCoins() -
                               trainer.thievesTotalCoins());
  };

  ga::GeneticAlgorithm<FnnSaverGenome> evolution (population_size, ws, elite,
                                                  mutation, crossover,
                                                  fitnessFunc);

  for (int i = 0; i < generations; ++i)
  {
    if (i > 0) evolution.epoch();

    printf("generation %d:\n", evolution.getGeneration());

    auto population = evolution.getPopulation();
    for (unsigned j = 0; j < population.size(); ++j)
      printf("%f\n", population[j].getFitness());
    printf("\n");
  }

  return 0;
}
