#include "Renderer.h"

SDL_Renderer* renderer = NULL;
SDL_Window* wnd = NULL;

void renderer_init(void) {
	wnd = SDL_CreateWindow("SpaceInvaders", 200, 200, WINDOW_W, WINDOW_H, 0);
	renderer = SDL_CreateRenderer(wnd, -1, SDL_RENDERER_ACCELERATED);

	printf("[INFO] Renderer initialised\n");
}

SDL_Texture* load_image(const char* path) {
	SDL_Texture* img = IMG_LoadTexture(renderer, path);
	if (img == NULL) {
		printf("[ERROR] Can't load %s\n", path);
		exit(1);
	}

	return img;
}

void render_rect(SDL_Rect* rc, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
	if (rc == NULL)
		return;
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	SDL_RenderFillRect(renderer, rc);
}

SDL_Renderer* get_renderer(void) {
	return renderer;
}
