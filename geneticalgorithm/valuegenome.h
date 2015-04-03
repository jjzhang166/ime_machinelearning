#ifndef GENETICALGORITHM_VALUEGENOME_H
#define GENETICALGORITHM_VALUEGENOME_H

#include <type_traits>
#include <vector>
#include <cmath>

#include "utils/rng.h"
#include "utils/clip.h"

#include "geneticalgorithm/genome.h"

namespace ga
{

/**
 * Generic class for value genomes.
 * Template class that defines a general value genome.
 * NOTE: This class does not have the explicit single unsigned constructor needed
 * ont GeneticAlgorithm interface. So you have to inherit this class into another
 * one that have the constructor needed.
 * \param T Value type.
 * \param Limited If value is only change inside construction min and max or not.
 * \param RandomMutation If mutations changes values to completely random value
 *                       between \a min_value_ and \a max_value_ or mutations
 *                       causes changes to a maximum of \a max_mutation_.
 */
template<typename T, bool Limited = true, bool RandomMutation = false>
class ValueGenome : public Genome
{
public:
  using type_chromo = std::vector<T>; /**< Chromossome type */
  using type_value  = T;  /**< Value type of genes */

  /**
   * Default constructor.
   * Calls Genome default constructor.
   */
  ValueGenome();

  /**
   * Minimum-maximum constructor.
   * Initialize a random chromossome and sets \a min_value, \a max_value_ and
   * \a max_mutation_ (if \a RandomMutation is true, this attribute will
   * not be used).
   * \todo For integer values, change \a max_value_ to be inclusive.
   * \param chromo_len Chromossome length
   * \param min_value If \a Limited and not \a RandomMutation, the minimum value
   *                  the genes can have. Else, the initial minimum value the
   *                  genes can have.
   * \param max_value If \a Limited and not \a RandomMutation, the maximum value
   *                  the genes can have. Else, the initial maximum value the
   *                  genes can have.
   * \param max_mutation If \a RandomMutation is false, maximum mutation value
   *                     range. Else, ignored.
   */
  ValueGenome(unsigned chromo_len,
              T min_value, T max_value, T max_mutation = T(0));

  /**
   * Extracts chromossome.
   * Necessary for GeneticAlgorithm genomes creation.
   * Return the whole chromossome (const reference).
   * \return Chromossome vector (const reference).
   */
  const type_chromo& extract() const;

  /**
   * Mutate genes.
   * If \a RandomMutation is true, the values of some genes, based on the
   * mutation rate, change into completely new values between \a min_value_
   * and \a max_value_ (exclusive).
   * \todo For integer values, change \a max_value_ to be inclusive.
   * \param rate Mutation rate (between 0 and 1)
   */
  void mutate(double rate);
protected:
  type_chromo chromossome_; /**< Chromossome */
  T min_value_, /**< Minimum gene value */
    max_value_, /**< Maximum gene value (exclusive for integral types) */
    max_mutation_; /**< Maximum gene value mutation range */
};


// Template definition

template<typename T, bool Limited, bool RandomMutation>
ValueGenome<T, Limited, RandomMutation>::
ValueGenome()
{
  static_assert(std::is_arithmetic<T>::value, "Non arithmetic type!");
}

template<typename T, bool Limited, bool RandomMutation>
ValueGenome<T, Limited, RandomMutation>::
ValueGenome(unsigned chromo_len, T min_value, T max_value, T max_mutation) :
  Genome {chromo_len},
  min_value_ {min_value},
  max_value_ {max_value},
  max_mutation_ {max_mutation}
{
  for (unsigned i = 0; i < chromo_length_; ++i)
    chromossome_.push_back(T (rng::randFloat() * (max_value_ - min_value_)
                              + min_value_));
}


template<typename T, bool Limited, bool RandomMutation>
const typename ValueGenome<T, Limited, RandomMutation>::type_chromo& ValueGenome<T, Limited, RandomMutation>::
extract() const
{
  return chromossome_;
}

// ------------------------------------------------
// Internal namespace to specialize mutate function
// ------------------------------------------------
namespace priv
{

template<typename T, bool Limited, bool RandomMutation>
struct mutateChromossome;

// Not limited and not random mutating)
template<typename T>
struct mutateChromossome<T, false, false>
{
  static void mutate(T& chromossome, T min_value, T max_value, T max_mutation)
  {
    chromossome += T (rng::randFloat() * (std::is_integral<T>::value ? 3 : 2) * max_mutation) - max_mutation;
  }
};

// Limited and not random mutating
template<typename T>
struct mutateChromossome<T, true, false>
{
  static void mutate(T& chromossome, T min_value, T max_value, T max_mutation)
  {
    T up_mut   = std::min(max_value - chromossome + (std::is_integral<T>::value ? 1 : 0), max_mutation),
      down_mut = std::min(chromossome - min_value, max_mutation);
    chromossome += T (rng::randFloat() * (up_mut + down_mut)) - down_mut;
  }
};

// Random mutating (implicit limited)
// Doesn't include max_value
template<typename T, bool Limited>
struct mutateChromossome<T, Limited, true>
{
  static void mutate(T& chromossome, T min_value, T max_value, T max_mutation)
  {
    chromossome = T (rng::randFloat() * (max_value - min_value));
  }
};

}
// ------------------------------------------------
//
// ------------------------------------------------


template<typename T, bool Limited, bool RandomMutation>
void ValueGenome<T, Limited, RandomMutation>::
mutate(double rate)
{
  for (unsigned i = 0; i < chromossome_.size(); ++i)
    if (rng::randFloat() < rate)
      priv::mutateChromossome<T, Limited, RandomMutation>::mutate(chromossome_[i], min_value_, max_value_, max_mutation_);
}

}

#endif //GENETICALGORITHM_VALUEGENOME_H
