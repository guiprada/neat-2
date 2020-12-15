// guiprada@gmail.com  Guilherme Cunha Prada 2017-2020 : )
// zlib license

#pragma once

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "neat.h"

static neat_window* check_and_get_window(lua_State* L, int stack_pos);
static neat_sprite* check_and_get_sprite(lua_State* L, int stack_pos);
static neat_texture* check_and_get_texture(lua_State* L, int stack_pos);
static SDL_Color* check_and_get_color(lua_State* L, int stack_pos);
static TTF_Font* check_and_get_font(lua_State* L, int stack_pos);

static int lua_window_create(lua_State* L);
static int lua_window_destroy(lua_State* L);
static int lua_window_hide(lua_State* L);
static int lua_window_show(lua_State* L);
static int lua_window_is_hidden(lua_State* L);
static int lua_window_is_shown(lua_State* L);

static int lua_sprite_create(lua_State* L);
static int lua_sprite_destroy(lua_State* L);
static int lua_sprite_render(lua_State* L);
static int lua_sprite_move(lua_State* L);
static int lua_sprite_move_to(lua_State* L);
static int lua_sprite_set_rotation(lua_State* L);
static int lua_sprite_set_anchor(lua_State* L);
static int lua_sprite_set_source_rect(lua_State* L);
static int lua_sprite_set_scale(lua_State* L);

static int lua_texture_create(lua_State* L);
static int lua_texture_create_from_text(lua_State* L);
static int lua_texture_destroy(lua_State* L);

static int lua_font_create(lua_State* L);
static int lua_font_destroy(lua_State* L);

static int lua_rect_fill(lua_State* L);

static int lua_render_clear(lua_State* L);
static int lua_render_show(lua_State* L);

static int lua_new_color(lua_State* L);

static int lua_handle_events(lua_State* L);
static int lua_is_key_pressed(lua_State* L);
static int lua_define_scancodes(lua_State* L);

////////////////////////////////////////////////////////////////////////////////

static const struct luaL_Reg neat_lib_functions[] = {
	{"new_window", lua_window_create},
	{"new_sprite", lua_sprite_create},
	{"new_texture", lua_texture_create},
	{"new_text_texture", lua_texture_create_from_text},
	{"new_color", lua_new_color},
	{"new_font", lua_font_create},
	{"rect_fill", lua_rect_fill},
	{"get_event", lua_handle_events},
	{"is_key_pressed", lua_is_key_pressed},
	{"define_scancodes", lua_define_scancodes},
	{NULL, NULL}
};

static const struct luaL_Reg neat_window_methods[] = {
	{"present", lua_render_show},
	{"clear", lua_render_clear},
	{"hide", lua_window_hide},
	{"show", lua_window_show},
	{"is_hidden", lua_window_is_hidden},
	{"is_shown", lua_window_is_shown},
	{"__gc", lua_window_destroy},
	{NULL, NULL}
};

static const struct luaL_Reg neat_sprite_methods[] = {
	{"render", lua_sprite_render},
	{"move", lua_sprite_move},
	{"move_to", lua_sprite_move_to},
	{"set_rotation", lua_sprite_set_rotation},
	{"set_anchor", lua_sprite_set_anchor},
	{"set_source", lua_sprite_set_source_rect},
	{"set_scale", lua_sprite_set_scale},
	{"__gc", lua_sprite_destroy},
	{NULL, NULL}
};

static const struct luaL_Reg neat_texture_methods[] = {
	{"__gc", lua_texture_destroy},
	{NULL, NULL}
};

static const struct luaL_Reg neat_font_methods[] = {
	{"__gc", lua_font_destroy},
	{NULL, NULL}
};

int lua_open_neat(lua_State* L) {
	luaL_newmetatable(L, "neat.font");
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);  /* pushes the metatable */
	lua_settable(L, -3);  /* metatable.__index = metatable */
	luaL_setfuncs(L, neat_font_methods, 0);

	luaL_newmetatable(L, "neat.color");
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);  /* pushes the metatable */
	lua_settable(L, -3);  /* metatable.__index = metatable */

	luaL_newmetatable(L, "neat.sprite");
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);  /* pushes the metatable */
	lua_settable(L, -3);  /* metatable.__index = metatable */
	luaL_setfuncs(L, neat_sprite_methods, 0);

	luaL_newmetatable(L, "neat.window");
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);  /* pushes the metatable */
	lua_settable(L, -3);  /* metatable.__index = metatable */
	luaL_setfuncs(L, neat_window_methods, 0);

	luaL_newmetatable(L, "neat.texture");
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);  /* pushes the metatable */
	lua_settable(L, -3);  /* metatable.__index = metatable */
	luaL_setfuncs(L, neat_texture_methods, 0);

	lua_getglobal(L, "neat");
	if (lua_isnil(L, -1)) {
		lua_pop(L, 1);
		lua_newtable(L);
	}
	luaL_setfuncs(L, neat_lib_functions, 0);
	lua_setglobal(L, "neat");

	return 1;
}

////////////////////////////////////////////////////////////////////////////////

static neat_window* check_and_get_window(lua_State* L, int stack_pos) {
	void** maybe_window = (void**)luaL_checkudata(L, stack_pos, "neat.window");
	luaL_argcheck(L, *maybe_window != NULL, stack_pos, "'window' expected");
	return (neat_window*)*(maybe_window);
}

static neat_sprite* check_and_get_sprite(lua_State* L, int stack_pos) {
	void** maybe_sprite = (void**)luaL_checkudata(L, stack_pos, "neat.sprite");
	luaL_argcheck(L, *maybe_sprite != NULL, stack_pos, "'sprite' expected");
	return (neat_sprite*)*(maybe_sprite);
}

static neat_texture* check_and_get_texture(lua_State* L, int stack_pos) {
	void** maybe_texture = (void**)luaL_checkudata(L, stack_pos, "neat.texture");
	luaL_argcheck(L, *maybe_texture != NULL, stack_pos, "'texture' expected");
	return (neat_texture*)*(maybe_texture);
}

static SDL_Color* check_and_get_color(lua_State* L, int stack_pos) {
	void* maybe_color = (void**)luaL_checkudata(L, stack_pos, "neat.color");
	luaL_argcheck(L, maybe_color != NULL, stack_pos, "'color' expected");
	return (SDL_Color*)maybe_color;
}

static TTF_Font* check_and_get_font(lua_State* L, int stack_pos) {
	void** maybe_font = (void**)luaL_checkudata(L, stack_pos, "neat.font");
	luaL_argcheck(L, *maybe_font != NULL, stack_pos, "'font' expected");
	return (TTF_Font*)*(maybe_font);
}

///////////////////////////////////////////////////////////////////////// Window
static int lua_window_create(lua_State* L) {
	int n = lua_gettop(L);
	if (n != 5) return 0;
	for (int i = 1; i <= n; i++) {
		if (!lua_isstring(L, i)) {
			lua_pushstring(L, "Incorrect argument to lua_create_window()");
			lua_error(L);
		}
	}
	neat_window** window = (neat_window**)lua_newuserdata(
		L,
		sizeof(neat_window*)
	);
	*window = neat_window_create(
		lua_tostring(L, 1),
		(int)lua_tointeger(L, 2),
		(int)lua_tointeger(L, 3),
		(int)lua_tointeger(L, 4),
		(int)lua_tointeger(L, 5));

	luaL_getmetatable(L, "neat.window");
	lua_setmetatable(L, -2);

	Uint32 windowID = SDL_GetWindowID((*window)->window);
	lua_pushinteger(L, windowID);
	return 2;
}

static int lua_window_destroy(lua_State* L) {
	int n = lua_gettop(L);
	if (n != 1) return 0;

	neat_window_destroy(check_and_get_window(L, 1));

	return 0;
}

static int lua_window_hide(lua_State* L) {
	int n = lua_gettop(L);
	if (n != 1) return 0;

	neat_window* window = check_and_get_window(L, 1);
	window->hidden = true;
	SDL_HideWindow(window->window);

	return 0;
}

static int lua_window_show(lua_State* L) {
	int n = lua_gettop(L);
	if (n != 1) return 0;

	neat_window* window = check_and_get_window(L, 1);
	window->hidden = false;
	SDL_ShowWindow(window->window);

	return 0;
}

static int lua_window_is_hidden(lua_State* L) {
	int n = lua_gettop(L);
	if (n != 1) return 0;

	neat_window* window = check_and_get_window(L, 1);
	lua_pushboolean(L, window->hidden);
	return 1;
}

static int lua_window_is_shown(lua_State* L) {
	int n = lua_gettop(L);
	if (n != 1) return 0;

	neat_window* window = check_and_get_window(L, 1);
	lua_pushboolean(L, !window->hidden);
	return 1;
}

///////////////////////////////////////////////////////////////////////// Sprite

static int lua_sprite_create(lua_State* L) {
	int n = lua_gettop(L);
	if (n != 1) return 0;

	neat_texture* t = check_and_get_texture(L, 1);

	neat_sprite** sprite = (neat_sprite**)lua_newuserdata(
		L,
		sizeof(neat_sprite*)
	);
	*sprite = neat_sprite_create(t);
	luaL_getmetatable(L, "neat.sprite");
	lua_setmetatable(L, -2);

	//already on the stack
	return 1;
}

static int lua_sprite_destroy(lua_State* L) {
	int n = lua_gettop(L);
	if (n != 1) return 0;

	neat_sprite_destroy(check_and_get_sprite(L, 1));
	return 0;
}

static int lua_sprite_render(lua_State* L) {
	int n = lua_gettop(L);
	if (n != 1) return 0;
	neat_sprite_render(check_and_get_sprite(L, 1));
	return 0;
}

static int lua_sprite_move(lua_State* L) {
	int n = lua_gettop(L);
	if (n != 3) return 0;

	if (!lua_isnumber(L, 2)) {
		lua_pushstring(L, "Incorrect argument to lua_sprite_move()");
		lua_error(L);
	}
	if (!lua_isnumber(L, 3)) {
		lua_pushstring(L, "Incorrect argument to lua_sprite_move()");
		lua_error(L);
	}

	neat_sprite* sp = check_and_get_sprite(L, 1);
	neat_sprite_move(sp, (double)lua_tonumber(L, 2), (double)lua_tonumber(L, 3));
	
	return 0;
}

static int lua_sprite_move_to(lua_State* L) {
	int n = lua_gettop(L);
	if (n != 3) return 0;

	if (!lua_isnumber(L, 2)) {
		lua_pushstring(L, "Incorrect argument to lua_sprite_move_to()");
		lua_error(L);
	}
	if (!lua_isnumber(L, 3)) {
		lua_pushstring(L, "Incorrect argument to lua_sprite_move_to()");
		lua_error(L);
	}

	neat_sprite* sp = check_and_get_sprite(L, 1);
	neat_sprite_move_to(sp, (double)lua_tonumber(L, 2), (double)lua_tonumber(L, 3));

	return 0;
}

static int lua_sprite_set_rotation(lua_State* L) {
	int n = lua_gettop(L);
	if (n != 2) return 0;

	if (!lua_isnumber(L, 2)) {
		lua_pushstring(L, "Incorrect argument to lua_sprite_set_rotation()");
		lua_error(L);
	}

	neat_sprite* sp = check_and_get_sprite(L, 1);
	sp->rotation = lua_tonumber(L, 2);
	return 0;
}
static int lua_sprite_set_anchor(lua_State* L) {
	int n = lua_gettop(L);
	if (n != 3) return 0;

	if (!lua_isinteger(L, 2)) {
		lua_pushstring(L, "Incorrect argument to lua_sprite_set_anchor()");
		lua_error(L);
	}
	if (!lua_isinteger(L, 3)) {
		lua_pushstring(L, "Incorrect argument to lua_sprite_set_anchor()");
		lua_error(L);
	}

	neat_sprite* sp = check_and_get_sprite(L, 1);
	neat_sprite_set_anchor(sp, (int)lua_tointeger(L, 2), (int)lua_tointeger(L, 3));

	return 0;
}

static int lua_sprite_set_source_rect(lua_State* L) {
	int n = lua_gettop(L);
	if (n != 5) return 0;


	if (!lua_isinteger(L, 2)) {
		lua_pushstring(L, "Incorrect argument to lua_sprite_move()");
		lua_error(L);
	}
	if (!lua_isinteger(L, 3)) {
		lua_pushstring(L, "Incorrect argument to lua_sprite_move()");
		lua_error(L);
	}
	if (!lua_isinteger(L, 4)) {
		lua_pushstring(L, "Incorrect argument to lua_sprite_move()");
		lua_error(L);
	}
	if (!lua_isinteger(L, 5)) {
		lua_pushstring(L, "Incorrect argument to lua_sprite_move()");
		lua_error(L);
	}

	neat_sprite* sp = check_and_get_sprite(L, 1);
	neat_sprite_set_source_rect(
		sp,
		(int)lua_tointeger(L, 2),
		(int)lua_tointeger(L, 3),
		(int)lua_tointeger(L, 4),
		(int)lua_tointeger(L, 5));

	return 0;
}

static int lua_sprite_set_scale(lua_State* L) {
	int n = lua_gettop(L);
	if (n != 3) return 0;

	if (!lua_isnumber(L, 2)) {
		lua_pushstring(L, "Incorrect argument to lua_sprite_move_to()");
		lua_error(L);
	}
	if (!lua_isnumber(L, 3)) {
		lua_pushstring(L, "Incorrect argument to lua_sprite_move_to()");
		lua_error(L);
	}

	neat_sprite* sp = check_and_get_sprite(L, 1);
	neat_sprite_set_scale(sp, (double)lua_tonumber(L, 2), (double)lua_tonumber(L, 3));

	return 0;
}

//////////////////////////////////////////////////////////////////////// Texture

static int lua_texture_create(lua_State* L) {
	int n = lua_gettop(L);
	if (n != 2) return 0;

	if (!lua_isstring(L, 2)) {
		lua_pushstring(L, "Incorrect argument to lua_texture_create()");
		lua_error(L);
	}

	neat_window* window = check_and_get_window(L, 1);

	neat_texture* new_texture = neat_texture_create(
		window,
		lua_tostring(L, 2)
	);
	neat_texture** tex = ((neat_texture**)lua_newuserdata(
		L,
		sizeof(neat_texture*))
		);

	*tex = new_texture;

	luaL_getmetatable(L, "neat.texture");
	lua_setmetatable(L, -2);

	return 1;
}

static int lua_texture_create_from_text(lua_State* L) {
	int n = lua_gettop(L);
	if (n != 4 && n != 3) return 0;

	if (!lua_isstring(L, 2)) {
		lua_pushstring(
			L,
			"Incorrect argument to lua_texture_create_from_text()"
		);
		lua_error(L);
	}

	TTF_Font* font = check_and_get_font(L, 3);
	neat_window* window = check_and_get_window(L, 1);

	SDL_Color  color = YELLOW;
	if (n == 4) {
		color = *check_and_get_color(L, 4);
	}

	neat_texture* new_texture = neat_texture_create_from_text(
		window,
		font,
		color,
		lua_tostring(L, 2)
	);
	neat_texture** tex = ((neat_texture**)lua_newuserdata(
		L,
		sizeof(neat_texture*))
		);

	*tex = new_texture;

	luaL_getmetatable(L, "neat.texture");
	lua_setmetatable(L, -2);

	return 1;
}

static int lua_texture_destroy(lua_State* L) {
	int n = lua_gettop(L);
	if (n != 1) return 0;

	neat_texture_destroy(check_and_get_texture(L, 1));
	return 0;
}

////////////////////////////////////////////////////////////////////////// Fonts

static int lua_font_create(lua_State* L) {
	int n = lua_gettop(L);
	if (n != 2) return 0;

	if (!lua_isstring(L, 1)) {
		lua_pushstring(L, "Incorrect argument to lua_font_create()");
		lua_error(L);
	}

	if (!lua_isinteger(L, 2)) {
		lua_pushstring(L, "Incorrect argument to lua_font_create()");
		lua_error(L);
	}

	TTF_Font** font = (TTF_Font**)lua_newuserdata(L, sizeof(TTF_Font**));
	*font = TTF_OpenFont(lua_tostring(L, 1), (int)lua_tointeger(L, 2));
	luaL_getmetatable(L, "neat.font");
	lua_setmetatable(L, -2);

	return 1;
}

static int lua_font_destroy(lua_State* L) {
	int n = lua_gettop(L);
	if (n != 1) return 0;

	TTF_Font* font = check_and_get_font(L, 1);
	if (font != NULL) {
		TTF_CloseFont(font);
		font = NULL;
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////// Graphics

static int lua_rect_fill(lua_State* L) {
	int n = lua_gettop(L);
	if (n != 6) return 0;

	neat_window* w = check_and_get_window(L, 1);
	SDL_Color* color = check_and_get_color(L, 2);

	SDL_Rect rect;
	if (!lua_isinteger(L, 3)) {
		lua_pushstring(L, "Incorrect argument to lua_rect_fill()");
		lua_error(L);
	}
	else rect.x = (int)lua_tointeger(L, 3);

	if (!lua_isinteger(L, 4)) {
		lua_pushstring(L, "Incorrect argument to lua_rect_fill()");
		lua_error(L);
	}
	else rect.y = (int)lua_tointeger(L, 4);
	if (!lua_isinteger(L, 5)) {
		lua_pushstring(L, "Incorrect argument to lua_rect_fill()");
		lua_error(L);
	}
	else rect.w = (int)lua_tointeger(L, 5);
	if (!lua_isinteger(L, 6)) {
		lua_pushstring(L, "Incorrect argument to lua_rect_fill()");
		lua_error(L);
	}
	else rect.h = (int)lua_tointeger(L, 6);

	neat_rect_fill(w, color, &rect);

	return 0;
}

///////////////////////////////////////////////////////////////////////// Render

static int lua_render_clear(lua_State* L) {
	int n = lua_gettop(L);
	if (n != 1) return 0;
	if (lua_isnil(L, 1)) return 0;

	neat_window* w = check_and_get_window(L, 1);
	SDL_RenderClear(w->renderer);
	return 0;
}

static int lua_render_show(lua_State* L) {
	int n = lua_gettop(L);
	if (n != 1) return 0;

	neat_window* w = check_and_get_window(L, 1);
	SDL_RenderPresent(w->renderer);

	return 0;
}

////////////////////////////////////////////////////////////////////////// Color

static int lua_new_color(lua_State* L) {
	int n = lua_gettop(L);
	if (n != 4) return 0;

	if (!lua_isinteger(L, 1)) {
		lua_pushstring(L, "Incorrect argument to lua_new_color()");
		lua_error(L);
	}
	if (!lua_isinteger(L, 2)) {
		lua_pushstring(L, "Incorrect argument to lua_new_color()");
		lua_error(L);
	}
	if (!lua_isinteger(L, 3)) {
		lua_pushstring(L, "Incorrect argument to lua_new_color()");
		lua_error(L);
	}
	if (!lua_isinteger(L, 4)) {
		lua_pushstring(L, "Incorrect argument to lua_new_color()");
		lua_error(L);
	}
	SDL_Color* new_color = (SDL_Color*)lua_newuserdata(L, sizeof(SDL_Color));
	new_color->r = (int)lua_tointeger(L, 1);
	new_color->g = (int)lua_tointeger(L, 2);
	new_color->b = (int)lua_tointeger(L, 3);
	new_color->a = (int)lua_tointeger(L, 4);
	luaL_getmetatable(L, "neat.color");
	lua_setmetatable(L, -2);
	return 1;
}

///////////////////////////////////////////////////////////////////////// Events

static int lua_handle_events(lua_State* L) {// returns 0 to quit
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_MOUSEMOTION:
			lua_pushstring(L, "mouse_motion");
			lua_pushinteger(L, event.motion.windowID);
			lua_pushinteger(L, event.motion.x);
			lua_pushinteger(L, event.motion.y);
			return 4;
			break;
		case SDL_WINDOWEVENT:
			if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
				lua_pushstring(L, "quit");
				lua_pushinteger(L, event.window.windowID);
				return 2;
			}
			if (event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) {
				lua_pushstring(L, "focus");
				lua_pushinteger(L, event.window.windowID);
				return 2;
			}
			break;
		case SDL_KEYDOWN:
			lua_pushstring(L, "keydown");
			lua_pushinteger(L, event.key.windowID);
			lua_pushstring(L, SDL_GetKeyName(event.key.keysym.sym));

			return 3;
			break;
		case SDL_KEYUP:
			lua_pushstring(L, "keyup");
			lua_pushinteger(L, event.key.windowID);
			lua_pushstring(L, SDL_GetKeyName(event.key.keysym.sym));
			return 3;
			break;
		default:
			//printf("Unhandled Event! \n");
			return 0;
		}
		return 0;
	}

	lua_pushboolean(L, false);
	return 1;
}

static int lua_is_key_pressed(lua_State* L) {// returns 0 to quit
	int n = lua_gettop(L);
	if (n != 1) {
		lua_pushstring(L, "Incorrect number of arguments to lua_is_key_pressed()");
		lua_error(L);
	}

	if (!lua_isinteger(L, 1)) {
		lua_pushstring(L, "Incorrect argument to lua_is_key_pressed()");
		lua_error(L);
	}

	SDL_Scancode scan_code = (SDL_Scancode)lua_tointeger(L, 1);
	SDL_PumpEvents();//update keyboardState
	const Uint8* key_state = SDL_GetKeyboardState(NULL);
	bool is_pressed = 0;
	if (key_state[scan_code]) {
		is_pressed = 1;
	}

	lua_pushboolean(L, is_pressed);
	return 1;
}

static int lua_define_scancodes(lua_State* L) {
	int num_keys;
	const Uint8* key_state = SDL_GetKeyboardState(&num_keys);

	lua_createtable(L, num_keys, 0);

	for (int i = 0; i < num_keys; i++) {
		lua_pushinteger(L, i);
		lua_setfield(L, -2, SDL_GetKeyName(SDL_GetKeyFromScancode((SDL_Scancode)i)));
	}

	return 1;
}