// guiprada@gmail.com  Guilherme Cunha Prada 2017-2020 : )
// zlib license

#pragma once

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
//////////////////////////////////////////////////////////////////////// structs

typedef struct window_struct neat_window;
struct window_struct {
	SDL_Window* window;
	SDL_Renderer* renderer;
	bool					hidden;
};

typedef struct sprite_struct neat_sprite;
struct sprite_struct {
	SDL_Texture* texture;
	SDL_Rect			source_rect;
	SDL_Rect			pos_rect;
	SDL_Point			anchor;
	double				rotation;
	SDL_RendererFlip	flip;
	neat_window* window;
};

typedef struct neat_texture_struct	neat_texture;
struct neat_texture_struct {
	SDL_Texture*	texture;
	neat_window*	window;
};
////////////////////////////////////////////////////////////////////// functions

int					neat_start();
// returns 0 on sucess, diferent errors are coded so look it up

void				neat_stop();
// return 0 on sucess

neat_window* neat_window_create(const char*, int, int, int, int);
// NULL on error, you have to free_context() it!

void				neat_window_destroy(neat_window*);
void                neat_sprite_render(neat_sprite*);
void				neat_sprite_init(neat_sprite*, neat_texture*);
neat_sprite*		neat_sprite_create(neat_texture*);
void				neat_sprite_destroy(neat_sprite*);
void				neat_sprite_move(neat_sprite*, double, double);
void				neat_sprite_move_to(neat_sprite*, double, double);
void				neat_sprite_set_source_rect(neat_sprite*, int, int, int, int);

SDL_Rect			neat_assign_rect(int, int, int, int);
SDL_Point			neat_assign_point(int, int);

neat_texture*		neat_texture_create(neat_window*, const char*);
neat_texture*		neat_texture_create_from_text(
	neat_window*,
	TTF_Font*,
	SDL_Color,
	const char*
);
void				neat_texture_destroy(neat_texture*);

////////////////////////////////////////////////////////////////////////////////

const SDL_Color RED = { 255,   0,   0 };
const SDL_Color GREEN = { 0, 255,   0 };
const SDL_Color BLUE = { 0,   0, 255 };

const SDL_Color YELLOW = { 255, 255,   0 };
const SDL_Color VIOLET = { 255,   0, 255 };
const SDL_Color CYAN = { 0, 255, 255 };

const SDL_Color WHITE = { 255, 255, 255 };
const SDL_Color BLACK = { 0,   0,   0 };
////////////////////////////////////////////////////////////////////////////////

int neat_start() {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0) {
		printf("SDL_Init: %s\n", SDL_GetError());
		return -1;
	}

	int img_flags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF;
	int img_initted = IMG_Init(img_flags);
	if ((img_initted & img_flags) != img_flags) {
		printf("IMG_Init: %s\n", IMG_GetError());
		return -2;
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
		printf("Mix_OpenAudio: %s\n", Mix_GetError());
		return -3;
	}

	int mix_flags = MIX_INIT_OGG | MIX_INIT_MOD | MIX_INIT_FLAC | MIX_INIT_MP3;	
	int mix_initted = Mix_Init(mix_flags);
	if ((mix_initted & mix_flags) != mix_flags) {
		printf("Mix_Init: %s\n", Mix_GetError());
		return -4;
	}

	if (TTF_Init() != 0) {
		return -5;
	}
	return 0;
}

void neat_stop() {
	TTF_Quit();
	Mix_CloseAudio();
	IMG_Quit();
	SDL_Quit();
}

neat_window* neat_window_create(
	const char* title,
	int xo,
	int yo,
	int width,
	int height
) {
	neat_window* w = (neat_window*)malloc(sizeof(neat_window));

	w->window = SDL_CreateWindow(
		title,
		xo, yo, width, height, SDL_WINDOW_SHOWN);
	if (w->window == NULL) {
		printf("Error creating SDL2 Window.\n");
		return NULL;
	}
	w->renderer = SDL_CreateRenderer(w->window, -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	//SDL_RENDERER_SOFTWARE);
	if (w->renderer == NULL) {
		printf("Error creating SDL2 Renderer.\n");
		SDL_DestroyWindow(w->window);
		free(w);
		return NULL;
	}

	w->hidden = false;

	return w;
}

void neat_window_destroy(neat_window* c) {
	SDL_DestroyRenderer(c->renderer);
	SDL_DestroyWindow(c->window);
	free(c);
}

void neat_sprite_render(neat_sprite* object) {
	if (object->texture == NULL) {
		SDL_RenderFillRect(object->window->renderer, &(object->pos_rect));
	}
	else {
		SDL_RenderCopyEx(
			object->window->renderer,
			object->texture,
			&(object->source_rect),
			&(object->pos_rect),
			object->rotation,
			&(object->anchor),
			object->flip
		);
	}
}

void neat_sprite_init(neat_sprite* object, neat_texture* tex) {
	object->texture = tex->texture;
	object->window = tex->window;
	int w, h;
	SDL_QueryTexture(object->texture, NULL, NULL, &w, &h);
	object->source_rect = neat_assign_rect(0, 0, w, h);
	object->pos_rect = neat_assign_rect(0, 0, w, h);
	object->anchor = neat_assign_point(w / 2, h / 2);
	object->rotation = 0;
	object->flip = SDL_FLIP_NONE;
}

neat_sprite* neat_sprite_create(neat_texture* tex) {
	neat_sprite* sprite = (neat_sprite*)malloc(sizeof(neat_sprite));
	neat_sprite_init(sprite, tex);
	return sprite;
}

void neat_sprite_move(neat_sprite* sprite, double x, double y) {
	sprite->pos_rect.x += x;
	sprite->pos_rect.y += y;
	sprite->anchor.x += x;
	sprite->anchor.y += y;
}

void neat_sprite_move_to(neat_sprite* sprite, double x, double y) {
	sprite->pos_rect.x = x;
	sprite->pos_rect.y = y;
	sprite->anchor.x = x + sprite->pos_rect.w / 2;
	sprite->anchor.y = y + sprite->pos_rect.h / 2;
}

void neat_sprite_set_source_rect(neat_sprite* sprite, int x, int y, int w, int h) {
	sprite->source_rect.x = x;
	sprite->source_rect.y = y;
	sprite->source_rect.w = w;
	sprite->source_rect.h = h;
	sprite->pos_rect.w = w;
	sprite->pos_rect.h = h;
	sprite->anchor.x = sprite->pos_rect.x + w / 2;
	sprite->anchor.y = sprite->pos_rect.y + h / 2;
}


void neat_sprite_destroy(neat_sprite* sprite) {
	free(sprite);
}

SDL_Rect neat_assign_rect(int x, int y, int w, int h) {
	SDL_Rect r;
	r.x = x;
	r.y = y;
	r.w = w;
	r.h = h;

	return r;
}

SDL_Point neat_assign_point(int x, int y) {
	SDL_Point p;
	p.x = x;
	p.y = y;

	return p;
}

neat_texture* neat_texture_create(neat_window* window, const char* file) {
	SDL_Surface* loaded_surface = IMG_Load(file);
	if (loaded_surface == NULL) {
		return NULL;
	}
	else {
		neat_texture* new_texture = (neat_texture*)malloc(sizeof(neat_texture));
		new_texture->texture = SDL_CreateTextureFromSurface(window->renderer,
			loaded_surface);
		new_texture->window = window;
		if (new_texture->texture == NULL) {
			printf("ERROR creating texture!\n");
			free(new_texture);
		}
		SDL_FreeSurface(loaded_surface);
		return new_texture;
	}
	//SDL_FreeSurface(loaded_surface);
}

void neat_texture_destroy(neat_texture* t) {
	if (t != NULL) {
		if (t->texture != NULL) {
			SDL_DestroyTexture(t->texture);
			t->texture = NULL;
		}
		free(t);
	}
	return;
}

neat_texture* neat_texture_create_from_text(
	neat_window* window,
	TTF_Font* font,
	SDL_Color color,
	const char* text
) {
	SDL_Surface* text_surface = TTF_RenderText_Solid(font, text, color);
	if (text_surface == NULL) {
		printf("SDL_ttf error: %s    line: %i    file:  %s\n",
			TTF_GetError(), __LINE__, __FILE__);
		return NULL;
	}
	else {
		neat_texture* text_texture = (neat_texture*)malloc(sizeof(neat_texture));
		text_texture->texture = SDL_CreateTextureFromSurface(window->renderer,
			text_surface);
		if (text_texture->texture == NULL) {
			printf("SDL error: %s    line: %i    file:  %s\n",
				SDL_GetError(), __LINE__, __FILE__);
			free(text_texture);
			return NULL;
		}
		text_texture->window = window;
		SDL_FreeSurface(text_surface);
		return text_texture;
	}
}