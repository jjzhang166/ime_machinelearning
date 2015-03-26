#ifndef GENETICALGORITHM_GENOME_H
#define GENETICALGORITHM_GENOME_H

#include <vector>

#include "utils/rng.h"

namespace ga
{

class Genome
{
public:
  Genome() : chromo_length_{0}, fitness_{0.0} {}
  Genome(unsigned chromo_len) : chromo_length_{chromo_len}, fitness_{0.0} {}

  void setFitness(double fitness) { fitness_ = fitness; }
  double getFitness() const { return fitness_; }

protected:
  unsigned chromo_length_;
  double fitness_;
};

}

#endif //GENETICALGORITHM_GENOME_H
