#include "neat.h"

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

	while (Mix_Init(0))
		Mix_Quit();
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

void neat_sprite_render(neat_sprite* sprite) {
	SDL_RenderCopyEx(
		sprite->window->renderer,
		sprite->texture,
		&(sprite->source_rect),
		&(sprite->pos_rect),
		sprite->rotation,
		&sprite->anchor_point,
		sprite->flip
	);
}

void neat_sprite_init(neat_sprite* sprite, neat_texture* tex) {
	sprite->texture = tex->texture;
	sprite->window = tex->window;
	int w, h;
	SDL_QueryTexture(sprite->texture, NULL, NULL, &w, &h);
	sprite->source_rect = neat_create_SDL_rect(0, 0, w, h);
	sprite->pos_rect = neat_create_SDL_rect(0, 0, w, h);
	sprite->pos = neat_create_vec2(0, 0);
	sprite->anchor = neat_create_vec2(w / 2, h / 2);
	sprite->anchor_set = false;
	sprite->rotation = 0;
	sprite->scale = neat_create_vec2(1, 1);
	sprite->flip = SDL_FLIP_NONE;
}

neat_sprite* neat_sprite_create(neat_texture* tex) {
	neat_sprite* sprite = (neat_sprite*)malloc(sizeof(neat_sprite));
	neat_sprite_init(sprite, tex);
	return sprite;
}

void neat_sprite_set_anchor(neat_sprite* sprite, double x, double y) {
	sprite->anchor_set = true;
	sprite->anchor.x = x;
	sprite->anchor.y = y;
	sprite->anchor_point.x = (int)x;
	sprite->anchor_point.y = (int)y;
}

void neat_sprite_set_rotation(neat_sprite* sprite, double rotation) {
	sprite->rotation = rotation;
}

void neat_sprite_set_scale(neat_sprite* sprite, double sx, double sy) {
	sprite->scale.x = sx;
	sprite->scale.y = sy;

	sprite->pos_rect.w = (int)(sprite->source_rect.w * sprite->scale.x);
	sprite->pos_rect.h = (int)(sprite->source_rect.h * sprite->scale.y);

	if (sprite->anchor_set) {
		sprite->anchor.x = sprite->anchor.x * sx;
		sprite->anchor.y = sprite->anchor.y * sy;
		sprite->anchor_point.x = (int)sprite->anchor.x;
		sprite->anchor_point.y = (int)sprite->anchor.y;
	}
	else {
		sprite->anchor.x = sprite->pos_rect.w / 2;
		sprite->anchor.y = sprite->pos_rect.h / 2;
	}
}

void neat_sprite_move(neat_sprite* sprite, double x, double y) {
	sprite->pos.x += x;
	sprite->pos.y += y;
	sprite->pos_rect.x = (int)sprite->pos.x;
	sprite->pos_rect.y = (int)sprite->pos.y;
}

void neat_sprite_move_to(neat_sprite* sprite, double x, double y) {
	sprite->pos.x = x;
	sprite->pos.y = y;
	sprite->pos_rect.x = (int)sprite->pos.x;
	sprite->pos_rect.y = (int)sprite->pos.y;
}

void neat_sprite_set_source_rect(neat_sprite* sprite, int x, int y, int w, int h) {
	sprite->source_rect.x = x;
	sprite->source_rect.y = y;

	if ((w != sprite->source_rect.w) || (h != sprite->source_rect.h)) {
		double sx = w / sprite->source_rect.w;
		double sy = h / sprite->source_rect.h;

		sprite->source_rect.w = w;
		sprite->source_rect.h = h;
		sprite->pos_rect.w = (int)(sprite->source_rect.w * sprite->scale.x);
		sprite->pos_rect.h = (int)(sprite->source_rect.h * sprite->scale.y);

		if (sprite->anchor_set) {
			sprite->anchor.x = sprite->anchor.x * sprite->scale.x * sx;
			sprite->anchor.y = sprite->anchor.y * sprite->scale.y * sy;
			sprite->anchor_point.x = (int)sprite->anchor.x;
			sprite->anchor_point.y = (int)sprite->anchor.y;
		}
		else {
			sprite->anchor.x = sprite->pos_rect.w / 2;
			sprite->anchor.y = sprite->pos_rect.h / 2;
		}
	}
}

void neat_sprite_set_source_rect_and_anchor(neat_sprite* sprite, int x, int y, int w, int h, int cx, int cy) {
	sprite->source_rect.x = x;
	sprite->source_rect.y = y;

	if ((w != sprite->source_rect.w) || (h != sprite->source_rect.h)) {
		sprite->source_rect.w = w;
		sprite->source_rect.h = h;
		sprite->pos_rect.w = (int)(sprite->source_rect.w * sprite->scale.x);
		sprite->pos_rect.h = (int)(sprite->source_rect.h * sprite->scale.y);

		sprite->anchor.x = cx;
		sprite->anchor.y = cy;
		sprite->anchor_point.x = (int)sprite->anchor.x;
		sprite->anchor_point.y = (int)sprite->anchor.y;
		if (!sprite->anchor_set) {
			sprite->anchor_set = true;
		}
	}
}

void neat_sprite_destroy(neat_sprite* sprite) {
	free(sprite);
}

SDL_Rect neat_create_SDL_rect(int x, int y, int w, int h) {
	SDL_Rect r;
	r.x = x;
	r.y = y;
	r.w = w;
	r.h = h;

	return r;
}

SDL_Point neat_create_SDL_Point(int x, int y) {
	SDL_Point p;
	p.x = x;
	p.y = y;

	return p;
}

SDL_Point neat_create_SDL_Point_from_neat_vec2(neat_vec2* v) {
	SDL_Point p;
	p.x = (int)v->x;
	p.y = (int)v->y;

	return p;
}

neat_vec2 neat_create_vec2(double x, double y) {
	neat_vec2 p;
	p.x = x;
	p.y = y;

	return p;
}

neat_texture* neat_create_texture(neat_window* window, const char* file) {
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
}

void neat_destroy_texture_destroy(neat_texture* t) {
	if (t != NULL) {
		if (t->texture != NULL) {
			SDL_DestroyTexture(t->texture);
			//t->texture = NULL;
		}
		free(t);
		//t = NULL;
	}
	return;
}

neat_texture* neat_create_texture_from_text(
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

void  neat_rect_fill(neat_window* w, SDL_Color* color, SDL_Rect* rect) {
	SDL_Color  old_color;
	SDL_GetRenderDrawColor(
		w->renderer,
		&old_color.r,
		&old_color.g,
		&old_color.b,
		&old_color.a
	);
	SDL_SetRenderDrawColor(
		w->renderer,
		color->r,
		color->g,
		color->b, color->a
	);
	int i = SDL_RenderFillRect(w->renderer, rect);

	SDL_SetRenderDrawColor(
		w->renderer,
		old_color.r,
		old_color.g,
		old_color.b,
		old_color.a
	);
}