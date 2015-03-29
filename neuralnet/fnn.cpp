#include <cstdio>
#include <cmath>
#include <cassert>
#include <chrono>
#include <random>
#include <vector>

#include "utils/rng.h"
#include "neuralnet/fnn.h"

namespace nn
{

/*
void Feedforward::
setInputSize(int s)
{
  assert (s > 0);
  input_size_ = s;
}

void Feedforward::
addLayer(int s)
{
  assert (input_size_ > 0);

  int prev_layer_size;
  if (layers_.empty())
    prev_layer_size = input_size_;
  else
    prev_layer_size = layers_.back().neurons.size();

  layers_.resize(layers_.size() + 1);
  layers_.back().neurons.resize(s);

  for (int i = 0; i < s; ++i)
    layers_.back().neurons[i].ws.resize(prev_layer_size + 1);
}
*/

void Feedforward::
randomize()
{
  for (unsigned l = 0; l < layers_.size(); ++l)
    for (unsigned n = 0; n < layers_[l].neurons.size(); ++n)
      for (unsigned w = 0; w < layers_[l].neurons[n].ws.size(); ++w)
        layers_[l].neurons[n].ws[w] = rng::randFloat();
}

void Feedforward::
addLayer(unsigned s)
{
  if (input_size_ < 0)
    input_size_ = s;
  else
  {
    int prev_layer_size;
    if (layers_.empty())
      prev_layer_size = input_size_;
    else
      prev_layer_size = layers_.back().neurons.size();

    layers_.push_back(NeuronLayer ());
    layers_.back().neurons.resize(s);

    for (unsigned i = 0; i < s; ++i)
      layers_.back().neurons[i].ws.resize(prev_layer_size + 1);
  }
}

void Feedforward::
setWeights(const std::vector<float>& ws)
{
  unsigned i = 0;
  for (unsigned l = 0; l < layers_.size(); ++l)
    for (unsigned n = 0; n < layers_[l].neurons.size(); ++n)
      for (unsigned w = 0; w < layers_[l].neurons[n].ws.size(); ++w)
        layers_[l].neurons[n].ws[w] = ws[i++];
}

void Feedforward::
reset()
{
  layers_.clear();
  input_size_ = -1;
}

std::vector<float> Feedforward::
activate(std::vector<float> input)
{
  std::vector<float> activation;

  for (unsigned int l = 0; l < layers_.size(); ++l)
  {
    activation.clear();

    for (uint n = 0; n < layers_[l].neurons.size(); ++n)
    {
      float a = 0;
      for (uint i = 0; i < input.size(); ++i)
        a += input[i] * layers_[l].neurons[n].ws[i];
      a -= layers_[l].neurons[n].ws.back();
      activation.push_back(sigmoid(a));
    }

    input = activation;
  }

  return input;
}

void Feedforward::
print() const
{
  for (unsigned int l = 0; l < layers_.size(); ++l)
  {
    printf("layer: %d\n", l);
    for (unsigned int n = 0; n < layers_[l].neurons.size(); ++n)
    {
      printf("neuron[%d]: ", n);
      for (unsigned int w = 0; w < layers_[l].neurons[n].ws.size(); ++w)
      {
        printf("%f ", layers_[l].neurons[n].ws[w]);
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
