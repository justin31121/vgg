#ifndef PGML_H
#define PGML_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct{
    int id;
    int x;
    int y;
}Node;

typedef struct{
    int source;
    int target;
}Edge;

void nodes_dump(Node *node_arr, size_t node_arr_size);
void edges_dump(Edge *edge_arr, size_t edge_arr_size);

void nodes_arrange(Node *node_arr, size_t node_arr_size, Edge *edge_arr, size_t edge_arr_size);

bool node_find_id(Node *node_arr, size_t node_arr_size, int id, int *n);
bool edges_intersect(Edge *edge_arr, size_t edge_arr_size,
	     Node *node_arr, size_t node_arr_size, int i, int j);

#endif //PGML_H
