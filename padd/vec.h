#ifndef VEC_H
#define VEC_H

typedef struct{
  float x, y;
}Vec;

Vec vec(float a, float b);
Vec vec_s(float a);
Vec vec_add(Vec a, Vec b);
Vec vec_sub(Vec a, Vec b);
Vec vec_mul(Vec a, Vec b);
Vec vec_div(Vec a, Vec b);
Vec vec_scale(Vec a, float b);
Vec vec_off(Vec a, float b);

#endif //VEC_H
