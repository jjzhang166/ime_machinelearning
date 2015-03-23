#ifndef GENETICALGORITHM_VALUEGENOME_H
#define GENETICALGORITHM_VALUEGENOME_H

#include <type_traits>
#include <vector>

#include "utils/rng.h"
#include "geneticalgorithm/genome.h"

namespace ga
{

template<typename T>
class ValueGenome : public Genome
{
public:
  ValueGenome();
  ValueGenome(unsigned chromo_len);
  ValueGenome(unsigned chromo_len, unsigned numstates);

  const std::vector<T>& extract() const;

  virtual void mutate(double rate) override;
protected:
  std::vector<T> chromossome_;
  T max_mutation;
  unsigned num_states_ = 1;
};


// Template definition

template<typename T>
ValueGenome<T>::
ValueGenome()
{
  static_assert(std::is_arithmetic<T>::value, "Non arithmetic type!");
}

template<typename T>
ValueGenome<T>::
ValueGenome(unsigned chromo_len) :
  Genome {chromo_len},
  chromossome_ {},
  max_mutation {1},
  num_states_ {1}
{
  for (unsigned i = 0; i < chromo_length_; ++i)
    chromossome_.push_back(rng::randFloat() * num_states_);
}

template<typename T>
ValueGenome<T>::
ValueGenome(unsigned chromo_len, unsigned num_states) :
  Genome {chromo_len},
  chromossome_ {},
  max_mutation {1},
  num_states_ {num_states}
{
  for (unsigned i = 0; i < chromo_length_; ++i)
    chromossome_.push_back(rng::randFloat() * num_states_);
}

template<typename T>
const std::vector<T>& ValueGenome<T>::
extract() const
{
  return chromossome_;
}

template<typename T>
void ValueGenome<T>::
mutate(double rate)
{
  for (unsigned i = 0; i < chromossome_.size(); ++i)
    if (rng::randFloat() < rate)
      chromossome_[i] += rng::randFloat() * 2 * max_mutation - max_mutation;
}

}

#endif //GENETICALGORITHM_VALUEGENOME_H
