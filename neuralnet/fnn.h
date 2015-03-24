#ifndef NEURALNET_FNN_H
#define NEURALNET_FNN_H

#include <vector>

namespace nn
{

struct Neuron
{
  std::vector<float> ws;
};

struct NeuronLayer
{
  std::vector<Neuron> neurons;
};

struct Feedforward
{
  std::vector<NeuronLayer> layers;
  int input_size = -1;

  void setInputSize(int s);
  void addLayer(int s);
  void randomize();
  std::vector<float> activate(std::vector<float> input);
  void print();
};

float sigmoid(float x);

}
#endif //NEURALNET_FNN_H
