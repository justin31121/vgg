#include <stdbool.h>

#include "./sdl_c.h"
#include "./font.h"

#include "./buffer.h"

#define BG_COLOR 0x202020ff
#define FONT_COLOR 0xffffffff

#define PADDING_LEFT 10.0
#define PADDING_TOP 10.0

#define WIDTH 800
#define HEIGHT 600

#define FONT_PATH "./consola.ttf"
#define FONT_SIZE 24

size_t x = 0;
size_t y = 0;

size_t width = WIDTH;
size_t height = HEIGHT;

bool running = true;

bool ctrl = false;
bool alt = false;
bool drag = false;
bool shift = false;

Vec vec_min(Vec a, Vec b) {
  int dy = (int) (a.y - b.y);
  int dx = (int) (a.x - b.x);

  if(dy<0) {
    return a;
  }
  else if(dy>0) {
    return b;
  }

  if(dx<0) {
    return a;
  }
  else {
    return b;
  }
}

Vec vec_max(Vec a, Vec b, Vec c) {
  if(a.x==c.x && a.y==c.y) {
    return a;
  }

  return b;
}

void render(SDL_Renderer *rend, Font *font, Buffer *buffer, const Cursor *cursor) {
  SDL_RenderClear(rend);

  float scale = 1.0;

  Vec origin = vec(x*font->width - PADDING_LEFT, y*font->height - PADDING_TOP);

  //DRAW BUFFER
  buffer_process_lines(buffer);
  for(size_t i=0;i<buffer->lines_size;i++) {
    font_render_text_sized(rend, font, vec(0 - origin.x, i*font->height - origin.y), buffer->buffer + buffer->positions[i], buffer->lines[i], scale, FONT_COLOR);
  }
  
  //DRAW CURSOR
  SDL_Rect rect = {
    cursor->x*font->width * scale - origin.x,
    cursor->y*font->height * scale - origin.y,
    font->width * scale,
    font->height * scale};
  SDL_SetRenderDrawColor(rend, decode(FONT_COLOR));
  SDL_RenderFillRect(rend, &rect);

  //DRAW HIGHLIGHTED TEXT
  /*
  Vec a = vec(cursor->x, cursor->y);
  Vec b = vec(cursor->i, cursor->j);
  
  Vec min = vec_min(a, b);
  Vec max = vec_max(a, b, min);

  size_t m1, m2, n1, n2;
  for(int j=(int) min.y;j<(int) max.y;j++) {    
    //RENDER FROM min.x
    if(j==min.y) {
      //RENDER TO max.x
      if(j==max.y) {
	n1 = 
      }
      else {
	
      }
    }
    //RENDER FROM 0
    else {
      //RENDER TO max.x
      if(j==max.y) {
	
      }
      else {
	
      }
    }
  }
  */

  //DRAW CHAR BEHIND CURSOR
  if(cursor->pos<buffer->buffer_size) {
    font_render_char(rend, font,
		vec(cursor->x*font->width - origin.x, cursor->y*font->height - origin.y),
		buffer->buffer[cursor->pos], scale, BG_COLOR);        
  }

  //RENDER BACKGROUND
  SDL_SetRenderDrawColor(rend, decode(BG_COLOR));
  SDL_RenderPresent(rend);
}

void window_fit(const Buffer *buffer, const Cursor *cursor) {
  if(cursor->y>=y+height) {
    y++;
  }
  else if(cursor->y<y) {
    y--;
  }

  if(cursor->x>=x+width) {
    x = width;
  }
  else if(cursor->x<x) {
    x--;
  }
}

void key_ctrl(size_t keycode, bool on) {
  switch(keycode) {
  case SDLK_RCTRL:
    ctrl = on;
    break;
  case SDLK_LCTRL:
    ctrl = on;
    break;
  case SDLK_LALT:
    alt = on;
    break;
  case SDLK_RALT:
    alt = on;
    break;
  case SDLK_LSHIFT:
    shift = on;
    break;
  case SDLK_RSHIFT:
    shift = on;
  default:
    break;
  }
}

void key_down(Buffer *buffer, Cursor *cursor, const char* file_path, size_t keycode) {
  //printf("Pressed %lld\n", keycode);
  if(ctrl) {
    switch(keycode) {
    case 'f':
      cursor_right(cursor, buffer, 1, shift);
      break;
    case 'b':
      cursor_left(cursor, buffer, 1, shift);
      break;
    case 'n':
      cursor_down(cursor, buffer, 1, shift);
      break;
    case 'p':
      cursor_up(cursor, buffer, 1, shift);
      break;
    case 'a':
      cursor_beg_line(cursor, buffer, shift);
      break;
    case 'e':
      cursor_end_line(cursor, buffer, shift);
      break;
    case 's':
      buffer_save_file(buffer, file_path);
      break;
    case 'y':
      char* content;
      content = SDL_GetClipboardText();
      if(content) {
	buffer_insert(buffer, cursor, content);
      }
      SDL_free(content);
      break;
    default:
      break;
    }
  }

  if(alt) {
    switch(keycode) {
    case 'f':
      cursor_word_right(cursor, buffer);
      break;
    case 'b':
      cursor_word_left(cursor, buffer);
      break;
    }
  }

  switch(keycode) {
  case SDLK_BACKSPACE:
    buffer_delete(buffer, cursor, 1);
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
    buffer_insert_size(buffer, cursor, "\n", 1);
    break;
  default:
    break;
  }

  window_fit(buffer, cursor);
}

static int resizingEventWatcher(void *data, SDL_Event *event) {
  if (event->type == SDL_WINDOWEVENT &&
      event->window.event == SDL_WINDOWEVENT_RESIZED) {
    SDL_Window* win = SDL_GetWindowFromID(event->window.windowID);
    if (win == (SDL_Window*)data) {
    }
  }
  return 0;
}

int main(int argc, char **argv) {
  Cursor cursor = {0};
  Buffer buffer = {0};
  
  if(argc>1) {
    buffer_load_file(&buffer, argv[1]);
  }
        
  scc(SDL_Init(SDL_INIT_VIDEO));
  stcc(TTF_Init());
  
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
			   SDL_RENDERER_ACCELERATED
			   ));

  SDL_AddEventWatch(resizingEventWatcher, wind);

  Font font;
  font_init(&font, rend, FONT_PATH, FONT_SIZE);


  printf("Cursor: %lld\n", sizeof(cursor));
  printf("Buffer: %lld\n", sizeof(buffer));
  printf("Font: %lld\n", sizeof(font));
  
  printf("SUM: %lld\n", sizeof(cursor) + sizeof(buffer) + sizeof(font));

  width = WIDTH / font.width;
  height = HEIGHT / font.height;

  SDL_Event event;
    
  while(running) {
    SDL_WaitEvent(&event);

    switch(event.type) {
    case SDL_QUIT:
      running = false;
      break;
    case SDL_KEYDOWN:
      key_ctrl(event.key.keysym.sym, true);
      key_down(&buffer, &cursor, argv[1], event.key.keysym.sym);
      break;
    case SDL_KEYUP:
      key_ctrl(event.key.keysym.sym, false);
      break;
    case SDL_TEXTINPUT:
      buffer_insert(&buffer, &cursor, event.text.text);
      break;
    case SDL_MOUSEWHEEL:
      if(event.wheel.y > 0) {
	y = y==0 ? 0 : (y-1);
      }
      else if(event.wheel.y < 0) {
	y++;
      }
      break;
    case SDL_MOUSEBUTTONDOWN:
      drag = true;
      if(event.button.button==SDL_BUTTON_LEFT) {
	cursor_click(&cursor, &buffer,
		     event.button.x + x*font.width - PADDING_LEFT,
		     event.button.y + y*font.height - PADDING_TOP,
		     font.width, font.height, false);
      }     
      break;
    case SDL_MOUSEBUTTONUP:
      if(event.button.button==SDL_BUTTON_LEFT) {
	cursor_click(&cursor, &buffer,
		     event.button.x + x*font.width - PADDING_LEFT,
		     event.button.y + y*font.height - PADDING_TOP,
		     font.width, font.height, true);
      }
      drag = false;
      break;
    default:
      break;
    }

    render(rend, &font, &buffer, &cursor);
  }

  SDL_DestroyRenderer(rend);
  SDL_DestroyWindow(wind);

  font_close(&font);

  TTF_Quit();
  SDL_Quit();
  return 0;
}
