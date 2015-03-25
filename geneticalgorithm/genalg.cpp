#include <cstdio>
#include <utility>
#include <algorithm>

#include "utils/rng.h"
#include "geneticalgorithm/genalg.h"

namespace ga
{

BinaryGeneticAlgorithm::
BinaryGeneticAlgorithm()
{}

BinaryGeneticAlgorithm::
BinaryGeneticAlgorithm(unsigned population_size,
                       unsigned chromo_len,
                       double elite_rate,
                       double mutation_rate,
                       double crossover_rate,
                       std::function<double (const BinaryGenome&)> fitnessFunc) :
  chromo_len_ {chromo_len},
  population_size_ {population_size},
  population_ {},
  total_fitness_ {0.0},
  elite_size_ {static_cast<unsigned>(population_size_ * elite_rate)},
  mutation_rate_ {mutation_rate},
  crossover_rate_ {crossover_rate},
  fitnessFunc_ {fitnessFunc}
{
}

void BinaryGeneticAlgorithm::
reset()
{
  population_.resize(population_size_);
  for (unsigned i = 0; i < population_size_; ++i)
    population_[i] = BinaryGenome {chromo_len_};
}

void BinaryGeneticAlgorithm::
populate()
{
  if (population_.size() == 0)
    reset();
  calculateFitness();
}

void BinaryGeneticAlgorithm::
calculateFitness()
{
  total_fitness_ = 0.0;
  for (unsigned i = 0; i < population_size_; ++i)
  {
    double fitness = fitnessFunc_(population_[i]);
    population_[i].setFitness(fitness);

    if (fitness > best_of_all_.getFitness())
      best_of_all_ = population_[i];

    total_fitness_ += fitness;
  }

  sortPopulation();
}

void BinaryGeneticAlgorithm::
sortPopulation()
{
  std::sort(population_.begin(), population_.end(),
            [] (BinaryGenome a, BinaryGenome b) { return a.getFitness() > b.getFitness(); });
}

BinaryGenome BinaryGeneticAlgorithm::
select()
{
  // TODO(naum): Tournament

  // Roulette Whell Selection
  double selected = rng::randFloat() * total_fitness_;
  for (unsigned i = 0; i < population_size_; ++i)
  {
    double fitness = population_[i].getFitness();
    if (selected <= fitness)
      return population_[i];
    selected -= fitness;
  }
}

void BinaryGeneticAlgorithm::
crossover(BinaryGenome& dad, BinaryGenome& mom)
{
  unsigned cutpoint = rng::randFloat() * (chromo_len_ - 2) + 1;

  BinaryGenome::type offspring[2];
  offspring[0] = dad.extract();
  offspring[1] = mom.extract();

#ifndef NDEBUG
  printf("crossover\n");
  printf("before: ");
  for (unsigned i = 0; i < offspring[0].size(); ++i)
  {
    if (i == cutpoint) printf(" ");
    printf("%d", offspring[0][i]);
  }
  printf("\n");
#endif

  // TODO(naum): Make it better
  for (unsigned i = cutpoint; i < chromo_len_; ++i)
    std::swap(offspring[0][i], offspring[1][i]);

#ifndef NDEBUG
  printf("after : ");
  for (unsigned i = 0; i < offspring[0].size(); ++i)
  {
    if (i == cutpoint) printf(" ");
    printf("%d", offspring[0][i]);
  }
  printf("\n\n");
#endif
}

void BinaryGeneticAlgorithm::
epoch()
{
  populate();

  std::vector<BinaryGenome> offspring;

  // Elitism
  for(unsigned i = 0; i < elite_size_; ++i)
  {
    offspring.push_back(population_[i]);
  }

  while (offspring.size() < population_size_)
  {
    BinaryGenome children[2];

    // Selection
    // TODO(naum): Tournament
    children[0] = select();
    children[1] = select();

    // Crossover
    if (rng::randFloat() < crossover_rate_)
      crossover(children[0], children[1]);

    // Mutation
#ifndef NDEBUG
    printf("mutation\n");
    printf("before: ");
    for (unsigned i = 0; i < children[0].extract().size(); ++i)
      printf("%d", children[0].extract()[i]);
    printf("\n");
#endif
    children[0].mutate(mutation_rate_);
    children[1].mutate(mutation_rate_);
#ifndef NDEBUG
    printf("after : ");
    for (unsigned i = 0; i < children[1].extract().size(); ++i)
      printf("%d", children[0].extract()[i]);
    printf("\n\n");
#endif

    offspring.push_back(children[0]);
    if (offspring.size() < population_size_)
      offspring.push_back(children[1]);
  }

  population_ = offspring;
  calculateFitness();

  generation_++;
}

}
