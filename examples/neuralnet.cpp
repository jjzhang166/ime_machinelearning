#include <cstdio>

#include "neuralnet/neuralnet.h"

int main()
{
  nn::NeuralNet net;
  net.setInputSize(1);
  net.addLayer(4);
  net.addLayer(2);
  net.randomize();
  net.print();

  std::vector<float> inputs;

  for (int i = 0; i < 3; ++i)
  {
    float f;
    scanf("%f", &f);
    inputs.push_back(f);
  }

  inputs = net.activate(inputs);

  for (unsigned int i = 0; i < inputs.size(); ++i)
    printf("%f ", inputs[i]);
  printf("\n");

  return 0;
}
