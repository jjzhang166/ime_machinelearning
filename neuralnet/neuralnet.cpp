#include <cstdio>
#include <cmath>
#include <cassert>
#include <chrono>
#include <random>
#include <vector>

#include "utils/rng.h"
#include "neuralnet/neuralnet.h"

namespace nn
{

void NeuralNet::
setInputSize(int s)
{
  assert (s > 0);
  input_size = s;
}

void NeuralNet::
addLayer(int s)
{
  assert (input_size > 0);

  int prev_layer_size;
  if (layers.empty())
    prev_layer_size = input_size;
  else
    prev_layer_size = layers.back().neurons.size();

  layers.resize(layers.size() + 1);
  layers.back().neurons.resize(s);

  for (int i = 0; i < s; ++i)
    layers.back().neurons[i].ws.resize(prev_layer_size + 1);
}

void NeuralNet::
randomize()
{
  for (unsigned int l = 0; l < layers.size(); ++l)
    for (unsigned int n = 0; n < layers[l].neurons.size(); ++n)
      for (unsigned int w = 0; w < layers[l].neurons[n].ws.size(); ++w)
        layers[l].neurons[n].ws[w] = rng::randFloat();
}

std::vector<float> NeuralNet::
activate(std::vector<float> input)
{
  std::vector<float> activation;

  for (unsigned int l = 0; l < layers.size(); ++l)
  {
    activation.clear();

    for (uint n = 0; n < layers[l].neurons.size(); ++n)
    {
      float a = 0;
      for (uint i = 0; i < input.size(); ++i)
        a += input[i] * layers[l].neurons[n].ws[i];
      a -= layers[l].neurons[n].ws.back();
      activation.push_back(sigmoid(a));
    }

    input = activation;
  }

  return input;
}

void NeuralNet::
print()
{
  for (unsigned int l = 0; l < layers.size(); ++l)
  {
    printf("layer: %d\n", l);
    for (unsigned int n = 0; n < layers[l].neurons.size(); ++n)
    {
      printf("neuron[%d]: ", n);
      for (unsigned int w = 0; w < layers[l].neurons[n].ws.size(); ++w)
      {
        printf("%f ", layers[l].neurons[n].ws[w]);
      }
      printf("\n");
    }
  }
}


float sigmoid(float x)
{
  return 1.0f / (1.0f + exp(-x));
}

}
