#include "pgml.h"

void nodes_dump(Node *node_arr, size_t node_arr_size) {
    printf("Nodes:\n");
    for(size_t i=0;i<node_arr_size;++i) {
        printf("\t%d\n", node_arr[i].id);
    }
    printf("\n");
}

void edges_dump(Edge *edge_arr, size_t edge_arr_size) {
    printf("Edges:\n");
    for(size_t i=0;i<edge_arr_size;++i) {
        printf("\t%d -> %d\n", edge_arr[i].source, edge_arr[i].target);
    }
    printf("\n");
}


bool inRange(int x0, int x1, int x) {
    int bottom = x0 < x1 ? x0 : x1;
    int top = x1 > x0 ? x1 : x0;

    return bottom<=x && x<=top;
}

bool edges_intersect(Edge *edge_arr, size_t edge_arr_size,
	     Node *node_arr, size_t node_arr_size, int i, int j) {
    if(i<0 || i>=edge_arr_size) return false;
    if(j<0 || j>=edge_arr_size) return false;

    Edge one = edge_arr[i];
    Edge two = edge_arr[j];

    int m0, m1, n0, n1;
    //ONE
    //m0
    if(node_arr[one.source].x == node_arr[one.target].x) {
        m0 = 0;
    }
    else {
        m0 =(node_arr[one.source].y - node_arr[one.target].y)
            / (node_arr[one.source].x - node_arr[one.target].x);
    }
    //n0
    n0 = node_arr[one.source].y - m0 * node_arr[one.source].x;

    //TWO
    //m1
    if(node_arr[two.source].x == node_arr[two.target].x) {
        m1 = 0;
    }
    else {
        m1 =(node_arr[two.source].y - node_arr[two.target].y)
            / (node_arr[two.source].x - node_arr[two.target].x);
    }
    //n1
    n1 = node_arr[two.source].y - m1 * node_arr[two.source].x;
    if(m0==m1) {
        return n0==n1;
    }
    else {
        int x = (n1 - n0) / (m1 - m0);
        return inRange(node_arr[one.source].x, node_arr[one.target].x, x)
            && inRange(node_arr[two.source].x, node_arr[two.target].x, x);
    }
 }

void nodes_arrange(Node *node_arr,
	   size_t node_arr_size,
	   Edge *edge_arr,
	   size_t edge_arr_size) {

    for(size_t i=0;i<node_arr_size; i++) {
        node_arr[i].y = rand() % 10 ;
        node_arr[i].x = rand() % 10 ;
    }

    while(true) {
        bool fine = true;
        for(size_t i=0;i<edge_arr_size;i++) {
            for(size_t j=0;j<edge_arr_size;j++) {
	if(i==j) continue;
	if(edges_intersect(edge_arr, edge_arr_size, node_arr, node_arr_size, i, j)) {
	    //Fix Positions
	    node_arr[edge_arr[j].target].y = rand() % 10;
	    node_arr[edge_arr[j].target].x = rand() % 10;
	    fine = false;
	    break;
	}
            }
            if(!fine) {
	break;
            }
        }
        if(fine) {
            break;
        }
    }

    for(size_t i=0;i<node_arr_size;i++) {
        node_arr[i].y = 30 + node_arr[i].y*30*2;
        node_arr[i].x = 30 + node_arr[i].x*30*2;
    }
}

bool node_find_id(Node *node_arr, size_t node_arr_size, int id, int *n) {
    for(size_t i=0;i<node_arr_size;i++) {
        if(node_arr[i].id==id) {
            *n = i;
            return true;
        }
    }
    return false;
}
