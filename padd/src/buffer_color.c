#include "./buffer_color.h"

void text_check_ptr(const Text *text) {
  if(!text->lines) {
    fprintf(stderr, "ERROR: Can not allocate enough memory for lines\n");
    exit(1);
  } 
}

void text_check_lines_ptr(const Text *text, size_t start) {
  for(size_t i=start;i<text->lines_cap;i++) {
    Line *line = &text->lines[i];
    if(!line->intervals) {
      fprintf(stderr, "ERROR: Can not allocate enough memory for lines\n");
      exit(1);      
    }    
  }
}

void text_init(Text *text) {
  text->lines = (Line *) malloc(sizeof(Line) * LINES_SIZE);
  text->lines_cap = LINES_SIZE;
  for(size_t i=0;i<text->lines_cap;i++) {
    Line *line = &text->lines[i];
    line->intervals = (Interval *) malloc(sizeof(Interval) * INTERVALS_SIZE);
    line->intervals_cap = INTERVALS_SIZE;
  }

  text_check_ptr(text);
  text_check_lines_ptr(text, 0);
}

void text_quit(Text *text) {
  for(size_t i=0;i<text->lines_cap;i++) {
    free(text->lines[i].intervals);
  }
  free(text->lines);
}

void text_line_alloc(Text *text) {

  text->lines = (Line *) realloc(text->lines, sizeof(Line) * text->lines_cap*2);
  
  text_check_ptr(text);
  
  for(size_t i=0;i<text->lines_cap;i++) {
    Line *line = &text->lines[i+text->lines_cap];
    line->intervals = (Interval *) malloc(sizeof(Interval) * INTERVALS_SIZE);
    line->intervals_cap = INTERVALS_SIZE;
  }

  size_t pos = text->lines_cap;
  text->lines_cap *= 2;
  text_check_lines_ptr(text, pos);
}

void text_intervall_alloc(Text *text, size_t index) {

  text->lines[index].intervals_cap *= 2;
  text->lines[index].intervals = (Interval *) realloc(text->lines[index].intervals,
						      sizeof(Interval) * text->lines[index].intervals_cap);
  
  text_check_lines_ptr(text, 0);
}

#define KEYWORDS_CAP 12
static char *keywords[KEYWORDS_CAP] = {
  "if", "while", "for", "const", "static", "typedef", "struct", "#include", "#define", "return"};
static size_t keywords_len[] = { 2, 5, 3, 5, 6, 7, 6, 8, 7, 6};
static int keywords_colors[] = { 1, 1, 1, 1, 1, 1, 1, 2, 2, 1};
static size_t keywords_count = 10;

static char sentence_open[] = {'\"', '<'};
static char sentence_close[] = {'\"', '>'};
static int sentence_colors[] = {3, 3};
static size_t sentence_count = 2;

bool line_eol(char *buffer, size_t len, int *color, size_t *end) {
  for(size_t i=0;i<len;i++) {
    for(size_t q=0;q<sentence_count;q++) {      
      if(buffer[i]==sentence_open[q] && (i==0 || buffer[i-1]!='\\')) {
	bool open = true;
	size_t p; 
	for(p=i+1;p<len;p++) {
	  if(buffer[p]==sentence_close[q] && buffer[p-1]!='\\') {
	    open = false;
	    p++;
	    break;
	  }
	}
	if(!open) {
	  if(i==0) {
	    if(end) *end=p;
	    if(color) *color=sentence_colors[q];
	    return p-1==len-1;
	  }
	  else {
	    if(end) *end=i;
	    if(color) *color=0;
	    return i==len-1;
	  }
	}
      }
    }
    for(size_t k=0;k<keywords_count;k++) {
      bool candidate = true;
      if(i+keywords_len[k]>len) continue;
      for(size_t j=0;j<keywords_len[k];j++) {
	if(buffer[i+j]!=keywords[k][j]) {
	  candidate = false;
	  break;
	}
      }
      if(candidate) {
	if(i==0) {
	  if(end) *end=keywords_len[k];
	  if(color) *color=keywords_colors[k];
	  return keywords_len[k]==len;
	}
	else {
	  if(end) *end=i;
	  if(color) *color=0;
	  return i==len-1;
	}      
      }
    }
  }

  if(end) *end=len;
  if(color) *color=0;
  return true;
}

bool isText(char *d) {
  char c = d[0];
  return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

void text_update(Text *text, const Buffer *buffer, const Cursor *cursor, size_t y, size_t height) {
  (void) cursor;
  text->lines_count = 0;
  for(size_t i=y;i<(y+height) && i<buffer->lines_size;i++) {
    if(text->lines_count>=text->lines_cap) {
      text_line_alloc(text);
    }
    
    Line *line = &text->lines[text->lines_count];
    line->line = i;
    line->intervals_count = 0;
        
    size_t end=0;
    size_t acc=0;
    int special = 0;
    bool eol;
    while(true) {
      eol = line_eol(buffer->buffer+buffer->positions[i] + acc, buffer->lines[i] - acc, &special, &end);

      if(special) {
	if(acc>0 && isText(buffer->buffer + buffer->positions[i] + (acc-1))) {
	  special = 0;
	}
	if((acc+end-1<buffer->lines[i]) && isText(buffer->buffer + buffer->positions[i] + (acc+end))) {
	  special = 0;
	}
      }
      //ADD INTERVAL
      if(line->intervals_count >= line->intervals_cap) {
	text_intervall_alloc(text, text->lines_count);
      }
      Interval *inter = &line->intervals[line->intervals_count];	   
      inter->start = acc;
      inter->end = end;
      inter->color = special;
      
      line->intervals_count++;
      acc+=end;
          
      if(eol) {
	break;
      }
    }

    //ADD LINE
    text->lines_count++;
  }
}

