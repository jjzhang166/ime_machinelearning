#ifndef NEURALNETWORK_FNN_H
#define NEURALNETWORK_FNN_H

#include <vector>

#include "neuralnetwork/neuron.h"

namespace nn
{


class Feedforward
{
public:
  struct NeuronLayer
  {
    std::vector<Neuron> neurons;
  };

  void addLayers(unsigned s);

  template<typename... Args>
  void addLayers(unsigned first, Args... args);

  void setWeights(const std::vector<float>& ws);
  void randomize();
  void reset();

  std::vector<float> activate(std::vector<float> input);
  void print() const;
protected:
private:
  std::vector<NeuronLayer> layers_;
  int input_size_ = -1;
};

// TODO(naum): Put this into another (generic) header
float sigmoid(float x);


// Template definitions
template<typename... Args>
void Feedforward::
addLayers(unsigned first, Args... args)
{
  addLayers(first);
  addLayers(args...);
}

}
#endif //NEURALNETWORK_FNN_H
