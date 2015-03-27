#include <cstdio>

#include "geneticalgorithm/binarygenome.h"
#include "geneticalgorithm/valuegenome.h"

int main()
{
  {
    printf("Binary Genome:\n");
    ga::BinaryGenome genome {10};

    for (int i = 0; i < 10; ++i)
    {
      auto genes = genome.extract();
      for (unsigned i = 0; i < genes.size(); ++i)
        printf("%d", genes[i]);
      printf("\n");
      genome.mutate(0.1);
    }
    printf("\n");
  }

  {
    printf("Float Value Genome (0.0f ~ 1.0f):\n");
    ga::ValueGenome<float> genome {10};

    for (int i = 0; i < 10; ++i)
    {
      auto genes = genome.extract();
      for (unsigned i = 0; i < genes.size(); ++i)
        printf("%.2f ", genes[i]);
      printf("\n");
      genome.mutate(0.1);
    }
    printf("\n");
  }

  {
    printf("Int Value Genome (0 ~ 9, mutation: 1):\n");
    ga::ValueGenome<int> genome {10, 0, 10, 1};

    for (int i = 0; i < 10; ++i)
    {
      auto genes = genome.extract();
      for (unsigned i = 0; i < genes.size(); ++i)
        printf("%d ", genes[i]);
      printf("\n");
      genome.mutate(0.1);
    }
    printf("\n");
  }

  {
    printf("Int Value Genome (0 ~ 9, random mutation):\n");
    ga::ValueGenome<int, true, true> genome {10, 0, 10};

    for (int i = 0; i < 10; ++i)
    {
      auto genes = genome.extract();
      for (unsigned i = 0; i < genes.size(); ++i)
        printf("%d ", genes[i]);
      printf("\n");
      genome.mutate(0.1);
    }
    printf("\n");
  }

  {
    printf("Int Value Genome (0 ~ 9, not limited, mutation: 1):\n");
    ga::ValueGenome<int, false, false> genome {10, 0, 10, 0.1};

    for (int i = 0; i < 10; ++i)
    {
      auto genes = genome.extract();
      for (unsigned i = 0; i < genes.size(); ++i)
        printf("%d ", genes[i]);
      printf("\n");
      genome.mutate(0.1);
    }
    printf("\n");
  }

  return 0;
}
