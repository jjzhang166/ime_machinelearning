#include <cstdio>

#include "geneticalgorithm/binarygenome.h"
#include "geneticalgorithm/valuegenome.h"

int main()
{
  ga::BinaryGenome bindividual {10};
  auto genes = bindividual.extract();
  for (unsigned i = 0; i < genes.size(); ++i)
    printf("%d", genes[i]);
  printf("\n");

  ga::ValueGenome<float> findividual {10};
  auto fgenes = findividual.extract();
  for (unsigned i = 0; i < fgenes.size(); ++i)
    printf("%f ", fgenes[i]);
  printf("\n");

  ga::ValueGenome<int> iindividual {10, 0, 10, 1};
  auto igenes = iindividual.extract();
  for (unsigned i = 0; i < igenes.size(); ++i)
    printf("%d ", igenes[i]);
  printf("\n");

  return 0;
}
