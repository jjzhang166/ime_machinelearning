#ifndef GENETICALGORITHM_GENALG_H
#define GENETICALGORITHM_GENALG_H

#include <functional>
#include <vector>
#include <utility>

#include "geneticalgorithm/binarygenome.h"

namespace ga
{

class BinaryGeneticAlgorithm
{
public:
  BinaryGeneticAlgorithm();
  BinaryGeneticAlgorithm(unsigned population_size,
                         unsigned chromo_len,
                         double mutation_rate,
                         double crossover_rate,
                         std::function<double (BinaryGenome)> fitnessFunc);

  unsigned generation() const { return generation_; }
  void epoch();

  const std::vector<BinaryGenome>& population() const { return population_; }
  BinaryGenome get_best_of_all() const { return best_of_all_; }
protected:
  void reset();
  void populate();
  void calculateFitness();

  BinaryGenome& select();
  std::pair<BinaryGenome, BinaryGenome> crossover(const BinaryGenome& dad,
                                                  const BinaryGenome& mom);

private:
  unsigned generation_ = 0;

  unsigned chromo_len_;
  unsigned population_size_;
  std::vector<BinaryGenome> population_;

  double total_fitness_;
  double mutation_rate_, crossover_rate_;

  std::function<double (BinaryGenome)> fitnessFunc_;

  BinaryGenome best_of_all_;
};

}

#endif //GENETICALGORITHM_GENALG_H
