#ifndef PGML_PARSE_H
#define PGML_PARSE_H

#include ".\pgml\pgml.h"

void usage(char *msg);
bool is_gml_file(char* filepath);

bool content_find_flag(char* content, size_t content_size,
	       char* flagname, size_t flagname_size,
	       size_t *bottom_result,
	       size_t *top_result);

bool content_find_block(char* content,
	        size_t content_size,
	        char* blockname,
	        size_t blockname_size,
	        size_t *addr,
	        size_t *bottom_result,
	        size_t *top_result);

bool content_parse_node(char* content,
	        size_t content_size,
	        Node *target);

bool content_parse_edge(char* content, size_t content_size,
	        Node *node_arr, size_t node_arr_size,
	        Edge *target);

bool content_parse(char* filename,
	   Node *node_arr, size_t *node_arr_size, size_t node_arr_cap,
	   Edge *edge_arr, size_t *edge_arr_size, size_t edge_arr_cap);

#endif //PGML_PARSE_H
