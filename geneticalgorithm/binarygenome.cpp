#include "geneticalgorithm/binarygenome.h"
#include "utils/rng.h"

namespace ga
{

BinaryGenome::
BinaryGenome()
{}

BinaryGenome::
BinaryGenome(unsigned chromo_len) :
  Genome {chromo_len}
{
  for (unsigned i = 0; i < chromo_length_; ++i)
    chromossome_.push_back(rng::randBool());
}

const BinaryGenome::type_chromo& BinaryGenome::
extract() const
{
  return chromossome_;
}

void BinaryGenome::
encode(const std::vector<unsigned>& genes, unsigned gene_length)
{
  for (unsigned i = 0; i < genes.size(); i++)
  {
    unsigned a = genes[i];
    for (int j = gene_length - 1; j >= 0; --j)
    {
      chromossome_[i * gene_length + j] = a % 2;
      a >>= 1;
    }
  }
}

std::vector<unsigned> BinaryGenome::
decode(unsigned gene_length) const
{
  std::vector<unsigned> decoded;
  int numgenes = chromossome_.size() / gene_length;
  for (int i = 0; i < numgenes; ++i)
  {
    unsigned a = 0;
    for (unsigned j = 0; j < gene_length; ++j)
    {
      a <<= 1;
      a += chromossome_[gene_length * i + j];
    }
    decoded.push_back(a);
  }
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
