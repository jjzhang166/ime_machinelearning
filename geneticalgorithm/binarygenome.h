#ifndef GENETICALGORITHM_BINARYGENOME_H
#define GENETICALGORITHM_BINARYGENOME_H

#include <vector>

#include "geneticalgorithm/genome.h"

namespace ga
{

/**
 * Generic class for binary genomes.
 * Alongside with the GeneticAlgorithm interface (extract, mutate,
 * unsigned param constructor), it has encode and decode methods.
 * Encoding and decoding with a provided gene length. If you want to
 * encode/decode another way, inherit this class (note that it is not
 * polymorphic) and create your own encode/decode methods.
 */
class BinaryGenome : public Genome
{
public:
  // NOTE(naum): Can't iterate on vector<bool> and
  //             can't use bitset cause it's not dynamic
  /**
   * Chromossome type.
   * Should be std::vector<bool> or std::bitset<>.
   * With std::vector<bool> we can't iterate on each bool value.
   * With std::bitset<> we have to define at compile-tipe the chromossome
   * length. Since we want the possibility to have a variable size chromossome,
   * std::bitset<> would limit us.
   */
  using type_chromo = std::vector<char>;
  using type_value = bool; /**< Value type of unit (genes?) */

  /**
   * Default constructor.
   * Calls Genome default constructor
   */
  BinaryGenome();

  /**
   * Explicit constructor.
   * Necessary for GeneticAlgorithm genomes creation.
   * Initializes a random chromossome.
   * \param chromo_len Chromossome length
   */
  explicit BinaryGenome(unsigned chromo_len);

  /**
   * Extract chromossome.
   * Returns the whole chromossome (const reference).
   * \return Chromossome vector (const reference).
   */
  const type_chromo& extract() const;

  /**
   * Encode integers into binary chromossome.
   * Converts integer genes with at most \a gene_length binary bits into binary
   * and saves into the \a chromossome_.
   * \param genes Vector of unsigned int genes.
   * \param gene_length Number of bits that will be encoded into binary for gene.
   */
  void encode(const std::vector<unsigned>& genes, unsigned gene_length);

  /**
   * Decode binary chromossome into integers.
   * Converts the binary chromossome into a vector of unsigned integers.
   * Every \a gene_length bits from the \a chromossome_ will be decoded into
   * a single integer.
   * \param gene_length Number of bits of each gene.
   * \return Vector of unsigned integers decoded from the binary chromossome.
   */
  std::vector<unsigned> decode(unsigned gene_length) const;

  /**
   * Flip bits of the chromossome based on the mutation rate.
   * Randomly flip bits of the chromossome given the mutation rate.
   * Necessary for GeneticAlgorithm interfacing.
   * \param rate Mutation rate (between 0 and 1).
   */
  void mutate(double rate);
protected:
  type_chromo chromossome_; /**< Chromossome */
};

}

#endif //GENETICALGORITHM_BINARYGENOME_H
