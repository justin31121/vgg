#include "./font.h"

void font_init(Font *f, SDL_Renderer *rend, const char* path, size_t font_size) {
    const char alphabet[] = " !\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
  
    TTF_Font *font = stcp(TTF_OpenFont(path, font_size));

    SDL_Color white = {decode(0xffffffff)};

    SDL_Surface *surface =
        scp(TTF_RenderText_Blended(font, alphabet, white));

    SDL_Texture *texture =
        scp(SDL_CreateTextureFromSurface(rend, surface));

    int w, h;
    TTF_SizeText(font, alphabet, &w, &h);
    size_t len = strlen(alphabet);

    f->texture = texture;
    f->width = (int) (w/len);
    f->height = font_size;

    for(size_t i=0;i<ASCII_LEN;i++) {
        f->glyphs[i] = (SDL_Rect) {
            i*f->width,
            0,
            f->width,
            f->height
        };
    }

    SDL_FreeSurface(surface);
}

void font_close(Font *font) {
    SDL_DestroyTexture(font->texture);
}
