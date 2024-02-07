#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>

#include "Renderer.h"
#include "Engine.h"

#define TICK_INTERVAL 30
#define SHIP_W 50
#define PROJECTILE_W 10
#define ALIEN_CAP 20
#define ALIEN_PADING 56
#define ALIEN_TEX_SIZE 48
#define ALIEN_SPEED 4

SDL_Texture* img_alien = NULL;

typedef struct Alien {
	SDL_Rect rect;
	bool isDestroyed;
} Alien;

Alien aliens[ALIEN_CAP];

static Uint32 next_time = 0;
static SDL_Rect proj = {0, WINDOW_H, PROJECTILE_W,0};
static SDL_Rect ship = {
	.x = WINDOW_W / 2 - SHIP_W / 2,
	.y = WINDOW_H - 100, .w = SHIP_W, .h = 25
};

Uint32 time_left(void) {
	Uint32 now = SDL_GetTicks();
	
	if (next_time <= now)
		return 0;
	else
		return next_time - now;
}

void spawn_projectile(void) {
	if (proj.y < 0)
		proj = (SDL_Rect){ (ship.x + SHIP_W / 2) - (PROJECTILE_W/2), ship.y, PROJECTILE_W, 25};
}

void poll_events() {
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		switch (event.type)
		{
		case SDL_QUIT: {
			SDL_Quit();
			exit(0);
		} break;
		case SDL_KEYDOWN: {
			// SPACE
			if (event.key.keysym.sym == 32) {
				spawn_projectile();
			}
		} break;
		default: break;
		}
	}
}

void render_aliens(void) {
	for (size_t i = 0; i < ALIEN_CAP; i++) {
		Alien* a = &aliens[i];
		if (!a->isDestroyed)
			SDL_RenderCopy(get_renderer(), img_alien, NULL, &aliens[i].rect);
	}
}

void move_aliens(void) {
	for (size_t i = 0; i < ALIEN_CAP; i++) {
		Alien* a = &aliens[i];
		if (a->isDestroyed)
			continue;
		a->rect.x += ALIEN_SPEED;
		if (a->rect.x + ALIEN_TEX_SIZE >= WINDOW_W) {
			a->rect.x = 0;
			a->rect.y += ALIEN_PADING;
		}
	}
}

void shoot_aliens(void) {
	for (size_t i = 0; i < ALIEN_CAP; i++) {
		Alien* a = &aliens[i];
		if (SDL_HasIntersection(&a->rect, &proj) && !a->isDestroyed) {
			proj.y = -1000;
			a->isDestroyed = true;
		}
	}
}

void update(void) {
	const Uint8* kbd = SDL_GetKeyboardState(NULL);
	poll_events();
	SDL_PumpEvents();

	if (kbd[SDL_SCANCODE_A]) {
		ship.x -= 10;
	}
	if (kbd[SDL_SCANCODE_D]) {
		ship.x += 10;
	}

	if (ship.x >= WINDOW_W - SHIP_W) {
		ship.x = WINDOW_W - SHIP_W;
	}
	else if (ship.x <= 0) {
		ship.x = 0;
	}
	proj.y -= 40;
	if (proj.y < 0)
		proj.y = -100;
	render_rect(&proj, 255, 0, 0, 255);
	render_rect(&ship, 255, 0, 0, 255);
	render_aliens();
	move_aliens();
	shoot_aliens();

	SDL_SetRenderDrawColor(get_renderer(), 20, 30, 100, 255);
	SDL_RenderPresent(get_renderer());
	SDL_RenderClear(get_renderer());

	SDL_Delay(time_left());
	next_time += TICK_INTERVAL;
}

void init_aliens(void) {
	int x = 0;
	int y = 0;
	for (size_t i = 0; i < ALIEN_CAP; i++) {
		aliens[i].rect = (SDL_Rect){x,y,ALIEN_TEX_SIZE,ALIEN_TEX_SIZE };
		x += ALIEN_PADING;
		if (x + 255 >= WINDOW_W) {
			x = 0;
			y+= ALIEN_PADING;
		}

	}
}

int main(int argc, char** argv) {
	SDL_Init(SDL_INIT_EVERYTHING);
	renderer_init();
	init_aliens();
	img_alien = load_image(ALIEN_IMAGE_PATH);

	next_time = SDL_GetTicks() + TICK_INTERVAL;
	while (true) {
		update();  
	}
}
