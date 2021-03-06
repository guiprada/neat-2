// guiprada@gmail.com  Guilherme Cunha Prada 2017-2020 : )
// zlib license

#pragma once

#define LUA_ERROR_STRING_BUFFER_SIZE 255

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "neat.h"

void LUA_ERROR(lua_State* L, char* message) {
	lua_Debug debug;
	lua_getstack(L, 1, &debug);
	lua_getinfo(L, "nSl", &debug);
	int line = debug.currentline;
	char* source = debug.short_src;
	const char* function = debug.name;

	printf("ERROR on file: %s -- function: %s -- line: %d\n", source, function, line);
	
	lua_pushstring(L, message);
	lua_error(L);
}

////////////////////////////////////////////////////////////////////////////////

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
static int lua_sprite_add_rotation(lua_State* L);
static int lua_sprite_set_anchor(lua_State* L);
static int lua_sprite_set_source_rect(lua_State* L);
static int lua_sprite_set_scale(lua_State* L);
static int lua_sprite_get_width(lua_State* L);
static int lua_sprite_get_height(lua_State* L);
static int lua_sprite_get_dimensions(lua_State* L);

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
	{"new_color", lua_new_color},
	{"new_font", lua_font_create},
	{"dispatch_events", lua_handle_events},
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
	{"new_texture", lua_texture_create},
	{"new_text_texture", lua_texture_create_from_text},
	{"new_sprite", lua_sprite_create},
	{"rect_fill", lua_rect_fill},
	{"__gc", lua_window_destroy},
	{NULL, NULL}
};

static const struct luaL_Reg neat_sprite_methods[] = {
	{"render", lua_sprite_render},
	{"move", lua_sprite_move},
	{"move_to", lua_sprite_move_to},
	{"set_rotation", lua_sprite_set_rotation},
	{"add_rotation", lua_sprite_add_rotation},
	{"set_anchor", lua_sprite_set_anchor},
	{"set_source", lua_sprite_set_source_rect},
	{"set_scale", lua_sprite_set_scale},
	{"get_width", lua_sprite_get_width},
	{"get_height", lua_sprite_get_height},
	{"get_dimensions", lua_sprite_get_dimensions},
	{"__gc", lua_sprite_destroy},
	{NULL, NULL}
};

static const struct luaL_Reg neat_texture_methods[] = {
	{"destroy", lua_texture_destroy},
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
	if (n != 5) {
		LUA_ERROR(L, "Incorrect number of arguments to lua_window_create()");		
	}
	for (int i = 1; i <= n; i++) {
		if (!lua_isstring(L, i)) {
			LUA_ERROR(L, "Incorrect argument to lua_window_create()");
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
	if (n != 1) {
		LUA_ERROR(L, "Incorrect number of arguments to lua_window_destroy()");
	}
	
	neat_window* window = check_and_get_window(L, 1);
	
	neat_window_destroy(window);

	return 0;
}

static int lua_window_hide(lua_State* L) {
	int n = lua_gettop(L);
	if (n != 1) {
		LUA_ERROR(L, "Incorrect number of arguments to lua_window_hide()");
	}

	neat_window* window = check_and_get_window(L, 1);
	window->hidden = true;
	SDL_HideWindow(window->window);

	return 0;
}

static int lua_window_show(lua_State* L) {
	int n = lua_gettop(L);
	if (n != 1) {
		LUA_ERROR(L, "Incorrect number of arguments to lua_window_show()");
	}

	neat_window* window = check_and_get_window(L, 1);
	window->hidden = false;
	SDL_ShowWindow(window->window);

	return 0;
}

static int lua_window_is_hidden(lua_State* L) {
	int n = lua_gettop(L);
	if (n != 1) {
		LUA_ERROR(L, "Incorrect number of arguments to lua_window_is_hidden()");
	}

	neat_window* window = check_and_get_window(L, 1);
	lua_pushboolean(L, window->hidden);
	return 1;
}

static int lua_window_is_shown(lua_State* L) {
	int n = lua_gettop(L);
	if (n != 1) {
		LUA_ERROR(L, "Incorrect number of arguments to lua_window_is_shown()");
	}

	neat_window* window = check_and_get_window(L, 1);
	lua_pushboolean(L, !window->hidden);
	return 1;
}

///////////////////////////////////////////////////////////////////////// Sprite

static int lua_sprite_create(lua_State* L) {
	int n = lua_gettop(L);
	if (n != 2) {
		LUA_ERROR(L, "Incorrect number of arguments to lua_sprite_create()");
	}

	neat_window* w = check_and_get_window(L, 1);
	neat_texture* t = check_and_get_texture(L, 2);

	if (t->window != w) {
		LUA_ERROR(L, "lua_sprite_create() Error, texture does not belong to window");
	}

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
	if (n != 1) {
		LUA_ERROR(L, "Incorrect number of arguments to lua_sprite_destroy()");
	}

	neat_sprite_destroy(check_and_get_sprite(L, 1));
	return 0;
}

static int lua_sprite_render(lua_State* L) {
	int n = lua_gettop(L);
	if (n != 1) {
		LUA_ERROR(L, "Incorrect number of arguments to lua_sprite_render()");
	}
	neat_sprite_render(check_and_get_sprite(L, 1));
	return 0;
}

static int lua_sprite_move(lua_State* L) {
	int n = lua_gettop(L);
	if (n != 3) {
		LUA_ERROR(L, "Incorrect number of arguments to lua_sprite_move()");
	}

	if (!lua_isnumber(L, 2)) {
		LUA_ERROR(L, "Incorrect argument to lua_sprite_move()");
	}
	if (!lua_isnumber(L, 3)) {
		LUA_ERROR(L, "Incorrect argument to lua_sprite_move()");
	}

	neat_sprite* sp = check_and_get_sprite(L, 1);
	neat_sprite_move(sp, (double)lua_tonumber(L, 2), (double)lua_tonumber(L, 3));
	
	return 0;
}

static int lua_sprite_move_to(lua_State* L) {
	int n = lua_gettop(L);
	if (n != 3) {
		LUA_ERROR(L, "Incorrect number of arguments to lua_sprite_move_to()");
	}

	if (!lua_isnumber(L, 2)) {
		LUA_ERROR(L, "Incorrect argument to lua_sprite_move_to()");
	}
	if (!lua_isnumber(L, 3)) {
		LUA_ERROR(L, "Incorrect argument to lua_sprite_move_to()");
	}

	neat_sprite* sp = check_and_get_sprite(L, 1);
	neat_sprite_move_to(sp, (double)lua_tonumber(L, 2), (double)lua_tonumber(L, 3));

	return 0;
}

static int lua_sprite_set_rotation(lua_State* L) {
	int n = lua_gettop(L);
	if (n != 2) {
		LUA_ERROR(L, "Incorrect number of arguments to lua_sprite_set_rotation()");
	}

	if (!lua_isnumber(L, 2)) {
		LUA_ERROR(L, "Incorrect argument to lua_sprite_set_rotation()");
	}

	neat_sprite* sp = check_and_get_sprite(L, 1);
	sp->rotation = lua_tonumber(L, 2);
	return 0;
}

static int lua_sprite_add_rotation(lua_State* L) {
	int n = lua_gettop(L);
	if (n != 2) {
		LUA_ERROR(L, "Incorrect number of arguments to lua_sprite_add_rotation()");
	}

	if (!lua_isnumber(L, 2)) {
		LUA_ERROR(L, "Incorrect argument to lua_sprite_add_rotation()");
	}

	neat_sprite* sp = check_and_get_sprite(L, 1);
	sp->rotation += lua_tonumber(L, 2);
	return 0;
}

static int lua_sprite_set_anchor(lua_State* L) {
	int n = lua_gettop(L);
	if (n != 3) {
		LUA_ERROR(L, "Incorrect number of arguments to lua_sprite_set_anchor()");
	}

	if (!lua_isinteger(L, 2)) {
		LUA_ERROR(L, "Incorrect argument to lua_sprite_set_anchor()");
	}
	if (!lua_isinteger(L, 3)) {
		LUA_ERROR(L, "Incorrect argument to lua_sprite_set_anchor()");
	}

	neat_sprite* sp = check_and_get_sprite(L, 1);
	neat_sprite_set_anchor(sp, (int)lua_tointeger(L, 2), (int)lua_tointeger(L, 3));

	return 0;
}

static int lua_sprite_set_source_rect(lua_State* L) {
	int n = lua_gettop(L);
	if ((n != 5) && (n != 7)) {
		LUA_ERROR(L, "Incorrect number of arguments to lua_sprite_set_source_rect()");
	}

	if (!lua_isinteger(L, 2)) {
		LUA_ERROR(L, "Incorrect argument to lua_sprite_set_source_rect()");
	}
	if (!lua_isinteger(L, 3)) {
		LUA_ERROR(L, "Incorrect argument to lua_sprite_set_source_rect()");
	}
	if (!lua_isinteger(L, 4)) {
		LUA_ERROR(L, "Incorrect argument to lua_sprite_set_source_rect()");
	}
	if (!lua_isinteger(L, 5)) {
		LUA_ERROR(L, "Incorrect argument to lua_sprite_set_source_rect()");
	}

	neat_sprite* sprite = check_and_get_sprite(L, 1);
	if( n==5)
		neat_sprite_set_source_rect(
			sprite,
			(int)lua_tointeger(L, 2),
			(int)lua_tointeger(L, 3),
			(int)lua_tointeger(L, 4),
			(int)lua_tointeger(L, 5));
	else if (n==7) {
		if (!lua_isinteger(L, 6)) {
			LUA_ERROR(L, "Incorrect argument to lua_sprite_set_source_rect()");
		}
		if (!lua_isinteger(L, 7)) {
			LUA_ERROR(L, "Incorrect argument to lua_sprite_set_source_rect()");
		}
		neat_sprite_set_source_rect_and_anchor(
			sprite,
			(int)lua_tointeger(L, 2),
			(int)lua_tointeger(L, 3),
			(int)lua_tointeger(L, 4),
			(int)lua_tointeger(L, 5),
			(int)lua_tointeger(L, 6),
			(int)lua_tointeger(L, 7));
	}else {
		LUA_ERROR(L, "Incorrect number of arguments to lua_sprite_set_source_rect()");
	}

	return 0;
}

static int lua_sprite_set_scale(lua_State* L) {
	int n = lua_gettop(L);
	if (n != 3) {
		LUA_ERROR(L, "Incorrect number of arguments to lua_sprite_set_scale()");
	}

	if (!lua_isnumber(L, 2)) {
		LUA_ERROR(L, "Incorrect argument to lua_sprite_set_scale()");
	}
	if (!lua_isnumber(L, 3)) {
		LUA_ERROR(L, "Incorrect argument to lua_sprite_set_scale()");
	}

	neat_sprite* sprite = check_and_get_sprite(L, 1);
	neat_sprite_set_scale(sprite, (double)lua_tonumber(L, 2), (double)lua_tonumber(L, 3));

	return 0;
}

static int lua_sprite_get_width(lua_State* L) {
	int n = lua_gettop(L);
	if (n != 1) {
		LUA_ERROR(L, "Incorrect number of arguments to lua_sprite_get_width()");
	}
	neat_sprite* sprite = check_and_get_sprite(L, 1);
	lua_pushinteger(L, sprite->source_rect.w);
	return 1;
}

static int lua_sprite_get_height(lua_State* L) {
	int n = lua_gettop(L);
	if (n != 1) {
		LUA_ERROR(L, "Incorrect number of arguments to lua_sprite_get_height()");
	}
	neat_sprite* sprite = check_and_get_sprite(L, 1);
	lua_pushinteger(L, sprite->source_rect.h);
	return 1;
}

static int lua_sprite_get_dimensions(lua_State* L) {
	int n = lua_gettop(L);
	if (n != 1) {
		LUA_ERROR(L, "Incorrect number of arguments to lua_sprite_get_dimensions()");
	}
	neat_sprite* sprite = check_and_get_sprite(L, 1);
	lua_pushinteger(L, sprite->source_rect.w);
	lua_pushinteger(L, sprite->source_rect.h);
	return 2;
}

//////////////////////////////////////////////////////////////////////// Texture

static int lua_texture_create(lua_State* L) {
	int n = lua_gettop(L);
	neat_window* window = NULL;
	if (n == 2) {
		window = check_and_get_window(L, 1);
	}else {
		LUA_ERROR(L, "Incorrect number of arguments to lua_texture_create()");
	}

	if (!lua_isstring(L, 2)) {
		LUA_ERROR(L, "Incorrect argument to lua_texture_create()");
	}

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
	neat_window* window = NULL;
	if (n == 4) {
		window = check_and_get_window(L, 1);
	}else {
		LUA_ERROR(L, "Incorrect number of arguments to lua_texture_create_from_text()");
	}

	if (!lua_isstring(L, 2)) {
		LUA_ERROR(L, "Incorrect argument to lua_texture_create_from_text()");
	}

	TTF_Font* font = check_and_get_font(L, 3);

	SDL_Color  color = *check_and_get_color(L, 4);
	
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
	if (n != 1) {
		LUA_ERROR(L, "Incorrect number of arguments to lua_texture_destroy()");
	}

	neat_texture_destroy(check_and_get_texture(L, 1));
	return 0;
}

////////////////////////////////////////////////////////////////////////// Fonts

static int lua_font_create(lua_State* L) {
	int n = lua_gettop(L);
	if (n != 2) {
		LUA_ERROR(L, "Incorrect number of arguments to lua_font_create()");
	}

	if (!lua_isstring(L, 1)) {
		LUA_ERROR(L, "Incorrect argument to lua_font_create()");
	}

	if (!lua_isinteger(L, 2)) {
		LUA_ERROR(L, "Incorrect argument to lua_font_create()");
	}

	TTF_Font** font = (TTF_Font**)lua_newuserdata(L, sizeof(TTF_Font**));
	*font = TTF_OpenFont(lua_tostring(L, 1), (int)lua_tointeger(L, 2));
	luaL_getmetatable(L, "neat.font");
	lua_setmetatable(L, -2);
	
	//int TTF_GetFontKerningSizeGlyphs(TTF_Font *font, Uint16 previous_ch, Uint16 ch);
	//int TTF_GetFontKerningSizeGlyphs32(TTF_Font *font, Uint32 previous_ch, Uint32 ch);
	//Uint16 ch1, ch2;
	/*for (ch1 = 20; ch1 < 128; ch1++)
		for (ch2 = 20; ch2 < 128; ch2++) {
			int i = TTF_GetFontKerningSizeGlyphs(*font, ch1, ch2);
			if( i!=0) printf("kerning works %d\n", i);
		}
	*/
	// Render and cache all printable ASCII characters in solid black
	//SDL_Surface *screen;
	//SDL_Color color = { 0,0,0 };
	//SDL_Surface *glyph_cache[128 - 20];
	//Uint16 ch;
	//for (ch = 20; ch < 128; ++ch) {
	//	glyph_cache[ch - 20] = TTF_RenderGlyph_Solid(*font, ch, color);
	//}

	return 1;
}

static int lua_font_destroy(lua_State* L) {
	int n = lua_gettop(L);
	if (n != 1) {
		LUA_ERROR(L, "Incorrect number of arguments to lua_font_destroy()");
	}

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
	neat_window* window = NULL;
	if (n == 6) {
		window = check_and_get_window(L, 1);
	}
	else {
		LUA_ERROR(L, "Incorrect number of arguments to lua_rect_fill()");
	}
		
	SDL_Color* color = check_and_get_color(L, 6);

	SDL_Rect rect;
	if (!lua_isinteger(L, 2)) {
		LUA_ERROR(L, "Incorrect argument to lua_rect_fill()");
	}
	else rect.x = (int)lua_tointeger(L, 2);

	if (!lua_isinteger(L, 3)) {
		LUA_ERROR(L, "Incorrect argument to lua_rect_fill()");
	}
	else rect.y = (int)lua_tointeger(L, 3);
	if (!lua_isinteger(L, 4)) {
		LUA_ERROR(L, "Incorrect argument to lua_rect_fill()");
	}
	else rect.w = (int)lua_tointeger(L, 4);
	if (!lua_isinteger(L, 5)) {
		LUA_ERROR(L, "Incorrect argument to lua_rect_fill()");
	}
	else rect.h = (int)lua_tointeger(L, 5);

	neat_SDL_Rect_fill(window, color, &rect);

	return 0;
}

///////////////////////////////////////////////////////////////////////// Render

static int lua_render_clear(lua_State* L) {
	int n = lua_gettop(L);
	neat_window* window = NULL;
	if (n == 1) {
		window = check_and_get_window(L, 1);
	}else {
		LUA_ERROR(L, "Incorrect number of arguments to lua_render_clear()");
	}
		
	SDL_RenderClear(window->renderer);
	return 0;
}

static int lua_render_show(lua_State* L) {
	int n = lua_gettop(L);
	neat_window* window = NULL;
	if (n == 1) {
		window = check_and_get_window(L, 1);
	}else {
		LUA_ERROR(L, "Incorrect number of arguments to lua_render_show()");
	}

	if(window->hidden == false)
		SDL_RenderPresent(window->renderer);

	return 0;
}

////////////////////////////////////////////////////////////////////////// Color

static int lua_new_color(lua_State* L) {
	int n = lua_gettop(L);
	if (n != 4) {
		LUA_ERROR(L, "Incorrect number of arguments to lua_new_color()");
	}

	if (!lua_isinteger(L, 1)) {
		LUA_ERROR(L, "Incorrect argument to lua_new_color()");
	}
	if (!lua_isinteger(L, 2)) {
		LUA_ERROR(L, "Incorrect argument to lua_new_color()");
	}
	if (!lua_isinteger(L, 3)) {
		LUA_ERROR(L, "Incorrect argument to lua_new_color()");
	}
	if (!lua_isinteger(L, 4)) {
		LUA_ERROR(L, "Incorrect argument to lua_new_color()");
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
	while (SDL_PollEvent(&event)){
	//if (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_MOUSEMOTION:
			{
				int i;
				i = lua_getglobal(L, "on_mouse_motion");
				if (i == LUA_TFUNCTION) {
					lua_pushinteger(L, event.motion.windowID);
					lua_pushinteger(L, event.motion.x);
					lua_pushinteger(L, event.motion.y);
					lua_call(L, 3, 0);
				}
				else {
					lua_pop(L, 1);
				}
			}
			break;
		case SDL_WINDOWEVENT:
			switch (event.window.event) {
			case SDL_WINDOWEVENT_CLOSE:
				{
					int i;
					i = lua_getglobal(L, "on_window_close");
					if (i == LUA_TFUNCTION) {
						lua_pushinteger(L, event.window.windowID);
						lua_call(L, 1, 0);
					}
					else {
						lua_pop(L, 1);
					}
				}
				break;
			case SDL_WINDOWEVENT_FOCUS_GAINED:
				{
					int i;
					i = lua_getglobal(L, "on_focus_gained");
					if (i == LUA_TFUNCTION) {
						lua_pushinteger(L, event.window.windowID);
						lua_call(L, 1, 0);
					}
					else {
						lua_pop(L, 1);
					}
				}
				break;
			case SDL_WINDOWEVENT_FOCUS_LOST:
				{
					int i;
					i = lua_getglobal(L, "on_focus_lost");
					if (i == LUA_TFUNCTION) {
						lua_pushinteger(L, event.window.windowID);
						lua_call(L, 1, 0);
					}
					else {
						lua_pop(L, 1);
					}
				}
				break;
			};
			break;
		case SDL_KEYDOWN:
			{
				int i;
				i = lua_getglobal(L, "on_keydown");
				if (i == LUA_TFUNCTION) {
					lua_pushinteger(L, event.key.windowID);
					lua_pushstring(L, SDL_GetKeyName(event.key.keysym.sym));
					lua_call(L, 2, 0);
				}
				else {
					lua_pop(L, 1);
				}
			}
			break;
		case SDL_KEYUP:
			{
				int i;
				i = lua_getglobal(L, "on_keyup");
				if (i == LUA_TFUNCTION) {
					lua_pushinteger(L, event.key.windowID);
					lua_pushstring(L, SDL_GetKeyName(event.key.keysym.sym));
					lua_call(L, 2, 0);
				}
				else {
					lua_pop(L, 1);
				}
			}
			break;
		default:
			{
				//printf("Unhandled Event: %i \n", event.type);
				int i;
				i = lua_getglobal(L, "unhandled");
				if (i == LUA_TFUNCTION) {
					lua_pushinteger(L, event.type);
					lua_call(L, 1, 0);
				}
				else {
					lua_pop(L, 1);
				}
			}
			break;
		}
	}	
	return 0;
}

static int lua_is_key_pressed(lua_State* L) {// returns 0 to quit
	int n = lua_gettop(L);
	if (n != 1) {
		LUA_ERROR(L, "Incorrect number of arguments to lua_is_key_pressed()");
	}

	if (!lua_isinteger(L, 1)) {
		LUA_ERROR(L, "Incorrect argument to lua_is_key_pressed()");
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