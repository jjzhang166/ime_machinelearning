#include <cstdio>
#include <cmath>

#include "geneticalgorithm/valuegenome.h"
#include "geneticalgorithm/genalg.h"

const int N = 10;
const int M = 10;

char labirinth[N][M + 1] =
{
  "$$$$$$$$$$",
  "$    $   $",
  "$  $   $ $",
  "$$$$$$$  $",
  "$        $",
  "$ $$$$$$ $",
  "$    $ $ $",
  "$ $ $$ $ $",
  "$ $    $ $",
  "$$$$$$$$$$"
};

int origin_x = 2, origin_y = 2;
int goal_x = 6, goal_y = 7;

class TeseuGenome : public ga::ValueGenome<int, true, true>
{
public:
  TeseuGenome() {}
  TeseuGenome(unsigned chromo_len) : ValueGenome(chromo_len, 0, 4)
  {}

  int x, y;
  int steps;
};

void printGenome(const TeseuGenome& genome)
{
  printf("fitness: %f\nsteps: %d\n", genome.getFitness(), genome.steps);
  auto chromo = genome.extract();
  for (unsigned j = 0; j < chromo.size(); ++j)
    printf("%c", (chromo[j] == 0) ? 'U' : ((chromo[j] == 1) ? 'D' : ((chromo[j] == 2) ? 'L' : 'R')));
  printf("\n");

  for (int i = 0; i < N; i++)
  {
    for (int j = 0; j < M; ++j)
    {
      if (genome.x == j && genome.y == i)
        printf("x");
      else
        printf("%c", labirinth[i][j]);
    }
    printf("\n");
  }
}

double fitnessFunc(TeseuGenome& genome)
{
  int x = origin_x, y = origin_y;

  auto chromo = genome.extract();
  unsigned i;
  for (i = 0; i < chromo.size(); ++i)
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
    {
      genome.x = x;
      genome.y = y;
      genome.steps = i + 1;
      return 1.0;
    }
  }

  genome.x = x;
  genome.y = y;
  genome.steps = i;

  int dx = goal_x - x,
      dy = goal_y - y;
  return 0.8 / sqrt(dx * dx + dy * dy);
}

int main(int argc, char** argv)
{
  int population_size = 20;
  unsigned elite = 2;
  double mutation = 0.01, crossover = 0.7;
  if (argc > 1)
    population_size = atoi(argv[1]);
  if (argc > 2)
    elite           = atoi(argv[2]);
  if (argc > 3)
    mutation        = atof(argv[3]);
  if (argc > 4)
    crossover       = atof(argv[4]);

  ga::GeneticAlgorithm<TeseuGenome> evolution (population_size, N * M, elite,
                                               mutation, crossover,
                                               fitnessFunc);

  auto population = evolution.population();
  while (population[0].x != goal_x || population[0].y != goal_y)
  {
    printf("generation %d:\n", evolution.generation());

    population = evolution.population();
    for (unsigned j = 0; j < 1; ++j)
      printGenome(population[j]);
    printf("\n");

    evolution.epoch();
  }

  return 0;
}
