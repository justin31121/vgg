#ifndef BUFFER_H
#define BUFFER_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#define BUFFER_CAP 1024
#define LINE_CAP 128

typedef struct{
  char buffer[BUFFER_CAP];
  size_t buffer_size;

  int lines[LINE_CAP];
  int positions[LINE_CAP];
    
  size_t lines_size;
}Buffer;

typedef struct{
  size_t x, y;
  size_t pos;

  size_t i, j;
}Cursor;

void cursor_log(const Cursor *cursor);

void cursor_click(Cursor *cursor,
		  const Buffer *buffer,
		  size_t _x, size_t _y,
		  size_t char_width, size_t char_height,
		  bool sec
		  );

void cursor_right(Cursor *cursor, const Buffer *buffer, size_t size, bool shift);
void cursor_left(Cursor *cursor, const Buffer *buffer, size_t size, bool shift);
void cursor_up(Cursor *cursor, const Buffer *buffer, size_t size, bool shift);
void cursor_down(Cursor *cursor, const Buffer *buffer, size_t size, bool shift);

void cursor_beg_line(Cursor *cursor, const Buffer *buffer, bool shift);
void cursor_end_line(Cursor *cursor, const Buffer *buffer, bool shift);
void cursor_word_right(Cursor *cursor, const Buffer *buffer);
void cursor_word_left(Cursor *cursor, const Buffer *buffer);

void buffer_process_lines(Buffer *buffer);
void buffer_lines_log(const Buffer *buffer);
void buffer_insert_size(Buffer *buffer, Cursor *cursor, const char* text, size_t size);
void buffer_insert(Buffer *buffer, Cursor *cursor, const char* text);
void buffer_delete(Buffer *buffer, Cursor *cursor, size_t size);

void buffer_load_file(Buffer *buffer, const char* file_path);
void buffer_save_file(const Buffer *buffer, const char* file_path);

#endif //BUFFER_H
