#include <cstdio>

#include "neuralnet/fnn.h"

int main()
{
  nn::Feedforward net;
  net.addLayers(1, 4, 2);
  net.randomize();
  net.print();

  std::vector<float> inputs;

  float f;
  while (scanf("%f", &f) != EOF)
  {
    inputs.clear();
    inputs.push_back(f);
    inputs = net.activate(inputs);

    for (unsigned int i = 0; i < inputs.size(); ++i)
      printf("%f ", inputs[i]);
    printf("\n");
  }

  return 0;
}
