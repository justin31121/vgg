#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <stdbool.h>

#define WIDTH 800
#define HEIGHT 600

#define FONT_PATH "./consola.ttf"

#define BUFFER_CAP 1024

char buffer[BUFFER_CAP];
size_t buffer_size = 0;

bool running = true;

int ssc(int code) {
  if(code<0) {
    fprintf(stderr, "SDL_Error: %s\n", SDL_GetError());
    exit(1);
  }  
  return code;
}

int sstc(int code) {
  if(code<0) {
    fprintf(stderr, "TTF_Error: %s\n", TTF_GetError());
    exit(1);
  }  
  return code;
}

void *scc(void* ptr) {
  if(!ptr) {
    fprintf(stderr, "SDL_Error: %s\n", SDL_GetError());
    exit(1);
  }
  return ptr;
}

void *sctc(void *ptr) {
  if(!ptr) {
    fprintf(stderr, "TTF_Error: %s\n", TTF_GetError());
    exit(1);
  }
  return ptr;  
}

typedef struct{
  SDL_Texture *font;
  int width;
  int height;
}Font;

Font font_init(SDL_Renderer *rend, const char* path) {

  const char alphabet[] = "abcdefghijklmnopqrstufwxyz"; 
  
  TTF_Font *font = sctc(TTF_OpenFont(path, 16));
  

  SDL_Color white = {0, 0, 0};

  SDL_Surface *surface =
    sctc(TTF_RenderText_Blended(font, alphabet, white));

  SDL_Texture *texture =
    scc(SDL_CreateTextureFromSurface(rend, surface));

  Font f = { .font = texture, .width = 9, .height=16};

  SDL_FreeSurface(surface);

  TTF_CloseFont(font);

  return f;
}

void font_close(Font font) {
  SDL_DestroyTexture(font.font);
}

void draw(SDL_Renderer *rend, Font font) {
  SDL_RenderClear(rend);

  //DRAW BACKGROUND
  SDL_SetRenderDrawColor(rend, 32, 32, 32, 255);
  SDL_RenderPresent(rend);
}

void key_down(size_t keycode) {
  //printf("Pressed %lld\n", keycode);
  switch(keycode) {
  case SDLK_BACKSPACE:
    if(buffer_size> 0) {
      buffer_size--;
      buffer[buffer_size] = 0;
    }
    break;
  case 'q':
    //q
    running = false;
    break;
  default:
    break;
  }
}

int main(int argc, char **argv) {

  ssc(SDL_Init(SDL_INIT_VIDEO));
  sstc(TTF_Init());
  
  SDL_Window *wind =
    scc(SDL_CreateWindow(
			 "Editor",
			 SDL_WINDOWPOS_CENTERED,
			 SDL_WINDOWPOS_CENTERED,
			 WIDTH, HEIGHT,
			 SDL_WINDOW_SHOWN
			 ));

  SDL_Renderer *rend =
    scc(SDL_CreateRenderer(
			   wind,
			   -1,
			   SDL_RENDERER_SOFTWARE
			   ));

  Font font = font_init(rend, FONT_PATH);

  SDL_Event event;

  while(running) {
    SDL_WaitEvent(&event);

    switch(event.type) {
    case SDL_QUIT:
      running = false;
      break;
    case SDL_KEYDOWN:
      key_down(event.key.keysym.sym);
      break;
    case SDL_TEXTINPUT:
      size_t size = strlen(event.text.text);
      if(BUFFER_CAP - buffer_size > 0) {
	memcpy(buffer + buffer_size, event.text.text, size);
	buffer_size += size;
	buffer[buffer_size] = 0;
      }
      break;
    }

    draw(rend, font);
  }

  SDL_DestroyRenderer(rend);
  SDL_DestroyWindow(wind);

  font_close(font);

  TTF_Quit();
  SDL_Quit();
  return 0;
}
