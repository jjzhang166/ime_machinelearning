#include <cstdio>
#include "fnnSaver.h"

FnnSaver::
FnnSaver(const int vision_size) : Agent {vision_size}
{
  auto v2 = (2 * vision_size + 1) * (2 * vision_size + 1);
  brain.addLayers(v2, 32, static_cast<int>(Direction::NUM_DIRECTIONS));
}

FnnSaver::
~FnnSaver()
{
}

Direction FnnSaver::
walk(Terrain vision[])
{
  int num_terrain = static_cast<int>(Terrain::NUM_TERRAINS);
  std::vector<float> data;
  for (int i = 0; i < vision_size; ++i)
  {
    int vi = static_cast<int>(vision[i]);
    for (int j = 0; j < num_terrain; ++j)
    {
      if (j == vi)
        data.push_back(1.0f);
      else
        data.push_back(0.0f);
    }
  }

  //data.push_back(x());
  //data.push_back(y());

  data = brain.activate(data);
  /*
  printf("Data: ");
  for (unsigned i = 0; i < data.size(); ++i)
    printf("%f ", data[i]);
  printf("\n");
  */

  int best = 0;
  for (unsigned i = 1; i < data.size(); ++i)
    if (data[i] > data[best])
      best = i;

  return static_cast<Direction>(best);
}

extern "C" Agent* maker(const int vision_size)
{
  return new FnnSaver {vision_size};
}
