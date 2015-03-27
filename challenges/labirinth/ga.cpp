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

class TeseuGenome : public ga::ValueGenome<int>
{
public:
  TeseuGenome() {}
  TeseuGenome(unsigned chromo_len) : ValueGenome(chromo_len, 0, 4, 4)
  {}
};

bool simulate(const TeseuGenome& genome, int* end_x = nullptr, int* end_y = nullptr)
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
    {
      if (end_x) *end_x = x;
      if (end_y) *end_y = y;
      return true;
    }
  }

  if (end_x) *end_x = x;
  if (end_y) *end_y = y;

  return false;
}

void printGenome(const TeseuGenome& genome)
{
  auto chromo = genome.extract();
  for (unsigned j = 0; j < chromo.size(); ++j)
    printf("%c", (chromo[j] == 0) ? 'U' : ((chromo[j] == 1) ? 'D' : ((chromo[j] == 2) ? 'L' : 'R')));
  printf("\n");

  int x, y;
  bool found = simulate(genome, &x, &y);
  for (int i = 0; i < N; i++)
  {
    for (int j = 0; j < M; ++j)
    {
      if (x == j && y == i)
        printf("x");
      else
        printf("%c", labirinth[i][j]);
    }
    printf("\n");
  }

  if (found)
    printf("Found!\n");
}

double fitnessFunc(const TeseuGenome& genome)
{
  int x, y;
  if (simulate(genome, &x, &y))
    return 1.0;

  int dx = goal_x - x,
      dy = goal_y - y;
  return 0.8 / sqrt(dx * dx + dy * dy);
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
    for (unsigned j = 0; j < 1; ++j)
    {
      //printf("%2d\n", j);
      printGenome(population[j]);
    }
    printf("\n");
  }

  return 0;
}
