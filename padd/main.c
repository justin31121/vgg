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

bool running = true;

float render_char(SDL_Renderer *rend, const Font *font, Vec pos, char c, float scale,
	 Uint32 color) {
    
    if(c<ASCII_LOW) {
        return 0.0;
    }
    
    int i = c - ASCII_LOW;
    
    SDL_Rect dst = {
        pos.x, pos.y,
        font->width * scale,
        font->height * scale};

    SDL_SetTextureColorMod(font->texture, decode_wo_a(color));
    SDL_SetTextureAlphaMod(font->texture, decode_a(color));

    SDL_RenderCopy(rend, font->texture, &font->glyphs[i], &dst);
    
    return dst.w;
}

void render_text(SDL_Renderer *rend, const Font *font, Vec pos, const char* cs, float scale,
	 Uint32 color) {
    size_t len = strlen(cs);

    for(size_t i=0;i<len;i++) {
        float off = render_char(rend, font, pos, cs[i], scale, color);
        pos.x += off;
    }
}

void render_text_sized(SDL_Renderer *rend, const Font *font, Vec pos, const char* cs, size_t size, float scale,
	       Uint32 color) {
    for(size_t i=0;i<size;i++) {
        float off = render_char(rend, font, pos, cs[i], scale, color);
        pos.x += off;
    }
}

void render(SDL_Renderer *rend, Font *font, Buffer *buffer, const Cursor *cursor) {
    SDL_RenderClear(rend);

    //DRAW BUFFER
    size_t low=0;
    size_t up=0;
    size_t line = 0;
    buffer->lines_size = 0;

    for(;up<buffer->buffer_size;up++) {
        if(buffer->buffer[up]==ENTER) {
            render_text_sized(rend, font, vec(0, line*font->height), buffer->buffer + low, up - low, 1.0,
	              FONT_COLOR);
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
    render_text_sized(rend, font, vec(0, line*font->height), buffer->buffer + low, buffer->buffer_size - low, 1.0,
	              FONT_COLOR);
    buffer->positions[buffer->lines_size] = low;
    buffer->lines[buffer->lines_size++] = buffer->buffer_size - low;

    //DRAW CURSOR
    SDL_Rect rect = { cursor->x*font->width, cursor->y*font->height, font->width, font->height};
    SDL_SetRenderDrawColor(rend, decode(FONT_COLOR));
    SDL_RenderFillRect(rend, &rect);

    //DRAW CHAR BEHIND CURSOR
    if(cursor->pos<buffer->buffer_size) {
        render_char(rend, font,
	    vec(cursor->x*font->width,  cursor->y*font->height),
	    buffer->buffer[cursor->pos], 1.0, BG_COLOR);        
    }

    //RENDER BACKGROUND
    SDL_SetRenderDrawColor(rend, decode(BG_COLOR));
    SDL_RenderPresent(rend);
}

void key_down(Buffer *buffer, Cursor *cursor, size_t keycode) {
    //printf("Pressed %lld\n", keycode);
    switch(keycode) {
    case SDLK_BACKSPACE:
        buffer_delete(buffer, cursor, 1);
        break;
    case SDLK_ESCAPE:
        running = false;
        break;
    case SDLK_UP:
        cursor_up(cursor, buffer, 1);
        break;
    case SDLK_DOWN:
        cursor_down(cursor, buffer, 1);
        break;
    case SDLK_RIGHT:
        cursor_right(cursor, buffer, 1);
        break;
    case SDLK_LEFT:
        cursor_left(cursor, buffer, 1);
        break;
    case SDLK_RETURN:
        buffer_insert_size(buffer, cursor, "\n", 1);
        break;
    case SDLK_TAB:
        buffer_insert_size(buffer, cursor, "        ", 8);
        break;
    default:
        break;
    }
}

int main(int argc, char **argv) {

    Cursor cursor = {0};
    Buffer buffer = {0};

    (void) argc;
    (void) argv;
        
    scc(SDL_Init(SDL_INIT_VIDEO));
    stcc(TTF_Init());
  
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

    SDL_Event event;
    
    while(running) {
        SDL_WaitEvent(&event);

        switch(event.type) {
        case SDL_QUIT:
            running = false;
            break;
        case SDL_KEYDOWN:
            key_down(&buffer, &cursor, event.key.keysym.sym);
            break;
        case SDL_TEXTINPUT:
            buffer_insert(&buffer, &cursor, event.text.text);
            break;
        case SDL_MOUSEBUTTONDOWN:
            if(event.button.button==SDL_BUTTON_LEFT) {
	cursor_click(&cursor, &buffer, event.button.x, event.button.y, font.width, font.height);
            } 
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
