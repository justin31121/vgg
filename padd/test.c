#include <stdio.h>

typedef struct{
    int x, y;
}Vec;

void vec_print(Vec *v) {
    printf("(%lld, %lld)\n", v->x, v->y);
}

Vec vec_add(Vec a, Vec b) {
    return (Vec) { a.x+b.x, a.y+b.y};
}

int main(int argc, char **argv) {
    Vec v = { 1, 2};
    Vec q = { -3, 4};

    q = vec_add(v, q);

    vec_print(&q);
    return 0;
}

