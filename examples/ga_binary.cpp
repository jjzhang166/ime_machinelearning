#include <stack>
#include <cmath>

#include "geneticalgorithm/binarygenome.h"
#include "geneticalgorithm/genalg.h"

int genomeValue(const ga::BinaryGenome& genome)
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

double fitnessFunc(const ga::BinaryGenome& genome)
{
  return 1.0 / (double)(exp(abs(genomeValue(genome) - 42) / 21.0));
}

void printGenome(const ga::BinaryGenome& genome)
{
  auto decoded = genome.decode(4);
  for (unsigned i = 0; i < decoded.size(); ++i)
  {
    int d = decoded[i];
    if (d <= 9)
      printf("%d ", d);
    else if (d >= 13)
      printf("x ");
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

  printf(" | ");
  for (unsigned i = 0; i < 32; ++i)
    printf("%d", genome.extract()[i]);

  printf("\n");
}

int main(int argc, char** argv)
{
  int population_size = 20, generations = 100;
  unsigned elite = 2;
  double mutation = 0.01, crossover = 0.7;
  if (argc > 1)
    population_size  = atoi(argv[1]);
  if (argc > 2)
    generations = atoi(argv[2]);
  if (argc > 3)
    elite       = atoi(argv[3]);
  if (argc > 4)
    mutation    = atoi(argv[4]);
  if (argc > 5)
    crossover   = atoi(argv[5]);

  ga::GeneticAlgorithm<ga::BinaryGenome> evolution (population_size, 32, elite, mutation, crossover, fitnessFunc);

  for (int i = 0; i < generations; ++i)
  {
    if (i > 0) evolution.epoch();

    printf("generation %d:\n", evolution.generation());

    printf("best:\n     %d %.2f: ", genomeValue(evolution.get_best_of_all()), fitnessFunc(evolution.get_best_of_all()));
    printGenome(evolution.get_best_of_all());
    printf("\n");

    auto population = evolution.population();
    for (unsigned j = 0; j < population.size(); ++j)
    {
      printf("%3d %3d %1.2f: ", j, genomeValue(population[j]), fitnessFunc(population[j]));
      printGenome(population[j]);
    }
    printf("\n");
  }

  printf("Best of all: %d\n", genomeValue(evolution.get_best_of_all()));

  return 0;
}
