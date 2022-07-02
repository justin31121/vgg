#include "./buffer.h"

void buffer_lines_log(const Buffer *buffer) {
    printf("LINES: (%lld)\n", buffer->lines_size);
    for(size_t i=0;i<buffer->lines_size;i++) {
        printf("\t%lld\n", buffer->lines[i]);
    }
    printf("\n");
}

void cursor_log(const Cursor *cursor) {
    printf("(%lld, %lld), pos: %lld\n", cursor->x, cursor->y, cursor->pos);
}

void cursor_click(Cursor *cursor, const Buffer *buffer, size_t _x, size_t _y, size_t char_width, size_t char_height) {
    size_t x = _x / char_width;
    size_t y = _y / char_height;

    if(y>=buffer->lines_size) {
        return;
    }

    cursor->y = y;
    cursor->x = buffer->lines[cursor->y] < x ? buffer->lines[cursor->y] : x;
    cursor->pos = buffer->positions[y] + cursor->x;
}

void cursor_right(Cursor *cursor, const Buffer *buffer, size_t size) {
    size_t i;
    for(i=0;(cursor->pos+i)<buffer->buffer_size && i<size;i++) {
        if(buffer->buffer[cursor->pos+i]==10) {
            cursor->x = 0;
            cursor->y++;
        }
        else {
            cursor->x++; 
        }
    }
    cursor->pos+=i;
}

void cursor_left(Cursor *cursor, const Buffer *buffer, size_t size) {
    size_t i=0;
    for(i=0;(int) (cursor->pos-i)>0 && i<size;i++) {
        if(cursor->x==0) {
            cursor->y--;
            cursor->x = buffer->lines[cursor->y];
        }
        else {
            cursor->x--;
        }
    }
    cursor->pos-=i;
}

void cursor_up(Cursor *cursor, const Buffer *buffer, size_t size) {
    for(size_t i=0;(int) (cursor->y-i)>0 && i<size;i++) {
        cursor->y--;
        cursor->x = buffer->lines[cursor->y] < cursor->x ? buffer->lines[cursor->y] : cursor->x;
        cursor->pos = buffer->positions[cursor->y] + cursor->x;
    }
}

void cursor_down(Cursor *cursor, const Buffer *buffer, size_t size) {
    for(size_t i=0;(cursor->y+i+1)<buffer->lines_size && i<size;i++) {
        cursor->y++;
        cursor->x = buffer->lines[cursor->y] < cursor->x ? buffer->lines[cursor->y] : cursor->x;
        cursor->pos = buffer->positions[cursor->y] + cursor->x;
    }
}

void buffer_insert_size(Buffer *buffer, Cursor *cursor, const char* text, size_t size) {
    if(BUFFER_CAP - buffer->buffer_size > 0) {

        size_t pos = cursor->pos;

        memcpy(buffer->buffer + pos + size, buffer->buffer + pos, buffer->buffer_size - pos + size);
        memcpy(buffer->buffer + pos, text, size);         

        buffer->buffer_size += size;
        cursor_right(cursor, buffer, size);
    }
    else {
        fprintf(stderr, "BUFFER OVERFLOW\n");
        exit(1);
    }
}

void buffer_insert(Buffer *buffer, Cursor *cursor, const char* text) {
    size_t size = strlen(text);
    buffer_insert_size(buffer, cursor, text, size);
}

void buffer_delete(Buffer *buffer, Cursor *cursor, size_t size) {
    if(cursor->pos<=size) {
        size = cursor->pos;
    }

    size_t pos = cursor->pos;
    cursor_left(cursor, buffer, size);
        
    memcpy(buffer->buffer + pos - size, buffer->buffer + pos, buffer->buffer_size - pos);

    buffer->buffer_size-=size;
}
