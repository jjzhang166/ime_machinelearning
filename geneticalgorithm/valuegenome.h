#ifndef GENETICALGORITHM_VALUEGENOME_H
#define GENETICALGORITHM_VALUEGENOME_H

#include <type_traits>
#include <vector>

#include "utils/rng.h"
#include "utils/clip.h"

#include "geneticalgorithm/genome.h"

namespace ga
{

template<typename T, bool Limited = true>
class ValueGenome : public Genome
{
public:
  using type_chromo = std::vector<T>;
  using type_value  = T;

  ValueGenome();
  ValueGenome(unsigned chromo_len,
              T min_value = T(0.0f), T max_value = T(1.0f),
              T max_mutation = T(0.1f));

  const type_chromo& extract() const;

  void mutate(double rate);
protected:
  type_chromo chromossome_;
  T min_value_, max_value_, max_mutation_;
};


// Template definition

template<typename T, bool Limited>
ValueGenome<T, Limited>::
ValueGenome()
{
  static_assert(std::is_arithmetic<T>::value, "Non arithmetic type!");
}

template<typename T, bool Limited>
ValueGenome<T, Limited>::
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

template<typename T, bool Limited>
const typename ValueGenome<T, Limited>::type_chromo& ValueGenome<T, Limited>::
extract() const
{
  return chromossome_;
}

template<typename T, bool Limited>
void ValueGenome<T, Limited>::
mutate(double rate)
{
  for (unsigned i = 0; i < chromossome_.size(); ++i)
  {
    if (rng::randFloat() < rate)
    {
      if (Limited)
      {
        T up_mut   = std::min(max_value_ - chromossome_[i], max_mutation_),
          down_mut = std::min(chromossome_[i] - min_value_, max_mutation_);
        chromossome_[i] = clip(chromossome_[i]
                               + T (rng::randFloat() * (up_mut + down_mut) - down_mut),
                               min_value_, max_value_);
      }
      else
        //TODO(rebeca): Fazer valer pra int
        chromossome_[i] += T (rng::randFloat() * 2 * max_mutation_) - max_mutation_;
    }
  }
}

}

#endif //GENETICALGORITHM_VALUEGENOME_H
