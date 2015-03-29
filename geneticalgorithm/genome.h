#ifndef GENETICALGORITHM_GENOME_H
#define GENETICALGORITHM_GENOME_H

namespace ga
{

/**
 * Genome base class.
 * Genomes (usually) share this interface and basic attributes.
 * This is not used for polymorphism! GeneticAlgorithm works
 * with templates, new Genomes can be created simply by sharing
 * the interface GeneticAlgorithm needs, not by inheriting from
 * Genome.
 */
class Genome
{
public:
  /**
   * Default constructor.
   * Instantiate all to zero
   */
  Genome() : chromo_length_{0}, fitness_{0.0}
  {}

  /**
   * Explicit constructor.
   * Necessary for GeneticAlgorithm genome creating.
   * Sets chromossome length and resets fitness to zero.
   * \param chromo_len Chromossome length (initial, in case of a variable size
   *                   chromossome
   */
  explicit Genome(unsigned chromo_len) : chromo_length_{chromo_len}, fitness_{0.0}
  {}

  /**
   * Fitness setter.
   * Sets fitness value.
   * \param fitness New fitness value
   */
  void setFitness(double fitness) { fitness_ = fitness; }

  /**
   * Fitness getter.
   * Returns genome fitness.
   * \return Fitness value
   */
  double getFitness() const { return fitness_; }

protected:
  unsigned chromo_length_; /**< Chromossome length */
  double fitness_;         /**< Fitness value */
};

}

#endif //GENETICALGORITHM_GENOME_H
