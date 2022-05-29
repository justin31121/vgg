#ifndef GPNG_H
#define GPNG_H

#include <png.h>
#include <stdlib.h>
#include <string.h>

#define color_a(r1,g1,b1,a1) (Color) {.r = r1, .g = g1, .b = b1, .a = a1}
#define color(r1,g1,b1) (Color) {.r = r1, .g = g1, .b = b1, .a = 255}

#define point(x1,y1) (Point) {.x = x1, .y=y1}

#define line(a1, b1, c) (Line) {.a = a1, .b = b1, .color = c}
#define rect(a1,b1,c) (Rect) {.a = a1, .b = b1, .color = c}
#define circle(a1, r1, c) (Circle) {.a = a1, .radius = r1, .border=0, .color=c}
#define circle_b(a1, r1, c, b1) (Circle) {.a = a1, .radius = r1, .border=b1, .color=c}

#ifndef BACKGROUND
#define BACKGROUND color(255, 255, 255)
#endif

typedef struct{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
}Color;

typedef struct{
    int x;
    int y;
}Point;

void point_swap(Point *a, Point *b);
int point_cmp(Point a, Point b);

typedef struct{
    Point a;
    Point b;
    Color color;
}Line;

typedef struct{
    Point a;
    Point b;
    Color color;
}Rect;

typedef struct{
    Point a;
    int radius;
    int border;
    Color color;
}Circle;

typedef struct {
    int width;
    int height;
    png_bytep *row_pointers;
}Map;


void map_to_png(Map map, char* filename);
void map_allocate(Map *map, int w, int h);
void map_free(Map map);

void map_set_pixel(Map *map, int x, int y, Color c);

void map_draw_line(Map *map, Line line);
void map_draw_circle(Map *map, Circle circle);
void map_draw_rect(Map *map, Rect rect);


#endif //GPNG_H