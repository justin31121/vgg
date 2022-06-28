#include <stdbool.h>

#include "./sdl_c.h"
#include "./font.h"

#include "./vec.h"
#include "./buffer.h"

#define BG_COLOR 0x202020ff
#define FONT_COLOR 0xffffffff

#define WIDTH 800
#define HEIGHT 600

#define FONT_PATH "./consola.ttf"
#define FONT_SIZE 24

#define ENTER 10
#define TAB_WIDTH 4

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

  Vec origin = vec(x*font->width, y*font->height);

  //DRAW BUFFER
  buffer_process_lines(buffer);
  for(size_t i=y;i<y+height;i++) {
    font_render_text_sized(rend, font, vec(0 - origin.x, i*font->height - origin.y),
			   buffer->buffer + buffer->positions[i], buffer->lines[i],
			   scale, FONT_COLOR);
  }

  //DRAW HIGHLIHGTED TEXT
  

  //DRAW CURSOR
  SDL_Rect rect = { cursor->x*font->width - origin.x, cursor->y*font->height - origin.y, font->width, font->height};
  SDL_SetRenderDrawColor(rend, decode(FONT_COLOR));
  SDL_RenderFillRect(rend, &rect);

  //DRAW CHAR BEHIND CURSOR
  if(cursor->pos<buffer->buffer_size) {
    font_render_char(rend, font,
		     vec(cursor->x*font->width - origin.x,
			 cursor->y*font->height - origin.y),
		     buffer->buffer[cursor->pos], 1.0, BG_COLOR);        
  }

  //RENDER BACKGROUND
  SDL_SetRenderDrawColor(rend, decode(BG_COLOR));
  SDL_RenderPresent(rend);
}

void window_fit(const Buffer *buffer, const Cursor *cursor) {
  (void) buffer;

  if(cursor->y>=y+height) {
    y++;
  }
  else if(cursor->y<y) {
    y--;
  }

  if(cursor->x>=width+x) {
    x++;
  }
  else if(cursor->x<x) {
    x--;
  }
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
  default:
    break;
  }
}

void key_down(Buffer *buffer, Cursor *cursor, size_t keycode, const char* file_path) {
  //printf("Pressed %lld\n", keycode);
  if(ctrl) {
    switch(keycode) {
    case 'y':
      char* content;
      content = SDL_GetClipboardText();
      buffer_insert(buffer, cursor, content);
      SDL_free(content);
      break;
    case 'w':
      if(cursor->x==cursor->i && cursor->y==cursor->j) {
	printf("no distance\n");
      }
      else {	
	printf("(%lld, %lld) -> (%lld, %lld)\n", cursor->i, cursor->j, cursor->x, cursor->y);
      }
      break;
    case 'k':
      size_t size = buffer->lines[cursor->y];
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
      cursor_right(cursor, buffer, 1, false);
      buffer_delete(buffer, cursor, 1);
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
  case SDLK_TAB:
    break;
  default:
    break;
  }

  window_fit(buffer, cursor);
}

int main(int argc, char **argv) {

  Cursor cursor = {0};
  Buffer buffer = {0};
        
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
			 SDL_WINDOW_SHOWN
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
    
  while(running) {
    SDL_WaitEvent(&event);

    switch(event.type) {
    case SDL_QUIT:
      running = false;
      break;
    case SDL_KEYDOWN:
      key_ctrl(event.key.keysym.sym, true);
      key_down(&buffer, &cursor, event.key.keysym.sym, argv[1]);
      break;
    case SDL_KEYUP:
      key_ctrl(event.key.keysym.sym, false);      
      break;
    case SDL_TEXTINPUT:
      buffer_insert(&buffer, &cursor, event.text.text);
      break;
    case SDL_MOUSEBUTTONDOWN:
      drag = true;
      if(event.button.button==SDL_BUTTON_LEFT) {
	cursor_click(&cursor, &buffer, event.button.x, event.button.y, font.width, font.height, false);
      }
      break;
    case SDL_MOUSEBUTTONUP:
      if(event.button.button==SDL_BUTTON_LEFT) {
	cursor_click(&cursor, &buffer, event.button.x, event.button.y, font.width, font.height, true);
      }
      drag = false;
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
