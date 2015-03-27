#include <stack>
#include <cmath>

#include "geneticalgorithm/valuegenome.h"
#include "geneticalgorithm/genalg.h"

class FloatValueGenome : public ga::ValueGenome<float>
{
public:
  FloatValueGenome() {}
  FloatValueGenome(unsigned chromo_len) : ValueGenome(chromo_len, -1.0f, 1.0f, 0.1f)
  {}
};

void printGenome(const FloatValueGenome& genome)
{
  auto chromo = genome.extract();
  for (unsigned j = 0; j < chromo.size(); ++j)
    printf("%+.2f ", chromo[j]);
  printf("\n");
}

int main(int argc, char** argv)
{
  int population_size = 20, generations = 10;
  unsigned elite = 2;
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

  ga::GeneticAlgorithm<FloatValueGenome> evolution (population_size, 16, elite,
                                                  mutation, crossover,
                                                  [] (const FloatValueGenome& a)
                                                  { return 1.0; });

  for (int i = 0; i < generations; ++i)
  {
    if (i > 0) evolution.epoch();

    printf("generation %d:\n", evolution.generation());

    auto population = evolution.population();
    for (unsigned j = 0; j < population.size(); ++j)
      printGenome(population[j]);
    printf("\n");
  }

  return 0;
}
