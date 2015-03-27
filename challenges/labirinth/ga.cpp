#include <cstdio>
#include <cmath>

#include "geneticalgorithm/valuegenome.h"
#include "geneticalgorithm/genalg.h"

const int N = 10;
const int M = 10;

char labirinth[N][M + 1] =
{
  "$$$$$$$$$$",
  "$        $",
  "$        $",
  "$        $",
  "$        $",
  "$        $",
  "$        $",
  "$        $",
  "$        $",
  "$$$$$$$$$$"
};

int origin_x = 2, origin_y = 2;
int goal_x = 7, goal_y = 7;

class TeseuGenome : public ga::ValueGenome<int>
{
public:
  TeseuGenome() {}
  TeseuGenome(unsigned chromo_len) : ValueGenome(chromo_len, 0, 4, 4)
  {}
};

void printGenome(const TeseuGenome& genome)
{
  auto chromo = genome.extract();
  for (unsigned j = 0; j < chromo.size(); ++j)
    printf("%d", chromo[j]);
  printf("\n");
}

double fitnessFunc(const TeseuGenome& genome)
{
  int x = origin_x, y = origin_y;

  auto chromo = genome.extract();
  for (unsigned i = 0; i < chromo.size(); ++i)
  {
    int dir = chromo[i];
    if      (dir == 0 && labirinth[y - 1][x    ] != '$')
      y--;
    else if (dir == 1 && labirinth[y + 1][x    ] != '$')
      y++;
    else if (dir == 2 && labirinth[y    ][x - 1] != '$')
      x--;
    else if (dir == 3 && labirinth[y    ][x + 1] != '$')
      x++;

    if (y == goal_y && x == goal_x)
      return 1.0;
  }

  return 1.0 / (abs(goal_y - y) + abs(goal_x - x));
}

int main(int argc, char** argv)
{
  int population_size = 20, generations = 100;
  unsigned elite = 2;
  double mutation = 0.01, crossover = 0.7;
  if (argc > 1)
    population_size = atoi(argv[1]);
  if (argc > 2)
    generations     = atoi(argv[2]);
  if (argc > 3)
    elite           = atoi(argv[3]);
  if (argc > 4)
    mutation        = atof(argv[4]);
  if (argc > 5)
    crossover       = atof(argv[5]);

  ga::GeneticAlgorithm<TeseuGenome> evolution (population_size, N * M, elite,
                                               mutation, crossover,
                                               fitnessFunc);

  for (int i = 0; i < generations; ++i)
  {
    if (i > 0) evolution.epoch();

    printf("generation %d:\n", evolution.generation());

    auto population = evolution.population();
    for (unsigned j = 0; j < population.size(); ++j)
    {
      printf("%2d ", j);
      printGenome(population[j]);
    }
    printf("\n");
  }

  return 0;
}
