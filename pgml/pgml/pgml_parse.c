#include "pgml_parse.h"
#include <js.h>

void usage(char *msg) {
    fprintf(stderr, msg);
    fprintf(stderr, "USAGE: pgml <.gml-file>\n");
    exit(1);
}

bool is_gml_file(char* filepath) {
    size_t len = strlen(filepath);
    if(len<5) {
        return false;
    }

    char extension[5];
    for(size_t i=0;i<4;i++) {
        extension[i] = filepath[(len-4)+i];
    }
    extension[4]=0;

    if(strcmp(".gml", extension)) {
        return false;
    }

    return true;
}

bool content_find_flag(char* content, size_t content_size,
	       char* flagname, size_t flagname_size,
	       size_t *bottom_result,
	       size_t *top_result) {
    for(size_t i=0;i<content_size-flagname_size;i++) {

        //Find flag
        bool candidate = true;
        for(size_t j=0;j<flagname_size;j++) {
            if(content[i+j]!=flagname[j]) {
	candidate = false;
	break;
            }
        }
        if(candidate) {
            //Find id
            size_t t = i+3;
            size_t top, bottom;

            //Find |
            //     v
            //     12345
            while(t<content_size && !(('0' <= content[t] && content[t] <= '9')
		      || ('a' <=content[t] && content[t] <= 'Z') || content[t]=='\"')) {
	t++;
            }
            if(t+1==t || t>=content_size) {
	return false;
            }
            bottom=t;

            //Find      |
            //          v
            //     12345
            while(t<content_size && (('0' <= content[t] && content[t] <= '9')
		     || ('a' <=content[t] && content[t] <= 'Z') || content[t]=='\"')) {
	t++;
            }
            if(t>=content_size) {
	return false;
            }
            top = t;

            *top_result = top;
            *bottom_result = bottom;
            return true;
        }
    }
    return false;
}

bool content_find_block(char* content,
	        size_t content_size,
	        char* blockname,
	        size_t blockname_size,
	        size_t *addr,
	        size_t *bottom_result,
	        size_t *top_result) {
    size_t i = *addr;
    for(;i<content_size-blockname_size;i++) {

        bool candidate = true;
        for(size_t j=0;j<blockname_size;j++) {
            if(content[i+j]!=blockname[j]) {
	candidate = false;
	break;
            }
        }
        if(candidate) {
            size_t t = i+4;
            size_t bottom;
            size_t top;
            //Find [
            while(t<content_size && content[t]!='[') {
	t++;
            }
            if(t>=content_size) {
	return false;
            }
            t++;
            bottom = t;

            //Find ]
            while(t<content_size && content[t]!=']') {
	t++;
            }
            if(t>=content_size) {
	return false;
            }
            top = t;

            *top_result = top;
            *bottom_result  = bottom;
            *addr = t;
            return true;
        }
    }
    *addr = content_size;
    return false;
}

bool content_parse_node(char* content, size_t content_size,
	        Node *target) {
    
    //Id
    size_t top, bottom;
    bool parsed_id = content_find_flag(content, content_size, "id", 2, &bottom, &top);

    if(!parsed_id) {
        return false;
    }

    string temp = string_from_parts(content+bottom, (top-bottom));
    int n;
    if(!string_to_int(temp, &n)) {
        return false;
    }

    target->id = n;
    
    return true;
}

bool content_parse_edge(char* content, size_t content_size,
	        Node *node_arr, size_t node_arr_size,
	        Edge *target) {
    //Source
    size_t top, bottom;
    bool parsed_source = content_find_flag(content, content_size, "source", 6, &bottom, &top);

    if(!parsed_source) {
        return false;
    }

    string temp = string_from_parts(content+bottom, (top-bottom));
    int n;
    if(!string_to_int(temp, &n)) {
        return false;
    }

    int id;
    if(!node_find_id(node_arr, node_arr_size, n, &id)) {
        return false;
    }
    target->source = id;

    //Target
    bool parsed_target = content_find_flag(content, content_size, "target", 6, &bottom, &top);

    if(!parsed_target) {
        return false;
    }

    temp = string_from_parts(content+bottom, (top-bottom));
    if(!string_to_int(temp, &n)) {
        return false;
    }

    if(!node_find_id(node_arr, node_arr_size,n, &id)) {
        return false;
    }
    target->target = id;

    return true;
}

bool content_parse(char* filename,
	   Node *node_arr, size_t *node_arr_size, size_t node_arr_cap,
	   Edge *edge_arr, size_t *edge_arr_size, size_t edge_arr_cap) {
    size_t size;
    char* content = slurp_file(filename, &size);
    *node_arr_size = 0;
    *edge_arr_size = 0;
    
    //Nodes
    size_t top, bottom;
    size_t address = 0;
    
    bool found_node = content_find_block(content, size, "node", 4, &address, &bottom, &top);
    while(found_node) {
        if(!content_parse_node(content+bottom, (top-bottom), &node_arr[*node_arr_size])) {
            return false;
        }

        if((*node_arr_size)==node_arr_cap) {
            return false;
        }
        (*node_arr_size)++;
        found_node = content_find_block(content, size, "node", 4, &address, &bottom, &top);
    }

    address = 0;
    bool found_edge = content_find_block(content, size, "edge", 4, &address, &bottom, &top);
    while(found_edge) {

        if(!content_parse_edge(content+bottom, (top-bottom), 
	               node_arr, (*node_arr_size),
	               &edge_arr[*edge_arr_size])) {
            return false;
        }

        if((*edge_arr_size)==edge_arr_cap) {
            return false;
        }
        (*edge_arr_size)++;

        found_edge = content_find_block(content, size, "edge", 4, &address, &bottom, &top);
    }
    
    free(content);

    return true;
}
