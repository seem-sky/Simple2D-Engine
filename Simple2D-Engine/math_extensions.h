#ifndef MATH_H
#define MATH_H

#include <cmath>

template <typename T>
inline T round(const T &num) { return floor(((double)num) + 0.5); }

#endif