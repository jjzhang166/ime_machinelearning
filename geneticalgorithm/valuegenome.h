#ifndef GENETICALGORITHM_VALUEGENOME_H
#define GENETICALGORITHM_VALUEGENOME_H

#include <type_traits>
#include <vector>

#include "utils/rng.h"
#include "utils/clip.h"

#include "geneticalgorithm/genome.h"

namespace ga
{

template<typename T, bool Limited = true, bool RandomMutation = false>
class ValueGenome : public Genome
{
public:
  using type_chromo = std::vector<T>;
  using type_value  = T;

  ValueGenome();
  ValueGenome(unsigned chromo_len, T min_value = T(0.0), T max_value = T(1.0), double max_mutation_percent = 0.1);

  const type_chromo& extract() const;

  void mutate(double rate);
protected:
  type_chromo chromossome_;
  T min_value_, max_value_, max_mutation_;
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
ValueGenome(unsigned chromo_len, T min_value, T max_value, double max_mutation_percent) :
  Genome {chromo_len},
  min_value_ {min_value},
  max_value_ {max_value},
  max_mutation_ {T(max_mutation_percent * (max_value_ - min_value_))}
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
namespace impl
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
    T up_mut   = std::min(max_value - chromossome, max_mutation),
      down_mut = std::min(chromossome - min_value, max_mutation);
    chromossome += T (rng::randFloat() * (up_mut + down_mut) - down_mut);
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
      impl::mutateChromossome<T, Limited, RandomMutation>::mutate(chromossome_[i], min_value_, max_value_, max_mutation_);
}

}

#endif //GENETICALGORITHM_VALUEGENOME_H
