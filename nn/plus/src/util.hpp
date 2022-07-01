#ifndef UTIL_H
#define UTIL_H

float randf(float min, float max) {
  return min + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(max-min)));
}

#endif //UTIL_H
