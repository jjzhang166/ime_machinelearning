#ifndef GENETICALGORITHM_GENALG_H
#define GENETICALGORITHM_GENALG_H

#include <functional>
#include <vector>
#include <utility>
#include <algorithm>
//
#include "utils/rng.h"

namespace ga
{

template<typename G>
class GeneticAlgorithm
{
public:
  GeneticAlgorithm();
  GeneticAlgorithm(unsigned population_size,
                   unsigned chromo_len,
                   unsigned elite_size,
                   double mutation_rate,
                   double crossover_rate,
                   std::function<double (const G&)> fitnessFunc);

  unsigned generation() const { return generation_; }
  void epoch();

  const std::vector<G>& population() const { return population_; }
  G get_best_of_all() const { return best_of_all_; }
protected:
  void reset();
  void populate();
  void calculateFitness();
  void sortPopulation();

  G select();

private:
  unsigned generation_ = 0;

  unsigned chromo_len_;
  unsigned population_size_;
  std::vector<G> population_;

  double total_fitness_;
  unsigned elite_size_;
  double mutation_rate_, crossover_rate_;

  std::function<double (G)> fitnessFunc_;

  G best_of_all_;
};

template<typename G>
void crossover(G& dad, G& mom)
{
  typename G::type_chromo chromo[2];
  chromo[0] = dad.extract();
  chromo[1] = mom.extract();
  unsigned chromo_len = chromo[0].size();

  unsigned cutpoint = rng::randFloat() * (chromo_len - 2) + 1;

  // TODO(naum): Make it better
  for (unsigned i = cutpoint; i < chromo_len; ++i)
    std::swap(chromo[0][i], chromo[1][i]);
}

// Definition
template<typename G>
GeneticAlgorithm<G>::
GeneticAlgorithm()
{}

template<typename G>
GeneticAlgorithm<G>::
GeneticAlgorithm(unsigned population_size,
                 unsigned chromo_len,
                 unsigned elite_size,
                 double mutation_rate,
                 double crossover_rate,
                 std::function<double (const G&)> fitnessFunc) :
  chromo_len_ {chromo_len},
  population_size_ {population_size},
  population_ {},
  total_fitness_ {0.0},
  elite_size_ {elite_size},
  mutation_rate_ {mutation_rate},
  crossover_rate_ {crossover_rate},
  fitnessFunc_ {fitnessFunc}
{
  populate();
  calculateFitness();
}

template<typename G>
void GeneticAlgorithm<G>::
reset()
{
  population_.resize(population_size_);
  for (unsigned i = 0; i < population_size_; ++i)
    population_[i] = G {chromo_len_};
}

template<typename G>
void GeneticAlgorithm<G>::
populate()
{
  if (population_.size() == 0)
    reset();
}

template<typename G>
void GeneticAlgorithm<G>::
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

template<typename G>
void GeneticAlgorithm<G>::
sortPopulation()
{
  std::sort(population_.begin(), population_.end(),
            [] (const G& a, const G& b) { return a.getFitness() > b.getFitness(); });
}

template<typename G>
G GeneticAlgorithm<G>::
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

template<typename G>
void GeneticAlgorithm<G>::
epoch()
{
  std::vector<G> offspring;

  // Elitism
  for(unsigned i = 0; i < elite_size_; ++i)
    offspring.push_back(population_[i]);

  while (offspring.size() < population_size_)
  {
    G children[2];

    // Selection
    // TODO(naum): Tournament
    children[0] = select();
    children[1] = select();

    // Crossover
    if (rng::randFloat() < crossover_rate_)
      crossover(children[0], children[1]);

    children[0].mutate(mutation_rate_);
    children[1].mutate(mutation_rate_);

    offspring.push_back(children[0]);
    if (offspring.size() < population_size_)
      offspring.push_back(children[1]);
  }

  population_ = offspring;
  calculateFitness();

  generation_++;
}

}

#endif //GENETICALGORITHM_GENALG_H
