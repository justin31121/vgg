#include <stdio.h>
#include "flag.h"

#include ".\pgml\pgml.h"
#include ".\pgml\pgml_parse.h"

#include ".\gpng\gpng.h"

#define WIDTH 800
#define HEIGHT 800

#define RADIUS 20

#define NODE_CAP 1024
#define EDGE_CAP 1024

//=====================

Node nodes[NODE_CAP];
size_t nodes_size;

Edge edges[EDGE_CAP];
size_t edges_size;

void map_draw_arrow(Map *map, Line line) {
    int m, n;
    if(line.b.x == line.a.x) {
        m = 0;
    }
    else {
        m = (line.b.y - line.a.y) / (line.b.x - line.a.x);
    }

    n = line.b.y - m * line.b.x;
    
    //
    int x = line.b.x;
    int dx = line.b.x > line.a.x ? -1 : 1;
    int y = m*x + n;

    while(true) {
        int tx = line.b.x - x;
        int ty = line.b.y - y;
        if(tx*tx+ty*ty>RADIUS*RADIUS) break;

        x+= dx;
        y = m*x + n;
    }
    
    map_draw_circle(map, circle(point(x, y), 20, color(255, 0, 0)));
}
 
int main(int argc, char **argv) {
    char **filename = flag_string("t", "0", "Path to the target file\n");
    flag_parse(argc, argv);

    //Right Format ? 
    bool is_right_format = is_gml_file(*filename);
    if(!is_right_format) {
        usage("ERROR: Please provide a gml file\n");
    }

    //Read Nodes
    bool success = content_parse(*filename,
		 nodes, &nodes_size, NODE_CAP,
		 edges, &edges_size, EDGE_CAP);
    if(!success) {
        usage("ERROR: Provided gml file is not in the right format\n");
    }

    //nodes_dump(nodes, nodes_size);
    //edges_dump(edges, edges_size);

    nodes_arrange(nodes, nodes_size, edges, edges_size);
    
    //=======================================================================

    //Init
    Map map = {0};
    map_allocate(&map, WIDTH, HEIGHT);
    printf("Initialized Map succesfully\n");

    //PROGRAM

    //Lines
    Line line = line(point(0, 0), point(0, 0), color(0, 0, 0));
    Node temp;
    for(size_t i=0;i<nodes_size;i++) {
        temp = nodes[edges[i].source];
        line.a = point(temp.x, temp.y);

        temp = nodes[edges[i].target];
        line.b = point(temp.x, temp.y);

        map_draw_line(&map, line);
        map_draw_arrow(&map, line);
    }
    
    //Circles
    Circle circle = circle(point(0, 0), RADIUS, color(204, 204, 9));
    for(size_t i=0;i<nodes_size;i++) {
        circle.a = point(nodes[i].x, nodes[i].y);
        map_draw_circle(&map, circle);
    }

    //Write
    map_to_png(map, "gpng.png");
    printf("Written gpng.png\n");
    
    //Free
    map_free(map);
    printf("Freed Map");
}
