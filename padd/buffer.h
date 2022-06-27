#ifndef BUFFER_H
#define BUFFER_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define BUFFER_CAP 1024
#define LINE_CAP 128

typedef struct{
    char buffer[BUFFER_CAP];
    size_t buffer_size;

    size_t lines[LINE_CAP];
    size_t positions[LINE_CAP];
    
    size_t lines_size;
}Buffer;

typedef struct{
    size_t x, y;
    size_t pos;
}Cursor;

void cursor_log(const Cursor *cursor);

void cursor_click(Cursor *cursor,
	 const Buffer *buffer,
	 size_t _x, size_t _y,
	 size_t char_width, size_t char_height);

void cursor_right(Cursor *cursor, const Buffer *buffer, size_t size);

void cursor_left(Cursor *cursor, const Buffer *buffer, size_t size);

void cursor_up(Cursor *cursor, const Buffer *buffer, size_t size);

void cursor_down(Cursor *cursor, const Buffer *buffer, size_t size);

void buffer_lines_log(const Buffer *buffer);
void buffer_insert_size(Buffer *buffer, Cursor *cursor, const char* text, size_t size);
void buffer_insert(Buffer *buffer, Cursor *cursor, const char* text);
void buffer_delete(Buffer *buffer, Cursor *cursor, size_t size);

#endif //BUFFER_H
