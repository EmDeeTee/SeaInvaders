#ifndef RENDERER_H
#define RENDERER_H

#define WINDOW_W 800
#define WINDOW_H 600
#define ALIEN_IMAGE_PATH "alien.bmp"

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

void renderer_init(void);
SDL_Texture* load_image(const char* path);
void render_rect(SDL_Rect* rc, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
SDL_Renderer* get_renderer(void);
#endif
