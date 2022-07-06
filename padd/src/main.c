#include <stdbool.h>

#include "./sdl_c.h"
#include "./font.h"

#include "./vec.h"
#include "./buffer.h"

#define BG_COLOR 0x202020ff
#define FONT_COLOR 0xffffffff
#define FONT_DARKER_COLOR 0xaaaaaaff
#define HG_COLOR 0x606060ff

#define WIDTH 800
#define HEIGHT 600

#define PAD_LEFT 25
#define PAD_TOP 5

#define FONT_PATH "./rsc/consola.ttf"
#define FONT_SIZE 24

#define ENTER 10
#define TAB_WIDTH 4

size_t font_size = FONT_SIZE;

size_t height;
size_t width;
size_t x = 0;
size_t y = 0;

bool running = true;

bool ctrl = false;
bool shift = false;
bool alt = false;
bool drag = false;

void render(SDL_Renderer *rend, Font *font, Buffer *buffer, const Cursor *cursor) {
  SDL_RenderClear(rend);

  float scale = 1.0;

  buffer_process_lines(buffer);
  
  float offset = 3;
  size_t f = buffer->lines_size;
  while((f=f/10)>0) offset++;

  Vec origin = vec((x*font->width) - offset * font->width, y*font->height - (float) PAD_TOP);
  
  //DRAW HIGHLIHGTED TEXT
  Vec pos = vec(cursor->x, cursor->y);
  Vec old = vec(cursor->i, cursor->j);

  if(!vec_eq(pos, old)) {
    Vec min = vec_min(pos, old);
    Vec max = vec_max(pos, old);

    for(int i=min.y;i<=max.y;i++) {
      float x1;
      if(i==min.y) {
 x1 = min.x;
      }
      else {
 x1 = 0;
      }

      float x2 = width;
      if(i==max.y) {
 x2 = max.x - x1;
      }

      SDL_Rect temp = {x1*font->width - origin.x, i*font->height - origin.y, x2*font->width, font->height};
      SDL_SetRenderDrawColor(rend, decode(HG_COLOR));
      SDL_RenderFillRect(rend, &temp);
    }
  }

  //DRAW BUFFER
  int k = 1;
  for(size_t i=y;i<buffer->lines_size;i++) {
    //DRAW TEXT
    font_render_text_sized(rend, font, vec(0 - origin.x, i*font->height*scale - origin.y),
      buffer->buffer + buffer->positions[i], buffer->lines[i],
      scale, FONT_COLOR);
    //DRAW LINE
    font_render_char(rend, font, vec(0 - origin.x - 2*font->width,i*font->height*scale - origin.y),
      k + '0',
      scale , FONT_DARKER_COLOR);
    k++;
  }
  
  //DRAW CURSOR
  SDL_Rect rect = { cursor->x*font->width*scale - origin.x, cursor->y*font->height*scale - origin.y, font->width*scale, font->height*scale};
  SDL_SetRenderDrawColor(rend, decode(FONT_COLOR));
  SDL_RenderFillRect(rend, &rect);

  //DRAW CHAR BEHIND CURSOR
  if(cursor->pos<buffer->buffer_size) {
    font_render_char(rend, font,
       vec(cursor->x*font->width*scale - origin.x,
    cursor->y*font->height*scale - origin.y),
       buffer->buffer[cursor->pos], scale, BG_COLOR);        
  }

  //RENDER BACKGROUND
  SDL_SetRenderDrawColor(rend, decode(BG_COLOR));
  SDL_RenderPresent(rend);
}

//void render(SDL_Renderer *rend, Font *font, Buffer *buffer, const Cursor *cursor) {
typedef struct{
  SDL_Window *wind;
  SDL_Renderer *rend;
  Font *font;
  Buffer *buffer;
  Cursor *cursor;
}PassingStruct;

static int resizingEventWatcher(void* data, SDL_Event* event) {
  if (event->type == SDL_WINDOWEVENT &&
      event->window.event == SDL_WINDOWEVENT_RESIZED) {
    SDL_Window* win = SDL_GetWindowFromID(event->window.windowID);
    PassingStruct* p = (PassingStruct*)data;
    if (win == p->wind) {
      render(p->rend, p->font, p->buffer, p->cursor);

      int w, h;
      SDL_GetWindowSize(p->wind, &w, &h);
      
      width = w / p->font->width;
      height = h / p->font->height;
    }
  }
  return 0;
}

void window_fit(const Buffer *buffer, const Cursor *cursor) {
  (void) buffer;


  while(cursor->y>=y+height) y++;
  while(cursor->y<y) y--;

  while(cursor->x>=x+width-2) x++;
  while(cursor->x<x) x--;
}

void key_ctrl(size_t keycode, bool on) {
  switch(keycode) {
  case SDLK_LCTRL:
    ctrl = on;
    break;
  case SDLK_RCTRL:
    ctrl = on;
    break;
  case SDLK_LSHIFT:
    shift = on;
    break;
  case SDLK_RSHIFT:
    shift = on;
    break;
  case SDLK_LALT:
    alt = on;
    break;
  case SDLK_RALT:
    alt = on;
    break;
  default:
    break;
  }
}

void key_down(Buffer *buffer, Cursor *cursor, Font *font, SDL_Renderer *rend, size_t keycode, const char* file_path) {
  //printf("Pressed %lld\n", keycode);
  size_t size, start, end;
  char* content;
  if(ctrl) {
    switch(keycode) {
    case'-':
      font_close(font);
      font_size -= 2;
      font_init(font, rend, FONT_PATH, font_size);
      height = HEIGHT / font->height;
      width = WIDTH / font->width;      
      break;
    case'+':
      font_close(font);
      font_size += 2;
      font_init(font, rend, FONT_PATH, font_size);
      height = HEIGHT / font->height;
      width = WIDTH / font->width;
      break;
    case 'g':
      cursor_reset(cursor);
      break;
    case 'y':
      content = SDL_GetClipboardText();
      buffer_insert(buffer, cursor, content);
      SDL_free(content);
      break;
    case 'w':
      size = cursor_dist(cursor, buffer, &start, &end);
      if(size!=0) {
 //COPY TO CLIPBOARD
 char text[size+1];
 memcpy(text, buffer->buffer + start, size);
 text[size] = 0;
 
 if(SDL_SetClipboardText(text)!=0) {
   printf("Cant copy text\n");   
 }
 cursor_outer(cursor, buffer);
 buffer_delete(buffer, cursor, size);
      }
      break;
    case 'k':
      size = buffer->lines[cursor->y];
      cursor_right(cursor, buffer, size, false);
      buffer_delete(buffer, cursor, size);
      break;
    case 's':
      buffer_save_file(buffer, file_path);
      break;
    case 'e':
      cursor_end_line(cursor, buffer, shift);
      break;
    case 'a':
      cursor_beg_line(cursor, buffer, shift);
      break;
    case 'd':
      size = cursor_dist(cursor, buffer, NULL, NULL);
      if(size==0) {
 cursor_right(cursor, buffer, 1, false);
 buffer_delete(buffer, cursor, 1);
      }
      else {
 cursor_outer(cursor, buffer);
 buffer_delete(buffer, cursor, size);
      }
      
      break;      
    case 'f':
      cursor_right(cursor, buffer, 1, shift);
      break;
    case 'b':
      cursor_left(cursor, buffer, 1, shift);
      break;
    case 'p':
      cursor_up(cursor, buffer, 1, shift);
      break;
    case 'n':
      cursor_down(cursor, buffer, 1, shift);
      break;
    default:
      break;
    }
  }

  if(alt) {
    switch(keycode) {
    case 'w':
      size = cursor_dist(cursor, buffer, &start, &end);
      if(size!=0) {
 //COPY TO CLIPBOARD
 char text[size+1];
 memcpy(text, buffer->buffer + start, end);
 text[size] = 0;
 
 if(SDL_SetClipboardText(text)!=0) {
   printf("Cant copy text\n");   
 }
      }
      break;
    default:
      break;
    }
  }

  switch(keycode) {
  case SDLK_BACKSPACE:
    size = cursor_dist(cursor, buffer, NULL, NULL);
    if(size==0) {
      buffer_delete(buffer, cursor, 1);
    }
    else {
      cursor_outer(cursor, buffer);      
      buffer_delete(buffer, cursor, size);
    }
    break;
  case SDLK_ESCAPE:
    running = false;
    break;
  case SDLK_UP:
    cursor_up(cursor, buffer, 1, shift);
    break;
  case SDLK_DOWN:
    cursor_down(cursor, buffer, 1, shift);
    break;
  case SDLK_RIGHT:
    cursor_right(cursor, buffer, 1, shift);
    break;
  case SDLK_LEFT:
    cursor_left(cursor, buffer, 1, shift);
    break;
  case SDLK_RETURN:
    size = cursor_dist(cursor, buffer, NULL, NULL);
    if(size!=0) {
      cursor_outer(cursor, buffer);
      buffer_delete(buffer, cursor, size);
    }
    buffer_insert_size(buffer, cursor, "\n", 1);
    break;
  case SDLK_TAB:
    buffer_insert_size(buffer, cursor, "    ", 4);
    break;
  default:
    break;
  }

  window_fit(buffer, cursor);
}

int main(int argc, char **argv) {

  Cursor cursor = {0};
  Buffer buffer = {0};

  buffer_init(&buffer);
        
  scc(SDL_Init(SDL_INIT_VIDEO));
  stcc(TTF_Init());

  if(argc>1) {
    buffer_load_file(&buffer, argv[1]);
  }
  
  SDL_Window *wind =
    scp(SDL_CreateWindow(
    "Editor",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    WIDTH, HEIGHT,
    SDL_WINDOW_RESIZABLE
    ));

  SDL_Renderer *rend =
    scp(SDL_CreateRenderer(
      wind,
      -1,
      SDL_RENDERER_SOFTWARE
      ));
  Font font;
  font_init(&font, rend, FONT_PATH, FONT_SIZE);

  width = WIDTH / font.width;
  height = HEIGHT / font.height;

  SDL_Event event;
  size_t d;

  PassingStruct s = {wind, rend, &font, &buffer, &cursor};
  SDL_AddEventWatch(resizingEventWatcher, &s);
  
  while(running) {
    SDL_WaitEvent(&event);

    switch(event.type) {
    case SDL_QUIT:
      running = false;
      break;
    case SDL_KEYDOWN:
      key_ctrl(event.key.keysym.sym, true);
      key_down(&buffer, &cursor, &font, rend, event.key.keysym.sym, argv[1]);
      break;
    case SDL_KEYUP:
      key_ctrl(event.key.keysym.sym, false);      
      break;
    case SDL_TEXTINPUT:
      d = cursor_dist(&cursor, &buffer, NULL, NULL);
      if(d!=0) {
 cursor_outer(&cursor, &buffer);
 buffer_delete(&buffer, &cursor, d);
      }
      buffer_insert(&buffer, &cursor, event.text.text);
      window_fit(&buffer, &cursor);
      break;
    case SDL_MOUSEWHEEL:
      if(event.wheel.y>0) {
 y = y > 0 ? (y-1) : 0;
      }
      else if(event.wheel.y<0){
 y++;
      }
      break;
    case SDL_MOUSEMOTION:
      break;
    case SDL_MOUSEBUTTONDOWN:
      drag = true;
      cursor_reset(&cursor);
      if(event.button.button==SDL_BUTTON_LEFT) {
 cursor_click(&cursor, &buffer,
       event.button.x + x*font.width - 2.0*font.width,
       event.button.y + y*font.height - PAD_TOP,
       font.width, font.height, false);
      }
      break;
    case SDL_MOUSEBUTTONUP:
      if(event.button.button==SDL_BUTTON_LEFT) {
 cursor_click(&cursor, &buffer,
       event.button.x + x*font.width - 2.0*font.width,
       event.button.y + y*font.height - PAD_TOP,
       font.width, font.height, true);

      }
      drag = false;
      break;
    }

    render(rend, &font, &buffer, &cursor);
  }

  SDL_DestroyRenderer(rend);
  SDL_DestroyWindow(wind);

  font_close(&font);

  buffer_quit(&buffer);

  TTF_Quit();
  SDL_Quit();
  return 0;
}
