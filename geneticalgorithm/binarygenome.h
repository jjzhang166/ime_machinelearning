#ifndef GENETICALGORITHM_BINARYGENOME_H
#define GENETICALGORITHM_BINARYGENOME_H

#include <vector>

#include "geneticalgorithm/genome.h"

namespace ga
{

class BinaryGenome : public Genome
{
public:
  BinaryGenome();
  BinaryGenome(unsigned chromo_len);

  const std::vector<char>& extract() const;

  void encode(const std::vector<unsigned>& genes, unsigned gene_length);
  std::vector<unsigned> decode(unsigned gene_length);

  virtual void mutate(double rate) override;
protected:
  // NOTE(naum): Can't iterate on vector<bool> and
  //             can't use bitset cause it's not dynamic
  std::vector<char> chromossome_;
};

}

#endif //GENETICALGORITHM_BINARYGENOME_H
