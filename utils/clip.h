#ifndef UTILS_CLAMP_H
#define UTILS_CLAMP_H

#include <algorithm>

template<typename T>
T clip(const T& n, const T& lower, const T& upper)
{
  return std::max(lower, std::min(n, upper));
}

#endif //UTILS_CLAMP_H

