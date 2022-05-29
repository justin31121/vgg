#include "gpng.h"

void point_swap(Point *a, Point *b) {
    Point temp = point(a->x, a->y);

    a->x = b->x;
    a->y = b->y;

    b->x = temp.x;
    b->y = temp.y;
}

int point_cmp(Point a, Point b) {
    int cx = a.x - b.x;
    int cy = a.y - b.y;

    if(cx) return cx;
    else return cy;
}

void map_set_pixel(Map *map, int x, int y, Color c) {
     png_bytep px = &(map->row_pointers[y])[x * 4];

     px[0] = c.r;
     px[1] = c.g;
     px[2] = c.b;
     px[3] = c.a;
}

void map_allocate(Map *map, int w, int h) {
    map->height = h;
    map->width = w;

    map->row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * h);
    if(!map->row_pointers) {
        fprintf(stderr, "ERROR: Not enough memory, can not alloc Map\n");
        exit(1);
    }

    for(int y=0;y<h;y++) {
        map->row_pointers[y] = (png_byte*) malloc(sizeof(png_byte) * w * 4);
        if(!map->row_pointers[y]) {
            fprintf(stderr, "ERROR: Not enougch memory, can not alloc Map\n");
            exit(1);
        }
    }

    for(int y = 0; y < map->height; y++) {
        for(int x = 0; x < map->width; x++) {
            map_set_pixel(map, x, y, BACKGROUND); 
        }
    }
}

void map_free(Map map) {
    for(int y=0;y<map.height;y++) {
        if(map.row_pointers[y]) free(map.row_pointers[y]);
    }
    if(map.row_pointers) free(map.row_pointers);
}

void map_to_png(Map map, char* filename) {

    //OPEN FILE
    FILE *fp = fopen(filename, "wb");
    
    if(!fp) {
        printf("am i here\n");
        fprintf(stderr, "ERROR: Can not open file '%s': %s\n",
	filename, strerror(errno));
        exit(1);
    }

    //CREATE PNG ? 

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!png) {
        fprintf(stderr, "ERROR: Can not create structp\n");
        exit(1);
    }

    png_infop info = png_create_info_struct(png);
    if(!info) {
        fprintf(stderr, "ERROR: Can not create png_infop\n");
        exit(1);
    }

    //SET EVERYTHING
    
    png_init_io(png, fp);
    
    png_set_IHDR(
	 png,
	 info,
	 map.width, map.height,
	 8,
	 PNG_COLOR_TYPE_RGBA,
	 PNG_INTERLACE_NONE,
	 PNG_COMPRESSION_TYPE_DEFAULT,
	 PNG_FILTER_TYPE_DEFAULT
	 );
    png_write_info(png, info);
    
    //WRITE
    png_write_image(png, map.row_pointers);
    png_write_end(png, NULL);

    //CLOSE FILE
    fclose(fp);

    png_destroy_read_struct(&png, &info, NULL);
    png=NULL;
    info=NULL;
}

void map_draw_line(Map *map, Line line) {

    int diff = line.b.x - line.a.x;
    int dx = diff >= 0 ? diff : (-1*diff);
    int sx = line.a.x < line.b.x ? 1 : -1;

    diff = line.b.y - line.a.y;
    int dy = diff >= 0 ? diff : (-1*diff);
    int sy = line.a.y < line.b.y ? 1 : -1;

    int err = (dx>dy ? dx : -dy)/2;
    int e2;

    int x = line.a.x;
    int y = line.a.y;

    for(;;) {
        map_set_pixel(map, x, y, line.color);
        if(x==line.b.x && y==line.b.y) break;
        e2 = err;
        if(e2 > -dx) {
            err -= dy;
            x+=sx;
        }
        if(e2 < dy) {
            err += dx;
            y+=sy;
        }
    }
}

void map_draw_circle(Map *map, Circle circle) {
    int botX = circle.a.x - circle.radius;
    int botY = circle.a.y - circle.radius;

    int topX = circle.a.x + circle.radius;
    int topY = circle.a.y + circle.radius;

    Color black = color(0, 0, 0);
    for(int x=botX;x<topX;x++) {
        for(int y=botY;y<topY;y++) {
            int dx=circle.a.x - x;
            int dy=circle.a.y - y ;

            if(!(0<=x && x<map->width && 0<=y && y<map->height)) continue;

            int diff = dx*dx+dy*dy;
            
            int r1 = (circle.radius-1)*(circle.radius-1);
            int r2 = circle.radius*circle.radius;
            
            if(diff<=r1) {
	map_set_pixel(map, x, y, circle.color);
            }
            else if(circle.border && diff<=r2) {
	map_set_pixel(map, x, y, black);
            }
        }
    }
}

void map_draw_rect(Map *map, Rect rect) {

    if(point_cmp(rect.b, rect.a)<0) {
        point_swap(&rect.a, &rect.b);
    }
    
    for(int y = rect.a.y;y<(map->height) && y<rect.b.y;y++) {
        for(int x=rect.a.x ;x<(map->width) && x<rect.b.x;x++) {
            map_set_pixel(map, x, y, rect.color);
        }
    }
    
}