#include <stack>
#include <cmath>

#include "geneticalgorithm/valuegenome.h"
#include "geneticalgorithm/genalg.h"

class IntValueGenome : public ga::ValueGenome<int, true, true>
{
public:
  IntValueGenome() {}
  IntValueGenome(unsigned chromo_len) : ValueGenome(chromo_len, 0, 10)
  {}
};

void printGenome(const IntValueGenome& genome)
{
  auto chromo = genome.extract();
  for (unsigned j = 0; j < chromo.size(); ++j)
    printf("%d ", chromo[j]);
  printf("\n");
}

int main(int argc, char** argv)
{
  unsigned population_size = 20, generations = 100, elite = 2;
  double mutation = 0.01, crossover = 0.7;
  if (argc > 1)
    population_size  = atoi(argv[1]);
  if (argc > 2)
    generations = atoi(argv[2]);
  if (argc > 3)
    elite       = atoi(argv[3]);
  if (argc > 4)
    mutation    = atof(argv[4]);
  if (argc > 5)
    crossover   = atof(argv[5]);

  auto fitnessFunc = [] (IntValueGenome& g)
  {
    int t = 0;
    auto x = g.extract();
    for (unsigned i = 0; i < x.size(); ++i)
      t += x[i];
    return 1.0 / exp(abs(42 - t) / 10.f);
  };

  ga::GeneticAlgorithm<IntValueGenome> evolution {population_size, 8, elite,
                                                  mutation, crossover,
                                                  fitnessFunc};

  for (unsigned i = 0; i < generations; ++i)
  {
    if (i > 0) evolution.epoch();

    printf("generation %d:\n", evolution.generation());

    auto population = evolution.population();
    for (unsigned j = 0; j < population.size(); ++j)
    {
      printf("%.2f: ", population[j].getFitness());
      printGenome(population[j]);
    }
    printf("\n");
  }

  return 0;
}
