#ifndef GENETICALGORITHM_BINARYGENOME_H
#define GENETICALGORITHM_BINARYGENOME_H

#include <vector>

#include "geneticalgorithm/genome.h"

namespace ga
{

class BinaryGenome : public Genome
{
public:
  // NOTE(naum): Can't iterate on vector<bool> and
  //             can't use bitset cause it's not dynamic
  using type_chromo = std::vector<char>;
  using type_value = bool;

  BinaryGenome();
  BinaryGenome(unsigned chromo_len);

  const type_chromo& extract() const;

  void encode(const std::vector<unsigned>& genes, unsigned gene_length);
  std::vector<unsigned> decode(unsigned gene_length) const;

  void mutate(double rate);
protected:
  type_chromo chromossome_;
};

}

#endif //GENETICALGORITHM_BINARYGENOME_H
