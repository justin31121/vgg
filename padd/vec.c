#include "./vec.h"

Vec vec(float a, float b) {
  return (Vec) {a, b};
}

Vec vec_s(float a) {
  return (Vec) {a, a};
}

Vec vec_add(Vec a, Vec b) {
  return (Vec) {a.x+b.x, a.y+b.y};
}

Vec vec_sub(Vec a, Vec b) {
  return (Vec) {a.x-b.x, a.y-b.y};
}

Vec vec_mul(Vec a, Vec b) {
  return (Vec) {a.x*b.x, a.y*b.y};
}

Vec vec_div(Vec a, Vec b) {
  return (Vec) {a.x/b.x, a.y/b.y};
}

Vec vec_scale(Vec a, float b) {
  return (Vec) {a.x*b, a.y*b};
}

Vec vec_off(Vec a, float b) {
  return (Vec) {a.x+b, a.y+b};
}
