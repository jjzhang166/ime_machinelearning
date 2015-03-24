#include <stack>
#include <cmath>

#include "geneticalgorithm/binarygenome.h"
#include "geneticalgorithm/genalg.h"

int genomeValue(ga::BinaryGenome genome)
{
  auto decoded = genome.decode(4);
  std::stack<int> values;
  for (unsigned i = 0; i < decoded.size(); ++i)
  {
    unsigned d = decoded[i];
    if (d <= 9)
      values.push(d);
    else if (d >= 13)
      continue;
    else if (values.size() >= 2)
    {
      int a = values.top(); values.pop();
      int b = values.top(); values.pop();
      if (d == 10) // Sum
        values.push(b + a);
      else if (d == 11) // Difference
        values.push(b - a);
      else if (d == 12) // Multiplication
        values.push(a * b);
    }
  }

  if (values.size() == 0)
    return 0;

  return values.top();
}

double fitnessFunc(ga::BinaryGenome genome)
{
  return 1.0 / (double)(exp(abs(genomeValue(genome) - 42) / 21.0));
}

void printGenome(ga::BinaryGenome genome)
{
  auto decoded = genome.decode(4);
  for (unsigned i = 0; i < decoded.size(); ++i)
  {
    int d = decoded[i];
    if (d <= 9 || d >= 13)
      printf("%d ", d);
    else
    {
      if (d == 10) // Sum
        printf("+ ");
      else if (d == 11) // Difference
        printf("- ");
      else if (d == 12) // Multiplication
        printf("* ");
    }
  }
  printf("\n");
}

int main(int argc, char** argv)
{
  int population = 20, generations = 10;
  if (argc > 1)
    population = atoi(argv[1]);
  if (argc > 2)
    generations = atoi(argv[2]);

  ga::BinaryGeneticAlgorithm evolution (population, 32, 0.01, 0.7, fitnessFunc);

  for (int i = 0; i < generations; ++i)
  {
    evolution.epoch();
    printf("generation %d:\n", evolution.generation());
    printf("best %d: ", genomeValue(evolution.get_best_of_all()));
    printGenome(evolution.get_best_of_all());

    auto population = evolution.population();
    for (unsigned j = 0; j < population.size(); ++j)
    {
      printf("%d %.2f: ", genomeValue(population[j]), fitnessFunc(population[j]));
      printGenome(population[j]);
    }
    printf("\n");
  }

  printf("Best of all: %d\n", genomeValue(evolution.get_best_of_all()));

  return 0;
}
