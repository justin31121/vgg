#include "./buffer.h"

void buffer_process_lines(Buffer *buffer) {
  size_t low=0;
  size_t up=0;
  size_t line = 0;
  buffer->lines_size = 0;

  for(;up<buffer->buffer_size;up++) {
    if(buffer->buffer[up]==10) {
      line++;
      if(buffer->lines_size>=LINE_CAP) {
	fprintf(stderr, "ERROR: Line overflow\n");
	exit(1);
      }
      buffer->positions[buffer->lines_size] = low;
      buffer->lines[buffer->lines_size++] = up - low;
      low = up+1;
    }
  }
  buffer->positions[buffer->lines_size] = low;
  buffer->lines[buffer->lines_size++] = buffer->buffer_size - low;
}

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

void cursor_click(Cursor *cursor, const Buffer *buffer, size_t _x, size_t _y, size_t char_width, size_t char_height, bool shift) {
  size_t x = _x / char_width;
  size_t y = _y / char_height;

  if(y>=buffer->lines_size) {
    return;
  }

  if(shift) {
    cursor->i = cursor->x;
    cursor->j = cursor->y;
    
    cursor->y = y;
    cursor->x = buffer->lines[cursor->y] < x ? buffer->lines[cursor->y] : x;
  }
  else {
    cursor->y = y;
    cursor->x = buffer->lines[cursor->y] < x ? buffer->lines[cursor->y] : x;
      
  }
  cursor->pos = buffer->positions[y] + cursor->x;
}

void cursor_right(Cursor *cursor, const Buffer *buffer, size_t size, bool shift) {
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
  if(!shift) {
    cursor->i = cursor->x;
    cursor->j = cursor->y;
  }
  cursor->pos+=i;
}

void cursor_left(Cursor *cursor, const Buffer *buffer, size_t size, bool shift) {   
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
  if(!shift) {
    cursor->i = cursor->x;
    cursor->j = cursor->y;
  } 
  cursor->pos-=i;
}

void cursor_up(Cursor *cursor, const Buffer *buffer, size_t size, bool shift) {
  for(size_t i=0;(int) (cursor->y-i)>0 && i<size;i++) {
    cursor->y--;
    cursor->x = buffer->lines[cursor->y] < cursor->x ? buffer->lines[cursor->y] : cursor->x;
    cursor->pos = buffer->positions[cursor->y] + cursor->x;
  }
  if(!shift) {
    cursor->i = cursor->x;
    cursor->j = cursor->y;
  }  
}

void cursor_down(Cursor *cursor, const Buffer *buffer, size_t size, bool shift) {    
  for(size_t i=0;(cursor->y+i+1)<buffer->lines_size && i<size;i++) {
    cursor->y++;
    cursor->x = buffer->lines[cursor->y] < cursor->x ? buffer->lines[cursor->y] : cursor->x;
    cursor->pos = buffer->positions[cursor->y] + cursor->x;
  }
  if(!shift) {
    cursor->i = cursor->x;
    cursor->j = cursor->y;
  }
}

void buffer_insert_size(Buffer *buffer, Cursor *cursor, const char* text, size_t size) {
  if(BUFFER_CAP - buffer->buffer_size > 0) {

    size_t pos = cursor->pos;

    memcpy(buffer->buffer + pos + size, buffer->buffer + pos, buffer->buffer_size - pos + size);
    memcpy(buffer->buffer + pos, text, size);         

    buffer->buffer_size += size;
    cursor_right(cursor, buffer, size, false);
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
  cursor_left(cursor, buffer, size, false);
        
  memcpy(buffer->buffer + pos - size, buffer->buffer + pos, buffer->buffer_size - pos);

  buffer->buffer_size-=size;
}

char* slurp_file(const char* file_path, size_t *size) {
  FILE *f=fopen(file_path, "rb");
  if(!f) {
    fprintf(stderr, "ERROR: Can not open file '%s' :%s", file_path, strerror(errno));
    exit(1);
  }
  if(fseek(f, 0,SEEK_END)<0) {
    fprintf(stderr, "ERROR: Can not read file '%s' :%s", file_path, strerror(errno));
    exit(1);
  }	
  long m = ftell(f);
  if(m<0) {
    fprintf(stderr, "ERROR: Can not read file '%s' :%s", file_path, strerror(errno));
    exit(1);
  }
  char* buffer = malloc(sizeof(char)*m);
  if(!buffer) {
    fprintf(stderr, "ERROR: Can not allocate memory\n");
    exit(1);
  }
	
  if(fseek(f, 0, SEEK_SET)<0) {
    fprintf(stderr, "ERROR: Can not read file '%s' :%s", file_path, strerror(errno));
    exit(1);
  }

  size_t n = fread(buffer, 1, m, f);
  buffer[m]='\0';
  assert((int) n==m);

  if(f) {
    if(fclose(f)!=0) {
      fprintf(stderr, "ERROR: Can when closing the file '%s' :%s", file_path, strerror(errno));
      exit(1);
    }
  }
  if(size) (*size)=n;
  return buffer;
}

void buffer_load_file(Buffer *buffer, const char* file_path) {
  size_t size;
  char* content = slurp_file(file_path, &size);

  for(size_t i=0;i<size;i++) {
    if(buffer->buffer_size>=BUFFER_CAP) {
      fprintf(stderr, "ERROR: Buffer overflow\n");
      exit(1);
    }
    
    if(content[i]=='\t') {
      buffer->buffer[buffer->buffer_size++] = ' ';
    }
    else if(content[i]!='\r') {
      buffer->buffer[buffer->buffer_size++] = content[i];
    }
  }

  free(content);
}

void buffer_save_file(const Buffer* buffer, const char* file_path) {
  FILE *f = fopen(file_path, "wb");
  if(!f) {
    fprintf(stderr, "ERROR: Can not open file '%s' :%s", file_path, strerror(errno));
    exit(1);
  }

  fwrite(buffer->buffer, 1, buffer->buffer_size, f);
	
  fclose(f);

  printf("Saved to %s\n", file_path);
}

void cursor_end_line(Cursor *cursor, const Buffer *buffer, bool shift) {
  size_t dist = buffer->lines[cursor->y] - cursor->x;
  cursor_right(cursor, buffer, dist, shift);
}

void cursor_beg_line(Cursor *cursor, const Buffer *buffer, bool shift) {
  cursor_left(cursor, buffer, cursor->x, shift);
}

void cursor_word_right(Cursor *cursor, const Buffer *buffer) {
  printf("WORD_RIGHT\n");
}

void cursor_word_left(Cursor *cursor, const Buffer *buffer) {
  printf("WORD_LEFT\n");  
}
