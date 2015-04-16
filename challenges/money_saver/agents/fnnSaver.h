#include "neuralnetwork/fnn.h"
#include "challenges/money_saver/agent.h"

class FnnSaver : public Agent
{
public:
  FnnSaver(const int vision_size);
  ~FnnSaver();
  virtual Direction walk(Terrain vision[]) override;

  nn::Feedforward brain;
};
