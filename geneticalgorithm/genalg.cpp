#include <cstdio>
#include <utility>

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
                       double mutation_rate,
                       double crossover_rate,
                       std::function<double (BinaryGenome)> fitnessFunc) :
  chromo_len_ {chromo_len},
  population_size_ {population_size},
  population_ {},
  total_fitness_ {0.0},
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
}

BinaryGenome& BinaryGeneticAlgorithm::
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

std::pair<BinaryGenome, BinaryGenome> BinaryGeneticAlgorithm::
crossover(const BinaryGenome& dad, const BinaryGenome& mom)
{
  unsigned cutpoint = rng::randFloat() * chromo_len_;

  BinaryGenome::type offspring[2];
  offspring[0] = dad.extract();
  offspring[1] = mom.extract();

  for (unsigned i = cutpoint; i < chromo_len_; ++i)
  {
    auto t = offspring[0][i];
    offspring[0][i] = offspring[1][i];
    offspring[1][i] = t;
  }

  return std::make_pair(BinaryGenome {offspring[0]}, BinaryGenome {offspring[1]});
}

void BinaryGeneticAlgorithm::
epoch()
{
  populate();
  calculateFitness();

  std::vector<BinaryGenome> offspring;

  // TODO(naum): Elite
  // TODO(naum): Tournament

  while (offspring.size() < population_size_)
  {
    BinaryGenome children[2];

    // Selection
    BinaryGenome dad = select(),
                 mom = select();

    // Crossover
    if (rng::randFloat() < crossover_rate_)
      std::tie(children[0], children[1]) = crossover(dad, mom);

    // Mutation
    children[0].mutate(mutation_rate_);
    children[1].mutate(mutation_rate_);

    offspring.push_back(children[0]);
    if (offspring.size() < population_size_)
      offspring.push_back(children[1]);
  }

  // NOTE(naum): Esqueci disso -.-'
  population_ = offspring;
  printf("pop: %zd\n", population_.size());

  generation_++;
}

}
