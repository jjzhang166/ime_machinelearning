#include "geneticalgorithm/binarygenome.h"
#include "utils/rng.h"

namespace ga
{

BinaryGenome::
BinaryGenome() :
  chromossome_{}
{}

BinaryGenome::
BinaryGenome(unsigned chromo_len) :
  Genome{chromo_len},
  chromossome_ {}
{
  for (unsigned i = 0; i < chromo_length_; ++i)
    chromossome_.push_back(rng::randBool());
}

const std::vector<char>& BinaryGenome::
extract() const
{
  return chromossome_;
}

void BinaryGenome::
encode(const std::vector<unsigned>& genes, unsigned gene_length)
{
}

std::vector<unsigned> BinaryGenome::
decode(unsigned gene_length)
{
  std::vector<unsigned> decoded;
  int numgenes = (chromossome_.size() / gene_length) - 1;
  for (int i = 0; i < numgenes; ++i)
    decoded.push_back((chromossome_[i    ] << 2) +
                      (chromossome_[i + 1] << 1) +
                      (chromossome_[i + 2]));
  return decoded;
}

void BinaryGenome::
mutate(double rate)
{
  for (unsigned i = 0; i < chromossome_.size(); ++i)
    if (rng::randFloat() < rate)
      chromossome_[i] = !chromossome_[i];
}

}
